#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <Core2/Debug.h>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class NLuaNode
{
public:
	lua_State* m_LuaState;
	std::string m_Path;

    std::vector<std::string> SplitPath(const std::string& path) {
        std::vector<std::string> result;
        size_t start = 0;
        size_t end = path.find('/');

        while (end != std::string::npos) {
            if (end > start) {
                result.push_back(path.substr(start, end - start));
            }
            start = end + 1;
            end = path.find('/', start);
        }

        if (start < path.length()) {
            result.push_back(path.substr(start));
        }

        return result;
    }

    virtual void RegisterFunction(std::string& function_name, lua_CFunction function)
    {
        std::vector<std::string>& walk_to = SplitPath(function_name);

        int index = lua_istable(m_LuaState, -1) ? -2 : LUA_GLOBALSINDEX;

        if (walk_to.size() > 1)
            for (std::vector<std::string>::iterator it = walk_to.begin(); it != walk_to.end() - 1; it++)
            {
                lua_pushstring(m_LuaState, it->c_str());
                lua_gettable(m_LuaState, index);
                index = -2;
            }

        lua_register(m_LuaState, walk_to.back().c_str(), function);
    }

	//method to get path (or walk to )

    // table1/table2/bool_varible
    virtual bool GetBool(const char* Path)
    {
        return GetBool(std::string(Path));
    }

    virtual std::vector<std::string> GetTableOfString(const char* Path)
    {
        return GetTableOfString(std::string(Path));
    }


    virtual std::vector<std::string> GetTableOfString(std::string& Path)
    {
        std::vector<std::string> result;
        if (!m_LuaState) return result;

        int initial = lua_gettop(m_LuaState);
        std::vector<std::string> walk_to = SplitPath(Path);

        // ALWAYS start from global table for absolute paths
        // If you want relative paths, use a different method
        lua_pushvalue(m_LuaState, LUA_GLOBALSINDEX);

        for (size_t i = 0; i < walk_to.size(); i++)
        {
            lua_pushstring(m_LuaState, walk_to[i].c_str());
            lua_gettable(m_LuaState, -2);

            if (lua_isnil(m_LuaState, -1)) {
                lua_settop(m_LuaState, initial);
                return result;
            }

            if (i < walk_to.size() - 1) {
                if (!lua_istable(m_LuaState, -1)) {
                    lua_settop(m_LuaState, initial);
                    return result;
                }
                lua_remove(m_LuaState, -2);
            }
        }

        if (lua_istable(m_LuaState, -1)) {
            lua_pushnil(m_LuaState);
            while (lua_next(m_LuaState, -2) != 0) {
                if (lua_isstring(m_LuaState, -1)) {
                    const char* str = lua_tostring(m_LuaState, -1);
                    if (str) result.push_back(str);
                }
                lua_pop(m_LuaState, 1);
            }
        }

        lua_settop(m_LuaState, initial);
        return result;
    }

    virtual bool GetBool(std::string& Path)
    {
        if (!m_LuaState) return false;

        int initial = lua_gettop(m_LuaState);
        std::vector<std::string> walk_to = SplitPath(Path);

        // Always start from GLOBAL table for absolute paths
        lua_pushvalue(m_LuaState, LUA_GLOBALSINDEX);

        for (size_t i = 0; i < walk_to.size(); i++)
        {
            lua_pushstring(m_LuaState, walk_to[i].c_str());
            lua_gettable(m_LuaState, -2);  // Get from current table

            if (lua_isnil(m_LuaState, -1)) {
                lua_settop(m_LuaState, initial);
                return false;
            }

            if (i < walk_to.size() - 1) {
                if (!lua_istable(m_LuaState, -1)) {
                    lua_settop(m_LuaState, initial);
                    return false;
                }
                lua_remove(m_LuaState, -2);  // Remove old table
            }
        }

        bool result = lua_toboolean(m_LuaState, -1) != 0;
        lua_settop(m_LuaState, initial);
        return result;
    }
};

class NLua : public NLuaNode
{
public:
    static int WriteVirtualBytes(lua_State* L)
    {
        const char* strOffset = lua_tostring(L, 1);
        const char* strBytes = lua_tostring(L, 2);

        // Convert offset
        char* end;
        unsigned long offset = strtoul(strOffset, &end, 16);
        void* pos = (void*)offset;

        // Calculate byte count
        size_t len = strlen(strBytes);
        size_t byteCount = len / 2;
        unsigned char* bytes = new unsigned char[byteCount];

        // Convert hex string to bytes
        for (size_t i = 0; i < byteCount; i++)
        {
            char hex[3] = { strBytes[i * 2], strBytes[i * 2 + 1], 0 };
            bytes[i] = (unsigned char)strtoul(hex, NULL, 16);
        }

        // Write memory
        HookNew::memcpy(pos, bytes, byteCount);
        delete[] bytes;

        return 0;
    }

    void RegisterMemoryFunctions()
    {
        RegisterFunction(std::string("WriteVirtualBytes"), &WriteVirtualBytes);
    }

    void LuaOpen()
    {
        m_LuaState = lua_open();
        if (!m_LuaState) return;

        lua_baselibopen(m_LuaState);
        lua_tablibopen(m_LuaState);
        lua_strlibopen(m_LuaState);
        lua_mathlibopen(m_LuaState);
        lua_dblibopen(m_LuaState);
    

        RegisterMemoryFunctions();
    }

    void LoadFile(std::string& path)
    {
        if (!m_LuaState) {
            HardwareMessage(L"ERROR", "Lua state not initialized");
            return;
        }

        LuaOpen();

        // Save initial stack size for cleanup
        int initialStack = lua_gettop(m_LuaState);

        std::ifstream file(path.c_str());
        if (!file.is_open()) {
            HardwareMessage(L"ERROR", "Failed to open file");
            return;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();

        // ===== LUA 5.0.2 with safer error handling =====

        // Step 1: Compile
        int compileResult = luaL_loadbuffer(m_LuaState, content.c_str(), content.size(), path.c_str());

        if (compileResult != 0) {
            // Get error SAFELY
            const char* error = NULL;
            if (lua_isstring(m_LuaState, -1)) {
                error = lua_tostring(m_LuaState, -1);
            }

            if (error) {
                HardwareMessage(L"LUA COMPILE ERROR", error);
            }
            else {
                HardwareMessage(L"LUA COMPILE ERROR", "Compilation failed (no error message)");
            }

            // Clean up: restore stack to initial state
            lua_settop(m_LuaState, initialStack);
            return;
        }

        // Step 2: Execute with protected call
        // Use index 0 for no error function (simplest)
        int execResult = lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);

        if (execResult != 0) {
            // Get error SAFELY
            const char* error = NULL;
            if (lua_isstring(m_LuaState, -1)) {
                error = lua_tostring(m_LuaState, -1);
            }

            char errorMsg[512];
            if (error) {
                sprintf(errorMsg, "Error %d: %s", execResult, error);
                HardwareMessage(L"LUA RUNTIME ERROR", errorMsg);
            }
            else {
                sprintf(errorMsg, "Runtime error %d (no message)", execResult);
                HardwareMessage(L"LUA RUNTIME ERROR", errorMsg);
            }

            // Clean up: restore stack to initial state
            lua_settop(m_LuaState, initialStack);
            return;
        }

        // Optional: Clean any return values from stack
        lua_settop(m_LuaState, initialStack);
    }
};
