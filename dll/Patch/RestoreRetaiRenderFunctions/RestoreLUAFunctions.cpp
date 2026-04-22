#include "RestoreLUAFunctions.h"
#include <SceneLODParam.h>
#include <LuaSystem.h>
#include <Core2/Hook.h>
#include <boost/shared_ptr.hpp>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

using namespace Patch::RestoreLUAFunctions;

int LUA_Resolve(lua_State* L)
{
    return 1;
}

int LUA_ResetRenderStates(lua_State* L)
{
    return 1;
}

int LUA_LoadTexture(lua_State* L)
{
    return 0;
}

int LUA_GetDepthStencilFormat(lua_State* L)
{
    const char* format = HookBranchTo(0x8256D060, const char*, 7);
    lua_pushstring(L, format);
    return 1;
}


static luaL_reg renderer_functions[] = {
    { "ResetRenderTargets", (lua_CFunction)0x8256C048 },
    { "CreateDefaultRenderTargets", (lua_CFunction)0x8256C078 },
    { "CreateSurface", (lua_CFunction)0x82570328 },
    { "CreateTexture", (lua_CFunction)0x825704B0 },
    { "CreateCubeTexture", (lua_CFunction)0x82570658 },
    { "CreateArrayTexture", (lua_CFunction)0x825707E0 },
    { "CreateDepthStencilSurface", (lua_CFunction)0x82570988 },
    { "SetLabel", (lua_CFunction)0x82570B10 },
    { "BeginBlock", (lua_CFunction)0x82570C18 },
    { "EndBlock", (lua_CFunction)0x82570D98 },
    { "SetFrameBufferObject", (lua_CFunction)0x825711A8 },
    { "SetFrameBufferObjectOnce", (lua_CFunction)0x82571260 },
    { "ClearRenderTarget", (lua_CFunction)0x82571318 },
    { "SetColorWriteEnable", (lua_CFunction)0x82575010 },
    { "SetViewport", (lua_CFunction)0x825714E0 },
    { "ResetViewport", (lua_CFunction)0x82571638 },
    { "SetScissorTest", (lua_CFunction)0x82571700 },
    { "SetScissorRect", (lua_CFunction)0x825718A8 },
    { "ResetScissorRect", (lua_CFunction)0x825719D8 },
    { "AutoSetAspect", (lua_CFunction)0x82571AA0 },
    { "SetCullMode", (lua_CFunction)0x825752A8 },
    { "LockCullMode", (lua_CFunction)0x82575550 },
    { "SetZMode", (lua_CFunction)0x82575718 },
    { "LockZMode", (lua_CFunction)0x825758B8 },
    { "SetBlendMode", (lua_CFunction)0x82575A80 },
    { "LockBlendMode", (lua_CFunction)0x82575C88 },
    { "SetStencilEnable", (lua_CFunction)0x82571B70 },
    { "SetStencilOp", (lua_CFunction)0x82571D80 },
    { "SetBackStencilOp", (lua_CFunction)0x82571FB8 },
    { "SetStencilFunc", (lua_CFunction)0x825721F0 },
    { "SetStencilWriteMask", (lua_CFunction)0x82572388 },
    { "ApplyDevice", (lua_CFunction)0x82572480 },
    { "CopyTexture", (lua_CFunction)0x82572548 },
    { "Capture", (lua_CFunction)0x82572768 },
    { "ApplySceneParams", (lua_CFunction)0x82572B70 },
    { "SetClip", (lua_CFunction)0x82572C38 },
    { "SetScreen", (lua_CFunction)0x82572D40 },
    { "SetFovY", (lua_CFunction)0x82572E48 },
    { "SetUserClipPlaneEnable", (lua_CFunction)0x82572F40 },
    { "SetTexture", (lua_CFunction)0x82573090 },
    { "SetDepthTextures", (lua_CFunction)0x82575E50 },
    { "SetCSMTextures", (lua_CFunction)0x82576090 },
    { "SetReflectionTextures", (lua_CFunction)0x825762D0 },
    { "SetConstantShader", (lua_CFunction)0x82576510 },
    { "ColorFill", (lua_CFunction)0x82573308 },
    { "MakeBloom", (lua_CFunction)0x82573428 },
    { "ApplyBloom", (lua_CFunction)0x82573768 },
    { "Rasterize", (lua_CFunction)0x82573830 },
    { "RasterizeBurnoutBlur", (lua_CFunction)0x82573A28 },
    { "RenderWorld", (lua_CFunction)0x82576840 },
    { "RenderSpanverse", (lua_CFunction)0x82573C50 },
    { "RenderCSD", (lua_CFunction)0x82573F88 },
    { "RenderDebug3DFont", (lua_CFunction)0x82574100 },
    { "RenderDebug", (lua_CFunction)0x825741F0 },
    { "RenderNeParticle", (lua_CFunction)0x821DC8A0 },
    { "RenderNeParticlePost", (lua_CFunction)0x821DC8A0 },
    { "RenderGE1Particle", (lua_CFunction)0x82573D30 },
    { "RenderPostprocess", (lua_CFunction)0x82573E30 },
    { "SetCurrentScreen", (lua_CFunction)0x825742E8 },
    { "SetCurrentCamera", (lua_CFunction)0x825743F8 },
    { "RenderMovie", (lua_CFunction)0x82574490 },
    { "PrepareCalculateCSM", (lua_CFunction)0x82574688 },
    { "SetAutoZPass", (lua_CFunction)0x82576D88 },
    { "CommandBufferBegin", (lua_CFunction)0x82576EF8 },
    { "CommandBufferEnd", (lua_CFunction)0x8256D0C8 },
    { "CommandBufferSetFrameBuffer", (lua_CFunction)0x82574918 },
    { "CommandBufferSetConstantShader", (lua_CFunction)0x82574A58 },
    { "CommandBufferRenderWorld", (lua_CFunction)0x82574B80 },
    { "CommandBufferRun", (lua_CFunction)0x82576FF8 },
    { "SetShaderGPRAllocation", (lua_CFunction)0x82574C90 },
    { "IsDebug", (lua_CFunction)0x8235F338 },
    { "GetPlatform", (lua_CFunction)0x8243F0A8 },
    { "GetSurfaceWidth", (lua_CFunction)0x82574DD0 },
    { "GetSurfaceHeight", (lua_CFunction)0x82574E98 },
    { "GetFrameBufferFormat", (lua_CFunction)0x8256D140 },
    { "GetRegistry", (lua_CFunction)0x82574F60 },
    { "GetNumScreens", (lua_CFunction)0x8256C0C0 },
    { "ResetRenderStates", LUA_ResetRenderStates},
    { "LoadTexture", LUA_LoadTexture},
    { "Resolve", LUA_Resolve},
    { "GetDepthStencilFormat", LUA_GetDepthStencilFormat},
    { NULL, NULL }
};


int LUA_LoadObjectPhysics(lua_State* L)
{
    void* pGameImp = lua_touserdata(L, -10002);
    const char* str = lua_tostring(L, -1);
    if (pGameImp)
    {
        *(std::string*)((uint32_t)pGameImp + 0x1268 + 0xA8) = str;
    }
    return 0;
}

int LUA_LoadExplosion(lua_State* L)
{
    void* pGameImp = lua_touserdata(L, -10002);
    const char* str = lua_tostring(L, -1);
    if (pGameImp)
    {
        *(std::string*)((uint32_t)pGameImp + 0x1268 + 0x188) = str;
    }
    return 0;
}

int LUA_LoadEnemy(lua_State* L)
{
    void* pGameImp = lua_touserdata(L, -10002);
    const char* str = lua_tostring(L, -1);
    if (pGameImp)
    {
        *(std::string*)((uint32_t)pGameImp + 0x1268 + 0x1A4) = str;
    }
    return 0;
}

int LUA_LoadShot(lua_State* L)
{
    void* pGameImp = lua_touserdata(L, -10002);
    const char* str = lua_tostring(L, -1);
    if (pGameImp)
    {
        *(std::string*)((uint32_t)pGameImp + 0x1268 + 0x1C0) = str;
    }
    return 0;
}

static luaL_reg game_functions[] = {
    {"SetPlayer", (lua_CFunction)0x82405490},
    {"StartEntityByName", (lua_CFunction)0x82405D48},
    {"Signal", (lua_CFunction)0x82405E18},
    {"PlayBGM", (lua_CFunction)0x82404F20},
    {"StopBGM", (lua_CFunction)0x82404F78},
    {"SetGenArea", (lua_CFunction)0x82405598},
    {"LoadTerrain", (lua_CFunction)0x82404FA8},
    {"LoadSky", (lua_CFunction)0x82406758},
    {"LoadStageSet", (lua_CFunction)0x82405030},
    {"LoadPath", (lua_CFunction)0x82405090},
    {"LoadParticle", (lua_CFunction)0x824051F0},
    {"LoadSceneParams", (lua_CFunction)0x82406A88},
    {"LoadLight", (lua_CFunction)0x82406B18},
    {"LoadSceneEnvMap", (lua_CFunction)0x82406BA8},
    {"LoadReflectionAreas", (lua_CFunction)0x82406C38},
    {"LoadSoundBank", (lua_CFunction)0x82405258},
    {"LoadVoiceBank", (lua_CFunction)0x824052B8},
    {"LoadCameraParam", (lua_CFunction)0x824067F0},
    {"LoadRenderScript", (lua_CFunction)0x82406888},
    {"LoadKynapse", (lua_CFunction)0x82406918},
    {"LoadTownsmanInfo", (lua_CFunction)0x824069D0},
    {"LoadRaderMap", (lua_CFunction)0x82406CB8},
    {"DebugCommand", (lua_CFunction)0x82407248},
    {"LoadTimeLight", (lua_CFunction)0x824071B0},
    {"ProcessMessage", (lua_CFunction)0x82405318},
    {"InternalMessage", (lua_CFunction)0x82405358},
    {"SeparateRenderAndExecute", (lua_CFunction)0x82405398},
    {"SetAreaName", (lua_CFunction)0x82406D48},
    {"PlayerIndexToActorID", (lua_CFunction)0x821DC8A0},
    {"LoadPEBank", (lua_CFunction)0x821DC8A0},
    {"PreloadPlayer", (lua_CFunction)0x821DC8A0},
    {"LoadCameraLib", (lua_CFunction)0x821DC8A0},
    {"LoadCameraSet", (lua_CFunction)0x821DC8A0},
    {"LoadEntityLib", (lua_CFunction)0x821DC8A0},
    {"CreateCameraTornado", (lua_CFunction)0x821DC8A0},
    {"CreateObjInfo", (lua_CFunction)0x821DC8A0},
    {"NewActor", (lua_CFunction)0x821DC8A0},
    {"LoadEvent", (lua_CFunction)0x821DC8A0},
    {"ChangeArea", (lua_CFunction)0x821DC8A0},
    {"SetupWorld", (lua_CFunction)0x821DC8A0},
    {"DestroyWorld", (lua_CFunction)0x821DC8A0},
    {"StartPlaying", (lua_CFunction)0x821DC8A0},
    {"Retry", (lua_CFunction)0x821DC8A0},
    {"End", (lua_CFunction)0x821DC8A0},
    // Missing functions
    {"LoadObjectPhysics", LUA_LoadObjectPhysics},
    {"LoadExplosion", LUA_LoadExplosion},
    {"LoadEnemy", LUA_LoadEnemy},
    {"LoadShot", LUA_LoadShot},
    {NULL, NULL}
};

void SceneLODParam__LOAD(Sonicteam::SceneLODParam* LOD, Sonicteam::ISPApplySPArg* root)
{
    for (int i = 0; i < 19; i++)
    {
        LOD->m_LODParam[i].m_IsSet = 0;
        LOD->m_LODParam[i].m_ClipDistance = 0;
        LOD->m_LODParam[i].m_FarDistance = 0;
    }

    if (boost::shared_ptr<Sonicteam::LuaNodeImp> pLODTable = root->scene_file_lua->GetTable(LOD->GetParamName()))
    {
        struct MainREF
        {
            const char* name;
            uint32_t index;
        };
        MainREF refs[3] =
        {
            {"Main",4},
            {"Main",0x12},
            {"Reflection",0xA},
        };

        for (int i = 0; i < 3; i++)
        {
            const char* ref_name = refs[i].name;
            uint32_t ref_index = refs[i].index;
            if (boost::shared_ptr<Sonicteam::LuaNodeImp> pLODParamLUA = pLODTable->GetTable(ref_name))
            {
                Sonicteam::SceneLODParam::LODParam* pLODParam = &LOD->m_LODParam[ref_index];
                pLODParam->m_IsSet = 1;
                if (pLODParamLUA->IsNumber("FarDistance"))
                    pLODParam->m_FarDistance = pLODParamLUA->GetNumberFloat("FarDistance");
                else
                    pLODParam->m_FarDistance = -50000.0;

                pLODParam->m_ClipDistance = -1000000.0;

                if (pLODParamLUA->IsNumber("ClipDistance"))
                {
                    pLODParam->m_ClipDistance = pLODParamLUA->GetNumberFloat("ClipDistance");
                }

                if (pLODParamLUA->IsNumber("ClumpClipDistance"))
                {
                    pLODParam->m_ClipDistance = pLODParamLUA->GetNumberFloat("ClumpClipDistance");
                }
            }
        }
    }
}

void Patch::RestoreLUAFunctions::INSTALL()
{

    HookNew::WRITE_VALUE(0x8257726C, POWERPC_ADDIS(11, 0, POWERPC_HI(renderer_functions)));
    HookNew::WRITE_VALUE(0x82577274, POWERPC_ADDI(5, 11, POWERPC_LO(renderer_functions)));
    HookNew::WRITE_VALUE(0x82577270, POWERPC_ADDI(6, 0, sizeof(renderer_functions) / sizeof(luaL_reg)) - 1);

    HookNew::WRITE_VALUE(0x82404EE0, POWERPC_ADDIS(11, 0, POWERPC_HI(game_functions)));
    HookNew::WRITE_VALUE(0x82404EE8, POWERPC_ADDI(5, 11, POWERPC_LO(game_functions)));

 //   HookNew::WRITE_VALUE(0x821574D0, POWERPC_ADDIS(5, 0, POWERPC_HI(MainGlareStr)));
//    HookNew::WRITE_VALUE(0x821574E0, POWERPC_ADDI(20, 5, POWERPC_LO(MainGlareStr)));
	
    // Restore Retail LOD Param Loading
    HookNew::WRITE_VALUE(0x8203BFB8, SceneLODParam__LOAD);
}