cmake_minimum_required(VERSION 3.8)
project(Sonic2006DLL)

# ==================== PATHS ====================
set(XENON_SDK "${CMAKE_SOURCE_DIR}/thirdparty/XENON_SDK")
set(MARATHON_LIB "${CMAKE_SOURCE_DIR}/lib/Sonic2006LIB")
set(DLL_DIR "${CMAKE_SOURCE_DIR}/dll")

set(XENON_CL "${XENON_SDK}/bin/win32/cl.exe")
set(XENON_LINK "${XENON_SDK}/bin/win32/link.exe")
set(XENON_LIB "${XENON_SDK}/bin/win32/lib.exe")

# ==================== COMMON FLAGS ====================
set(COMMON_COMPILE_DEFS
    _XBOX
    XBOX
    _XENON
    _WIN32
)

set(COMMON_INCLUDE_DIRS
    "${XENON_SDK}/include"
    "${XENON_SDK}/include/win32"
    "${XENON_SDK}/include/xbox"
    "${MARATHON_LIB}/Marathon"
    "${MARATHON_LIB}/Boost"
    "${MARATHON_LIB}/Lua/include"
    "${MARATHON_LIB}/Lua/src"
    "${MARATHON_LIB}/Chao"
    "${DLL_DIR}"
)

# ==================== SOURCE FILES ====================
set(PROJECT_SOURCES
    "main.cpp"
    "Core2/Debug.cpp"
    "Core2/DebugFont.cpp"
    "Core2/Hook.cpp"
    "Patch/Api/FThread.cpp"
    "Patch/Api/FStepableThread.cpp"
    "Patch/Api/FFrameSynchronizerThread.cpp"
    "Patch/Api/FHavokRunnerThread.cpp"
    "Patch/DebugUI/DebugUI.cpp"
    "Patch/ArcFix/ArcFix.cpp"
    "Patch/RestoreRetaiRenderFunctions/RestoreGameimp.cpp"
    "Patch/RestoreRetaiRenderFunctions/RestoreLUAFunctions.cpp"
    "Patch/RestoreRetaiRenderFunctions/RestoreMainMenuTask.cpp"
    "Patch/RestoreRetaiRenderFunctions/RestoreSonicContext.cpp"
    "Patch/RestoreRetaiRenderFunctions/RestoreThreading.cpp"
)

file(GLOB_RECURSE PROJECT_HEADERS "*.h")

add_executable(DLL_VIEW ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(DLL_VIEW PRIVATE ${COMMON_INCLUDE_DIRS})
target_compile_definitions(DLL_VIEW PRIVATE ${COMMON_COMPILE_DEFS} WIN32)

set_target_properties(DLL_VIEW PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    EXCLUDE_FROM_DEFAULT_BUILD TRUE
    EXCLUDE_FROM_ALL TRUE
)

# Simulate 32 bit view
add_compile_options(-m32)
add_link_options(-m32)

file(GLOB_RECURSE MARATHON_SOURCES 
    "${MARATHON_LIB}/Marathon/*.cpp"
)

# Do it EXACTLY like Lua - copy-paste and modify:
set(MARATHON_OBJ_DIR "${CMAKE_BINARY_DIR}/marathon_obj")

foreach(marathon_src ${MARATHON_SOURCES})
    get_filename_component(obj_name ${marathon_src} NAME_WE)
    get_filename_component(file_ext ${marathon_src} EXT)
    
    # Делаем путь ОТНОСИТЕЛЬНО Marathon директории
    # Получаем полный путь
    get_filename_component(full_path ${marathon_src} ABSOLUTE)
    
    # Делаем его относительным от MARATHON_LIB
    file(RELATIVE_PATH rel_path "${MARATHON_LIB}" "${full_path}")
    
    # Теперь преобразуем в плоскую структуру
    get_filename_component(dir_part ${rel_path} DIRECTORY)
    get_filename_component(file_part ${rel_path} NAME_WE)
    
    if(dir_part)
        # Заменяем все разделители на подчеркивания
        string(REPLACE "/" "_" dir_flat "${dir_part}")
        string(REPLACE "\\" "_" dir_flat "${dir_flat}")
        
        # Убираем возможные точки и двоеточия
        string(REGEX REPLACE "[.:]" "" dir_flat "${dir_flat}")
        
        set(obj_base "${dir_flat}_${file_part}${file_ext}")
    else()
        set(obj_base "${file_part}${file_ext}")
    endif()

    set(obj "${MARATHON_OBJ_DIR}/${obj_base}.obj")
    
    message(STATUS "Source: ${marathon_src}")
    message(STATUS "Relative: ${rel_path}")
    message(STATUS "Object: ${obj}")
   
    # Convert path to Windows format with backslashes - LIKE LUA DOES
    file(TO_NATIVE_PATH "${obj}" obj_native)
    file(TO_NATIVE_PATH "${marathon_src}" src_native)
    
    add_custom_command(
        OUTPUT ${obj}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${MARATHON_OBJ_DIR}"
        COMMAND ${XENON_CL}
            /nologo
            /TP
            /O2
            /GS-
            /Zc:wchar_t-
            /MT
            /D_XBOX
            /DXBOX
            /D_XENON
            /D_WIN32
            /DBOOST_NO_EXCEPTIONS
            /DBOOST_DISABLE_THREADS
            /EHsc
            /D_LIB
            /X
            /I"${XENON_SDK}/include/win32"
            /I"${XENON_SDK}/include/xbox"
            /I"${MARATHON_LIB}/boost"
            /I"${MARATHON_LIB}/Lua/src"
            /I"${MARATHON_LIB}/Lua/include"
            /I"${MARATHON_LIB}"
            /I"${MARATHON_LIB}/Marathon"
            /I"${MARATHON_LIB}/Chao"
            /c
            "${src_native}"
            /Fo${obj_native}
        DEPENDS "${marathon_src}"
        VERBATIM
        COMMENT "Compiling Marathon: ${obj_base}"
    )
    list(APPEND MARATHON_OBJECTS ${obj})
endforeach()

# Create library EXACTLY like Lua
set(MARATHONLIB_PATH "${CMAKE_BINARY_DIR}/marathon.lib")
file(TO_NATIVE_PATH "${MARATHONLIB_PATH}" marathonlib_native)

set(MARATHON_RSP "${CMAKE_BINARY_DIR}/marathon.rsp")

file(WRITE "${MARATHON_RSP}" "/nologo\n")
file(APPEND "${MARATHON_RSP}" "/OUT:\"${CMAKE_BINARY_DIR}/marathon.lib\"\n")

foreach(obj ${MARATHON_OBJECTS})
    file(TO_NATIVE_PATH "${obj}" obj_native)
    file(APPEND "${MARATHON_RSP}" "\"${obj_native}\"\n")
endforeach()

list(LENGTH MARATHON_OBJECTS MARATHON_OBJECTS_COUNT)
message(STATUS "Created ${MARATHON_RSP} with ${MARATHON_OBJECTS_COUNT} objects")

# ТОЛЬКО ОДНА КОМАНДА для создания библиотеки!
add_custom_command(
    OUTPUT "${MARATHONLIB_PATH}"
    COMMAND ${XENON_LIB} 
        /nologo 
        /VERBOSE
        /OUT:${marathonlib_native}
        @${MARATHON_RSP}
    DEPENDS ${MARATHON_RSP} ${MARATHON_OBJECTS}
    COMMENT "Creating Marathon static library"
    VERBATIM
)

add_custom_target(Marathon_Lib ALL DEPENDS "${MARATHONLIB_PATH}")

# ==================== LUA 5.0 FOR XBOX 360 ====================
set(LUA_SRC_DIR "${MARATHON_LIB}/Lua/src")
set(LUA_LIB_DIR "${LUA_SRC_DIR}/lib")

# Core Lua library files
set(LUA_CORE_SOURCES
    "${LUA_SRC_DIR}/lapi.c"
    "${LUA_SRC_DIR}/lcode.c"
    "${LUA_SRC_DIR}/ldebug.c"
    "${LUA_SRC_DIR}/ldo.c"
    "${LUA_SRC_DIR}/ldump.c"
    "${LUA_SRC_DIR}/lfunc.c"
    "${LUA_SRC_DIR}/lgc.c"
    "${LUA_SRC_DIR}/llex.c"
    "${LUA_SRC_DIR}/lmem.c"
    "${LUA_SRC_DIR}/lobject.c"
    "${LUA_SRC_DIR}/lopcodes.c"
    "${LUA_SRC_DIR}/lparser.c"
    "${LUA_SRC_DIR}/lstate.c"
    "${LUA_SRC_DIR}/lstring.c"
    "${LUA_SRC_DIR}/ltable.c"
    "${LUA_SRC_DIR}/ltm.c"
    "${LUA_SRC_DIR}/lundump.c"
    "${LUA_SRC_DIR}/lvm.c"
    "${LUA_SRC_DIR}/lzio.c"
)

# Standard libraries from lib folder
set(LUA_LIB_SOURCES
    "${LUA_LIB_DIR}/lauxlib.c"
    "${LUA_LIB_DIR}/lbaselib.c"
    "${LUA_LIB_DIR}/ldblib.c"
    "${LUA_LIB_DIR}/liolib.c"
   # "${LUA_LIB_DIR}/loadlib.c"
    "${LUA_LIB_DIR}/lmathlib.c"
    "${LUA_LIB_DIR}/lstrlib.c"
    "${LUA_LIB_DIR}/ltablib.c"
)

set(LUA_SOURCES 
    ${LUA_CORE_SOURCES}
    ${LUA_LIB_SOURCES}
)

# Create output directory for Lua objects
set(LUA_OBJ_DIR "${CMAKE_BINARY_DIR}/lua_obj")

# Compile each Lua file
foreach(lua_src ${LUA_SOURCES})
    # Get relative path from Lua directory
    file(RELATIVE_PATH rel_path "${MARATHON_LIB}/Lua/src" "${lua_src}")
    get_filename_component(dir_path ${rel_path} DIRECTORY)
    get_filename_component(file_name ${lua_src} NAME_WE)
    
    # Create object path preserving directory structure
    if(dir_path)
        set(obj "${LUA_OBJ_DIR}/${dir_path}/${file_name}.obj")
    else()
        set(obj "${LUA_OBJ_DIR}/${file_name}.obj")
    endif()
    
    # Convert path to Windows format with backslashes
    file(TO_NATIVE_PATH "${obj}" obj_native)
    file(TO_NATIVE_PATH "${lua_src}" src_native)
    
    # Get directory for output object
    get_filename_component(obj_dir ${obj} DIRECTORY)
    
    add_custom_command(
        OUTPUT ${obj}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${obj_dir}"
        COMMAND ${XENON_CL}
            /nologo
            /TC
            /O2
            /GS-
            /Zc:wchar_t-
            /MT
            /D_XBOX
            /DXBOX
            /D_XENON
            /D_WIN32
            /D_LIB
            /DBOOST_NO_EXCEPTIONS
            /DBOOST_DISABLE_THREADS
            /EHsc
            /X
            /I"${XENON_SDK}/include/win32"
            /I"${XENON_SDK}/include/xbox"
            /I"${MARATHON_LIB}/Lua/src"
            /I"${MARATHON_LIB}/Lua/include"
            /c
            "${lua_src}"
            /Fo${obj_native}
        DEPENDS "${lua_src}"
        VERBATIM
        COMMENT "Compiling Lua: ${rel_path}"
    )
    list(APPEND LUA_OBJECTS ${obj})
endforeach()

# Create library
set(LUALIB_PATH "${CMAKE_BINARY_DIR}/lualib.lib")
file(TO_NATIVE_PATH "${LUALIB_PATH}" lualib_native)

add_custom_command(
    OUTPUT "${LUALIB_PATH}"
    COMMAND ${XENON_LIB} 
        /nologo 
        /OUT:${lualib_native}
        ${LUA_OBJECTS}
    DEPENDS ${LUA_OBJECTS}
    VERBATIM
    COMMENT "Creating Lua static library"
)

add_custom_target(Lua_Lib ALL DEPENDS "${LUALIB_PATH}")

# ==================== PROJECT COMPILATION ====================
# Create output directory for project objects
set(XBOX_OBJ_DIR "${CMAKE_BINARY_DIR}/xbox_obj")

# Compile project files
foreach(src ${PROJECT_SOURCES})
    # Get relative path from source directory
    file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    get_filename_component(dir_path ${rel_path} DIRECTORY)
    get_filename_component(file_name ${src} NAME_WE)
    get_filename_component(file_ext ${src} EXT)
    
    # Create object path preserving directory structure
    if(dir_path)
        set(obj "${XBOX_OBJ_DIR}/${dir_path}/${file_name}${file_ext}.obj")
    else()
        set(obj "${XBOX_OBJ_DIR}/${file_name}${file_ext}.obj")
    endif()
    
    # Convert path to Windows format with backslashes
    file(TO_NATIVE_PATH "${obj}" obj_native)
    
    # Get directory for output object
    get_filename_component(obj_dir ${obj} DIRECTORY)
    
    add_custom_command(
        OUTPUT ${obj}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${obj_dir}"
        COMMAND ${XENON_CL}
            /nologo
            /TP
            /O2
            /GS-
            /Zc:wchar_t-
            /MT
            /D_XBOX
            /DXBOX
            /D_XENON
            /DBOOST_NO_EXCEPTIONS
            /DBOOST_DISABLE_THREADS
            /EHsc
            /X
            /I"${XENON_SDK}/include/win32"
            /I"${XENON_SDK}/include/xbox"
            /I"${MARATHON_LIB}/Marathon"
            /I"${MARATHON_LIB}/Boost"
            /I"${MARATHON_LIB}/Lua/include"
            /I"${MARATHON_LIB}/Chao"
            /I"${MARATHON_LIB}/"
            /I"${DLL_DIR}"
            /c
            "${CMAKE_CURRENT_SOURCE_DIR}/${src}"
            /Fo${obj_native}
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${src}"
        VERBATIM
        COMMENT "Compiling ${rel_path}"
    )
    list(APPEND XBOX_OBJECTS ${obj})
endforeach()

# ==================== FINAL XEX LINKING ====================
set(XEX_OUTPUT "${CMAKE_BINARY_DIR}/Core.xex")
file(TO_NATIVE_PATH "${XEX_OUTPUT}" xex_native)

add_custom_command(
    OUTPUT "${XEX_OUTPUT}"
    COMMAND ${XENON_LINK}
        /nologo
        /DLL
        /MACHINE:PPCBE
        /entry:_DllMainCRTStartup
        /include:XboxKrnlBuildNumber
        /ignore:4089
        /nodefaultlib
        /DEBUG
        /DEBUGTYPE:cv
        /PDB:${xex_native}.pdb
        /INCREMENTAL:YES
        /OPT:NOREF
        /OUT:${xex_native}
        ${XBOX_OBJECTS}
        "${MARATHONLIB_PATH}"
        "${LUALIB_PATH}"
        "${XENON_SDK}/lib/xbox/libcmt.lib"
        "${XENON_SDK}/lib/xbox/libcpMT.lib"
        "${XENON_SDK}/lib/xbox/xapilib.lib"
        "${XENON_SDK}/lib/xbox/xboxkrnl.lib"
        "${XENON_SDK}/lib/xbox/xbdm.lib"
        "${XENON_SDK}/lib/xbox/xgraphics.lib"
        "${XENON_SDK}/lib/xbox/d3d9.lib"
        "${XENON_SDK}/lib/xbox/d3dx9.lib"
        /FORCE:MULTIPLE
    DEPENDS ${XBOX_OBJECTS} "${MARATHONLIB_PATH}" "${LUALIB_PATH}"
    VERBATIM
    COMMENT "Linking XBOX 360 XEX"
)

add_custom_target(DLL_BUILD ALL 
    DEPENDS "${XEX_OUTPUT}"
    SOURCES ${PROJECT_SOURCES}
)

add_dependencies(DLL_BUILD Marathon_Lib Lua_Lib)

# ==================== UTILITY TARGETS ====================
# Clean target to remove generated files
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/lua_obj"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/marathon_obj"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/xbox_obj"
    COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/lualib.lib"
    COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/marathon.lib"
    COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_BINARY_DIR}/Core.xex"
    COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_SOURCE_DIR}/Core.xex"
    COMMENT "Cleaning all build artifacts"
)