#include "RestoreRetaiRenderFunctions.h"
#include <SceneLODParam.h>
#include <LuaSystem.h>
#include <Player/State/SonicContext.h>
#include <Player/Item.h>
#include <Player/State/Machine2.h>
#include <Player/State/SonicState.h>
#include <Player/Effect/ParticleJoint.h>
#include <Sox/Message.h>
#include <MyModelStrategyNN.h.h>
#include <Sox/Thread.h>
#include <MyModel.h>
#include <SoX/Input/Manager.h>
#include <Core2/Utility.h>
#include <Core2/Debug.h>
#include <Player/ModelSonic.h>
#include <Player/HairSimulator.h>
#include <Player/RootFrame.h>
#include <Player/PackageModel/IAnimation.h>
#include <Player/PackageModel/HairAnimation.h>
#include <Player/PackageModel/HairHierarchy.h>
#include <Player/Effect/ParticleLuaInfo.h>
#include <MainMenuTask.h>
#include <SoX/Audio/AudioPlayerXenon.h>
#include <SoX/Engine/Task.h>
#include <AudioRoomTask.h>
#include <SoX/Audio/PlayerImp.h>
#include <SoX/Audio/Cue.h>
#include <stdx/RTTI.h>
#include <Sox/LinkNode.h>
#include <Patch/Api/FStepableThread.h>
#include <Patch/Api/FFrameSynchronizerThread.h>

#include <xtl.h>
#include <Core2/Hook.h>
#include <string>
#include <algorithm>
#include <cmath>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

using namespace Patch::RestoreRetaiRenderFunctions;

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



HOOKV3(0x8250FCA0, int, sub_8250FCA0, (int, int, int, int, int, int, int, int), (a1, a2, a3, a4, a5, a6, a7, a8), int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
{
    auto lr = 0;
    AddMessage("%s - %d - sub_8250FCA0 CreateNode %s", __FILE__, __LINE__, (const char*)a4);
    return a1;
}


static CRITICAL_SECTION ms_CriticalSectionHandles;

static std::vector<HANDLE> ms_CloseThreads;
static std::vector<HANDLE> ms_CloseThreads1;
static std::vector<HANDLE> ms_CloseThreads2;
static std::vector<bool> ms_IsSynced;



void ThreadJoin(Sonicteam::SoX::Thread* result, char flag1)
{
    if (result->m_IsRunned)
    {
        result->m_IsRunned = 0;
        if (result->m_IsSuspended)
        {
            result->m_IsSuspended = 0;
            ResumeThread(result->m_Handle);
        }
        SetEvent(result->m_StepFrameEvent);
        WaitForSingleObject(result->m_Handle, -1);
        CloseHandle(result->m_Handle);
       // CloseHandle(result->m_ThreadHandle); WE DO NOT
       // result->m_ThreadHandle = INVALID_HANDLE_VALUE;
    }
}

void ThreadProperDestuction(Sonicteam::SoX::Thread* result, int a2)
{
    ThreadJoin(result, a2);

    if ((a2 & 1) != 0)
    {
        CloseHandle(result->m_StepFrameEvent);
        CloseHandle(result->m_StepFrameEventEnd);
        result->m_StepFrameEvent = INVALID_HANDLE_VALUE;
        result->m_StepFrameEventEnd = INVALID_HANDLE_VALUE;
    }
    else
    {
        HardwareBreakpoint(__FILE__, __LINE__, "sub_824F9CE8 Thread::Ctor, flag == %d", a2);
    }

}



// Retail DocMarathonState (Test)
void DocMarathonState__Update(Sonicteam::DocMarathonImp* a1, double a2)
{

    BranchTo(0x82558CD0, void, a1, a2);
}


// Thread Destructor
HOOKV3(0x824F9CE8, Sonicteam::SoX::Thread*, sub_824F9CE8, (Sonicteam::SoX::Thread*), (result), Sonicteam::SoX::Thread* result)
{
    ThreadJoin(result, 1);
    return result;
}

HOOKV3(0x824F9B10, int, sub_824F9B10, (Sonicteam::SoX::Thread*), (result), Sonicteam::SoX::Thread* result)
{
    AddMessage2(Logging::ms_LoggingThreadJoin, "%s - %d - sub_824F9B10 %x ThreadJoin Handle %x", __FILE__, __LINE__, result, result->m_Handle);
    ThreadJoin(result, 0);
    return 0;
}

// CloseHandle
HOOKV3(0x824A8930, int, sub_824A8930, (HANDLE), (a1), HANDLE a1)
{
    AddMessage2(Logging::ms_LoggingCloseHandle, "%s - %d - sub_824A8930 CloseHandle %x", __FILE__, __LINE__, a1);
    return CloseHandle(a1);
}

HOOKV3(0x824F9F50, int, sub_824F9F50, (Sonicteam::SoX::Thread*), (a1), Sonicteam::SoX::Thread* a1)
{
    WaitForSingleObject(a1->m_StepFrameEventEnd, -1);
    return (int)a1;
}

// CreateEvent
HOOKV3(0x824F9F50, HANDLE, sub_82626428, (LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCSTR), (lpEventAttributes, bManualReset, bInitialState, lpName),
    IN LPSECURITY_ATTRIBUTES lpEventAttributes,
    IN BOOL bManualReset,
    IN BOOL bInitialState,
    IN LPCSTR lpName)
{
    HANDLE eventHandle = CreateEvent(lpEventAttributes, bManualReset, bInitialState, lpName);
    AddMessage2(false, "%s - %d - sub_82626428 CreateEvent %x", __FILE__, __LINE__, eventHandle);
    return eventHandle;
}

// CreateThread
HOOKV3(0x826268E8, HANDLE, sub_826268E8, (LPSECURITY_ATTRIBUTES, DWORD, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD), (lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId), IN LPSECURITY_ATTRIBUTES lpThreadAttributes,
    IN DWORD dwStackSize,
    IN LPTHREAD_START_ROUTINE lpStartAddress,
    IN LPVOID lpParameter,
    IN DWORD dwCreationFlags,
    OUT LPDWORD lpThreadId)
{
    HANDLE THREAD = CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
    AddMessage2(Logging::ms_LoggingThreadConstructor, "%s - %d - sub_826268E8 CreateThread %x", __FILE__, __LINE__, THREAD);
    return THREAD;
}

HOOKV3(0x82506E68, Sonicteam::SoX::Graphics::Frame*, sub_82506E68,
    (Sonicteam::SoX::Graphics::Frame*, const char*), (frame, name), Sonicteam::SoX::Graphics::Frame* frame, const char* name)
{
    Sonicteam::SoX::Graphics::Frame* result = frame->FindNode(name);
    if (!result)
        AddMessage2(Logging::ms_LoggingFindGraphicsNode, "%s - %d - sub_82506E68 Failed to FindNode %s", __FILE__, __LINE__, name);

    return result;
}
// GameImp, Proper Cleanup, forgotten Actor
HOOKV3(0x82147718, int, sub_82147718,
    (int, int, int), (a1, a2, a3), int a1, int a2, int a3)
{

    if (*(int*)(a1 + 0x1158))
    {
        AddMessage2(Logging::ms_LoggingGameImpClearActors, "%s - %d - sub_82147718 %p", __FILE__, __LINE__, *(int*)(a1 + 0x1158));
      //  HookBranchTo(0x824F53A8, int, *(int*)(a1 + 0x1158));
    }
    return 0;
}

// This should recreate 3 tasks for restart
HOOKV3(0x8214C4D8, int, sub_8214C4D8,
    (int, int, int), (a1, a2, a3), int a1, int a2, int a3)
{
    int v2 = *(int*)(a1 + 0x1E0);

    /*
    *(int*)(a1 + 0x1E0) = 0;
    if (v2)
    {
        HardwareBreakpoint(__FILE__, __LINE__, "sub_8214C4D8 (0x1E0) %x",v2);
        BranchTo(0x825C0118, int, v2);
    }


    int v3 = *(int*)(a1 + 0x1DC);
    *(int*)(a1 + 0x1DC) = 0;
    if (v3)
    {
        HardwareBreakpoint(__FILE__, __LINE__, "sub_8214C4D8 (0x1DC) %x", v3);
        BranchTo(0x825C0118, int, v3);
    }
     */

    return 0;
}

HOOKV3(0x82155528, int, sub_82155528,
    (int, int, int), (a1, a2, a3), int a1, int a2, int a3)
{
    memset((void*)a1, 0, 0x15F0);
    *(int*)(a1 + 0x1E0) = 0;
    *(int*)(a1 + 0x1DC) = 0;
    return a1;
}

HOOKV3(0x8215AC18, int, sub_8215AC18,
    (int, int, int), (a1, a2, a3), int a1, int a2, int a3)
{
    Sonicteam::GameImp* pGame = (Sonicteam::GameImp*)a1;
 //   pGame->DocMarathon->__KernelUpdateRootGTask();

    AddMessage2(false, "%s - %d sub_8215AC18 GameImp Scene Switch %x,%x->%x", __FILE__, __LINE__, *(int*)(a1 + 4), *(int*)(a1 + 8), a2);
    int v4; // r29
    v4 = *(int*)(a1 + 4);
    *(int*)(a1 + 8) = a2;
    *(int*)(a1 + 4) = a2;
    switch (v4)
    {
    case 1:
        switch (a2)
        {
        case 2:
        case 3:
        case 6:
            goto LABEL_3;
        case 4:
            goto LABEL_12;

        case 0:

            if (*(int*)(a1 + 0x1158))
            {
                 // HookBranchTo(0x824F53A8, int, *(int*)(a1 + 0x1158));
            }

            goto LABEL_4;
        default:
            goto LABEL_4;
        }
    case 2:
        HookBranchTo(0x82156F28, int, a1);
        break;
    case 3:
        HookBranchTo(0x821570B8, int, a1);
        break;
    case 4:
        if (a2 >= 2 && (a2 <= 3 || a2 == 6))
        {
        LABEL_3:
            HookBranchTo(0x8214C4D8, int, a1);
            HookBranchTo(0x821566C0, int, a1);
        }
        else
        {
        LABEL_4:
            HookBranchTo(0x8214C4D8, int, a1);
        }
        break;
    case 6:
        HookBranchTo(0x82157178, int, a1);
        break;
    default:
        break;
    }
LABEL_12:

    bool _initialize_removed = false;
    switch (a2)
    {
    case 1:
        switch (v4)
        {
        case 0:
          //  BranchTo(0x82567EF8, int, operator new[](0x50), *(int*)(a1 + 0x1158), *(int*)(a1 + 0x1578));
         //   BranchTo(0x82404460, int, operator new[](0x54), *(int*)(a1 + 0x1158), (int*)(a1 + 0xFE4));
         //  BranchTo(0x823FAD48, int, operator new[](0x54), *(int*)(a1 + 0x1158), (int*)(a1 + 0xFDC));
        case 2:
        case 3:
        case 5:
        case 6:
            HookBranchTo(0x821587C8, int, a1);
            break;
        default:
            break;
        }
        HookBranchTo(0x82152560, int, a1);
        break;
    case 2:
        HookBranchTo(0x82157FC8, int, a1);
        break;
    case 3:
        HookBranchTo(0x82158438, int, a1);
        break;
    case 4:
        if (v4 >= 2 && v4 <= 3)
            HookBranchTo(0x821587C8, int, a1);
        HookBranchTo(0x8214ED28, int, a1);
        break;
    case 5:
        *(int*)(a1 + 0x10) &= ~0x400u;
        *(int*)(a1 + 0xF2C) = 0;
        break;
    case 6:
        HookBranchTo(0x82158538, int, a1);
        break;
    case 7:
    case 8:
        HookBranchTo(0x8214C4D8, int, a1);
        HookBranchTo(0x821566C0, int, a1);
        HookBranchTo(0x82149C58, int, a1);
        break;
    default:
        break;
    }


    return a1;
}

HOOKV3(0x821FC870, int, sub_821FC870,
    (Sonicteam::Player::ModelSonic*, int, int), (pModel, a2, a3),
    Sonicteam::Player::ModelSonic* pModel, int a2, int a3)
{
    // Save Particles From Previous Frame
    std::map<std::string, std::vector<Sonicteam::SoX::Graphics::FrameObserver*> > ParticleJointSave;
    std::vector<Sonicteam::Player::HairSimulator*> HairSimulatorSave;



    double currentKeyFrame = pModel->m_spPackageModel->m_PackageAnimationRoot->GetKeyFrame();
    int curretAnimationID = pModel->IModelGetAnimID1();

    FrameProcessor processor(ParticleJointSave, HairSimulatorSave, pModel);
    processor.Process(pModel->m_spRootFrame.get());

    pModel->m_spPackageShoesModel->m_pFrameGPObject->RemoveFromParent();
    pModel->m_spPackageModel->m_pFrameGPObject->RemoveFromParent();
    pModel->m_spFrameGPStandard->AddChild(pModel->m_spPackageShoesModel->m_pFrameGPObject);


    int result = HOOK_CALL_ORIGINAL(sub_821FC870, 0x821FC870, pModel, a2, a3);

    BranchTo(0x821F51F0, int, pModel->m_spPackageModel.get(), curretAnimationID);
    pModel->m_spPackageModel->m_PackageAnimationRoot->SetKeyFrame2(currentKeyFrame);

    processor.AttachParticles(pModel->m_spRootFrame.get());
    processor.ProcessHairAnimation(pModel->m_spRootFrame.get());

    /*
    Sonicteam::Player::Item* pItem = pModel->m_pPlayerLoad->



    for (std::vector<Sonicteam::Player::ItemPoint>::iterator& it  = pModel->m_spItem->m_pvItemPoints.begin();it != pModel->m_spItem->m_pvItemPoints.end();it++)
    {
        Sonicteam::Player::ItemPoint* pItem = &*it;
        pItem->m_spInstanceModel->m_spNode->RemoveFromParent();
    }
    */


    return result;
}


HOOKV3(0x82486A28, Sonicteam::MainMenuTask*, MainMenuTask__CTOR,
    (Sonicteam::MainMenuTask*, int), (pMenuTask, a2),
    Sonicteam::MainMenuTask* pMenuTask, int a2)
{
    pMenuTask->MMTSoundFlags = 0;
    pMenuTask->MMMyAudioADXCue = 0;
    return pMenuTask;
}

void MainMenuTask__StopSound(Sonicteam::MainMenuTask* pMainMenuTask)
{
    Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Audio::Cue>* pspCue = (Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Audio::Cue>*) & pMainMenuTask->MMMyAudioADXCue;
    pspCue->reset();
}


uint32_t MainMenuTask__DTOR(Sonicteam::MainMenuTask* pMenuTask, uint32_t flag)
{
    MainMenuTask__StopSound(pMenuTask);
    return BranchTo(0x82485C90, int, pMenuTask, flag);
}

void MainMenuTask__PlaySound(Sonicteam::MainMenuTask* pMainMenuTask, const char* bank, const char* cue)
{
    Sonicteam::SoX::Audio::Player* pAudioPlayer = pMainMenuTask->GetCurrentDoc()->DocGetAudioPlayerImp().get();
    uint32_t bankID = pAudioPlayer->GetSoundBankID(bank);
    uint32_t cueID = pAudioPlayer->GetSoundCueID(bankID, cue);

    Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Audio::Cue>* pspCue = (Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Audio::Cue>*) & pMainMenuTask->MMMyAudioADXCue;

    uint32_t prevBankID = -1;
    uint32_t prevCueID = -1;

    bool isNew = false;
    if (Sonicteam::SoX::Audio::Cue* cue = pspCue->get())
    {
        if (pMainMenuTask->MMTSoundFlags != cueID)
        {
            isNew = true;
            *pspCue = pAudioPlayer->Play(cueID);
        }

    }
    else
    {
        isNew = true;
        *pspCue = pAudioPlayer->Play(cueID);
    }
    pMainMenuTask->MMTSoundFlags = cueID;


    AddMessage2(isNew, "%s - %d MainMenuTask__PlaySound bank : [%s - %x], cue [%s - %x], previous bank [%x], cue [%x]",
        __FILE__, __LINE__, bank, bankID, cue, cueID, prevBankID, prevCueID);
}

uint32_t MainMenuTask__Update(Sonicteam::MainMenuTask* pMenuTask, double deltaTime)
{
    bool isStopSound = false;
    if (pMenuTask->m_pCurrentTask && stdx::ClassToName(pMenuTask->m_pCurrentTask) == "class Sonicteam::AudioRoomTask")
    {
        Sonicteam::AudioRoomTask* pAudioRoomTask = (Sonicteam::AudioRoomTask*)pMenuTask->m_pCurrentTask;
        if (pAudioRoomTask->m_State == 3)
        {
            isStopSound = true;
            MainMenuTask__StopSound(pMenuTask);
        }
        if (pAudioRoomTask->m_State == 2)
        {
            MainMenuTask__PlaySound(pMenuTask, "bgm", "mainmenu");
        }
    }


    switch (pMenuTask->MMCurrentCase)
    {
        //MainMenu-Init
    case 1:
        if (!isStopSound) MainMenuTask__PlaySound(pMenuTask, "bgm", "mainmenu");
    case 0x17:
        if (!isStopSound) MainMenuTask__PlaySound(pMenuTask, "bgm", "mainmenu");
        break;

        // Tag Story/ Trial, Constructor
    case 0x22:
        if (!isStopSound) MainMenuTask__PlaySound(pMenuTask, "bgm", "select");
        break;

        // Tag Story/ tRIAL, Exit
    case 0x1E:
        if (!isStopSound) MainMenuTask__PlaySound(pMenuTask, "bgm", "mainmenu");
        break;

    case 0x2D:
        if (!isStopSound) MainMenuTask__PlaySound(pMenuTask, "bgm", "select");
        break;
    }



    static uint32_t LastCase = pMenuTask->MMCurrentCase;
    if (pMenuTask->MMCurrentCase != LastCase)
    {
        LastCase = pMenuTask->MMCurrentCase;
        AddMessage2(false, "%s - %d MainMenuTask__Update MMCurrentCase %x", __FILE__, __LINE__, pMenuTask->MMCurrentCase);
    }


    return BranchTo(0x8248CD20, uint32_t, pMenuTask, deltaTime);
}

uint32_t MainMenuTask__ProcessMessage(Sonicteam::SoX::MessageReceiver* pTask, Sonicteam::SoX::IMessage* pMessage)
{

    Sonicteam::MainMenuTask* pMainMenuTask = static_cast<Sonicteam::MainMenuTask*>(pTask);

    Sonicteam::SoX::Audio::Player* pAudioPlayer = pMainMenuTask->GetCurrentDoc()->DocGetAudioPlayerImp().get();

    uint32_t systemBankID = pAudioPlayer->GetSoundBankID("system");
    uint32_t systemBankBGM = pAudioPlayer->GetSoundBankID("bgm");

    uint32_t mainDesideCueID = pAudioPlayer->GetSoundCueID(systemBankID, "main_deside");
    uint32_t cannotDesideCueID = pAudioPlayer->GetSoundCueID(systemBankID, "cannot_deside");
    uint32_t WindowCloseCueID = pAudioPlayer->GetSoundCueID(systemBankID, "window_close");
    uint32_t moveCueID = pAudioPlayer->GetSoundCueID(systemBankID, "move");

    uint32_t selectCueID = pAudioPlayer->GetSoundCueID(systemBankBGM, "select");



    // Message ID definitions based on decompiled code analysis
    struct Message0x10001 : public Sonicteam::SoX::Message<0x10001>
    {
        uint32_t buttonID;
    };
    struct Message0x10002 : Sonicteam::SoX::Message<0x10002>
    {
        uint32_t buttonID;
        bool Action;
    };
    struct Message0x1C002 : Sonicteam::SoX::Message<0x1C002> {};
    struct Message0x1C003 : Sonicteam::SoX::Message<0x1C003> {};
    struct Message0x1C005 : Sonicteam::SoX::Message<0x1C005> {};

    if (Message0x10001* msg = (Message0x10001*)Message0x10001::AsMessage(pMessage))
    {
        pAudioPlayer->Play(moveCueID);

    }

    if (Message0x10002* msg = (Message0x10002*)Message0x10002::AsMessage(pMessage))
    {
        if (msg->buttonID == 0x5A || msg->Action)
        {
            pAudioPlayer->Play(mainDesideCueID);
        }
        else
        {
            pAudioPlayer->Play(WindowCloseCueID);
        }
    }


    return (BranchTo(0x82483A48, int, pTask, pMessage));
}
static const char* MainGlareStr = "MainGlare";


// THREADS
// TODO, REDO ALL CODE OVER AGAIN, TO DESTRUCTION BE ON PROPER PLACE


void* DestroyObject(void* object, uint32_t flag)
{
    if ((flag & 1) != 0)
    {
        free(object);
    }
    return object;
}

uint32_t FrameSynchronizerThread__DTOR(Sonicteam::SoX::Thread* pThread, uint32_t flag)
{
    ThreadProperDestuction(pThread, flag);
    return BranchTo(0x824F9E00, uint32_t, pThread, flag);
}

void* HavokRunnerThread__DTOR(Sonicteam::SoX::Thread* pThread, uint32_t flag)
{
    Sonicteam::SoX::RefCountObject* v4 = *(Sonicteam::SoX::RefCountObject**)((uint32_t)pThread + 0x40);
    if (v4)
        v4->Release();

    ThreadProperDestuction(pThread, flag);
    BranchTo(0x824F9CE8, uint32_t, pThread, flag);
    return DestroyObject(pThread, flag);
}

void* FrustumCullingThread__DTOR(Sonicteam::SoX::Thread* pThread, uint32_t flag)
{

    std::vector<boost::shared_ptr<void*>>* v2 = (std::vector<boost::shared_ptr<void*>>*)((uint32_t)pThread + 0x40);
    v2->clear();
    v2->~vector();
    ThreadProperDestuction(pThread, flag);
    BranchTo(0x824F9CE8, uint32_t, pThread, flag);
    return DestroyObject(pThread, flag);
}

void* UpdateResourceInFrustumThread__DTOR(Sonicteam::SoX::Thread* pThread, uint32_t flag)
{
    std::vector<boost::shared_ptr<void*>>* v2 = (std::vector<boost::shared_ptr<void*>>*)((uint32_t)pThread + 0x40);
    v2->clear();
    v2->~vector();
    ThreadProperDestuction(pThread, flag);
    BranchTo(0x824F9CE8, uint32_t, pThread, flag);
    return DestroyObject(pThread, flag);
}

void* NoSyncThread__DTOR(Sonicteam::SoX::Thread* pThread, uint32_t flag)
{
    uint32_t v4 = (uint32_t)pThread + 0x40;
    int(__fastcall * v5)(DWORD, int) = *(int(__fastcall**)(DWORD, int))((uint32_t)pThread + 0x40);
    if (v5)
        *(DWORD*)((uint32_t)pThread + 0x44) = v5(*(DWORD*)((uint32_t)pThread + 0x44), 1);
    *(DWORD*)v4 = 0;
    *(DWORD*)(v4 + 8) = 0;

    ThreadProperDestuction(pThread, flag);
    BranchTo(0x824F9CE8, uint32_t, pThread, flag);
    return DestroyObject(pThread, flag);
}

uint32_t DocMarathonWorkerThread__DTOR(Sonicteam::SoX::Thread* pThread, uint32_t flag)
{
    ThreadProperDestuction(pThread, flag);
    return BranchTo(0x824F9E00, uint32_t, pThread, flag);
}

uint32_t Thread__DTOR(Sonicteam::SoX::Thread* pThread, uint32_t flag)
{
    ThreadProperDestuction(pThread, flag);
    return BranchTo(0x824F9E00, uint32_t, pThread, flag);
}

void StepableThread__OnStep(Sonicteam::SoX::StepableThread* pThread, double f1)
{
    if (pThread->func1.empty())
    {
        HardwareBreakpoint(__FILE__, __LINE__, "not should happend, sync error");
    }
    else

    {
        BranchTo(0x82582CF8, void, pThread, f1);
    }
  
}

HOOKV3(0x82582AE8, FStepableThread*, sub_82582AE8,
    (FStepableThread*, boost::function<void()>&, boost::function<void()>&, uint32_t), (pThread, OnStart, OnEnd, Proccessor),
    FStepableThread* pThread, boost::function<void()>& OnStart, boost::function<void()>& OnEnd, uint32_t Proccessor)
{
    HardwareBreakpoint(__FILE__, __LINE__, "LOL2");
    new (pThread) FStepableThread(OnStart, OnEnd, Proccessor);
    return pThread;
}

// Well Almost Works ;), better try just do same :0
HOOKV3(0x8257E638, void*, sub_8257E638,
    (void*, void*), (a1, a2),
    void* a1, void* a2)
{
    ((Sonicteam::SoX::Engine::Task*)a1)->~Task();
    return a1;
}

HOOKV3(0x8257EAB0, void*, sub_8257EAB0,
    (void*, void*,void*), (a1,a2,a3),
    void* a1,void* a2,void* a3)
{
    memset(a1, 0, 0x78);
    BranchTo(0x824F55A8, void, a1,a2);

    *(uint32_t*)a1 = 0x8203DC74;
    *(uint32_t*)((uint32_t)a1 + 0x20) = 0x8203DC68;

    *(uint32_t*)((uint32_t)a1 + 0x4C) = (uint32_t)a3;
    BranchTo(0x824F9DE8,void, (uint32_t)a1 + 0x50);

    *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x60) = new FFrameSynchronizerThread((Sonicteam::SoX::Graphics::FrameSynchronizer*)a3,0,1);
    *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x64) = new FFrameSynchronizerThread((Sonicteam::SoX::Graphics::FrameSynchronizer*)a3,1,1);
    *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x68) = new FFrameSynchronizerThread((Sonicteam::SoX::Graphics::FrameSynchronizer*)a3,2,1);
    *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x6C) = new FFrameSynchronizerThread((Sonicteam::SoX::Graphics::FrameSynchronizer*)a3,3,1);
    *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x70) = new FFrameSynchronizerThread((Sonicteam::SoX::Graphics::FrameSynchronizer*)a3,4,1);
    *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x74) = new FFrameSynchronizerThread((Sonicteam::SoX::Graphics::FrameSynchronizer*)a3,5,1);

    BranchTo(0x824FA1F8, void, (uint32_t)a1 + 0x50, *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x60));
    BranchTo(0x824FA1F8, void, (uint32_t)a1 + 0x50, *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x64));
    BranchTo(0x824FA1F8, void, (uint32_t)a1 + 0x50, *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x68));
    BranchTo(0x824FA1F8, void, (uint32_t)a1 + 0x50, *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x6C));
    BranchTo(0x824FA1F8, void, (uint32_t)a1 + 0x50, *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x70));
    BranchTo(0x824FA1F8, void, (uint32_t)a1 + 0x50, *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x74));
 
    return a1;
}


void __fastcall sub_82506F68X(INT a1, int a2, int a3)
{
    int v4; // r11
    unsigned int v5; // r10
    int v6; // r29
    void(__fastcall * v7)(INT, int); // r11
    INT v8; // r30
    char v9[96]; // [sp+50h] [-60h] BYREF


    v4 = *(int*)(a1 + 8);
    v5 = v4 & 0xFFFFFFFC;
    v6 = v4 | a2;
    *(int*)(a1 + 8) = v4 & 0xFFFFFFFC;
    if (((v4 | a2) & 2) != 0)
    {
        v7 = *(void(__fastcall**)(INT, int))(*(int*)a1 + 0x18);
        *(int*)(a1 + 8) = v5 | 8;
        v7(a1, a3);
    }
    (*(void(__fastcall**)(char*, INT, int))(*(int*)a1 + 0xC))(v9, a1, a3);
    if ((v6 & 1) != 0)
    {
        v8 = *(int*)(a1 + 0x1C);
        for (*(int*)(a1 + 8) |= 4u; v8; v8 = *(int*)(v8 + 0x14))
            sub_82506F68X((int)v8, (int)v6, (int)v9);
    }

}

// Temp, crash fix (not works, beause points to empty pointer, which means :\ syncronize issues :C )
HOOKV3(0x82506F68, void*, sub_82506F68,
    (void*, void*, void*), (a1, a2, a3),
    void* a1, void* a2, void* a3)
{
    if (a1)
    {
        sub_82506F68X((int)a1, (int)a2, (int)a3);
        return a1;
    }
      
}


void Patch::RestoreRetaiRenderFunctions::INSTALL()
{
    /*
    // Frame::FindNode(const char*)
    INSTALL_HOOKV3EX(sub_82506E68, -1, true, 9);



    // Fluid Shoes-Root Swap
    INSTALL_HOOKV3EX(sub_821FC870, -1, true, 9);

    // GameImp Restores Missing Actor after restart
  INSTALL_HOOKV3EX(sub_82147718, 1, false, 9);

  // GameImp Clear Missing Actor
  INSTALL_HOOKV3EX(sub_8215AC18, -1, true, 9);

    // GameImp Clear Missing sTUFF
  //  INSTALL_HOOKV3EX(sub_8214C4D8, -1, false, 9);

    // GameImp CTOR
   // INSTALL_HOOKV3EX(sub_82155528, -1, false, 9);

    INSTALL_HOOKV3EX(SonicContext__CTOR, 1, false, 9);
    HookNew::WRITE_VALUE(0x8200A3C4, SonicContext__DTOR);
    HookNew::WRITE_VALUE(0x8200A3D0, SonicContext__IsCanDrain);
    HookNew::WRITE_VALUE(0x8200A3CC, SonicContext__Update);

    HookNew::WRITE_VALUE(0x8202D4C8, MainMenuTask__ProcessMessage);
    HookNew::WRITE_VALUE(0x8202D4D4, MainMenuTask__DTOR);
    HookNew::WRITE_VALUE(0x8202D4D8, MainMenuTask__Update);
    INSTALL_HOOKV3EX(MainMenuTask__CTOR, 1, true, 9);

    HookNew::WRITE_VALUE(0x8257726C, POWERPC_ADDIS(11, 0, POWERPC_HI(renderer_functions)));
    HookNew::WRITE_VALUE(0x82577274, POWERPC_ADDI(5, 11, POWERPC_LO(renderer_functions)));
    HookNew::WRITE_VALUE(0x82577270, POWERPC_ADDI(6, 0, sizeof(renderer_functions) / sizeof(luaL_reg)) - 1);

    HookNew::WRITE_VALUE(0x82404EE0, POWERPC_ADDIS(11, 0, POWERPC_HI(game_functions)));
    HookNew::WRITE_VALUE(0x82404EE8, POWERPC_ADDI(5, 11, POWERPC_LO(game_functions)));

    HookNew::WRITE_VALUE(0x821574D0, POWERPC_ADDIS(5, 0, POWERPC_HI(MainGlareStr)));
    HookNew::WRITE_VALUE(0x821574E0, POWERPC_ADDI(20, 5, POWERPC_LO(MainGlareStr)));


    // Restore Retail LOD Param Loading
    HookNew::WRITE_VALUE(0x8203BFB8, SceneLODParam__LOAD);
    HookNew::WRITE_VALUE(0x82186A04, POWERPC_ADDIS(3, 0, sizeof(NewSonicContext)));




    INSTALL_HOOKV3EX(sub_824F9CE8, -1, false, 5);
    INSTALL_HOOKV3EX(sub_824F9B10, -1, true, 5);
    INSTALL_HOOKV3EX(sub_824A8930, -1, true, 4);


    INSTALL_HOOKV3EX(sub_8257E638, -1, true, 5);


  //  HookNew::WRITE_VALUE(0x824F9D50, 0x60000000);
 //   HookNew::WRITE_VALUE(0x824F9D58, 0x60000000);


    //HookNew::WRITE_VALUE(0x823357C4, 0x60000000); // Let this code, JoinThread leave to Thread Desturctor


    // INSTALL_HOOKV3EX(sub_826268E8, -1, true, 11);

     //End Event ( Fix, Test)
  //   INSTALL_HOOKV3EX(sub_824F9F50, -1, true, 11);
//     INSTALL_HOOKV3EX(sub_82626428, -1, true, 11);

    InitializeCriticalSection(&ms_CriticalSectionHandles);

    // THREADS Dtors

/*
   HookNew::WRITE_VALUE(0x8203DB04, FrameSynchronizerThread__DTOR);
   HookNew::WRITE_VALUE(0x8203C288, HavokRunnerThread__DTOR);
   HookNew::WRITE_VALUE(0x8203DCA0, FrustumCullingThread__DTOR);
   HookNew::WRITE_VALUE(0x8203DCE8, UpdateResourceInFrustumThread__DTOR);
   HookNew::WRITE_VALUE(0x8203DED0, NoSyncThread__DTOR);

   HookNew::WRITE_VALUE(0x8203DBA8, DocMarathonWorkerThread__DTOR);
   HookNew::WRITE_VALUE(0x82038044, Thread__DTOR);
   */
 //  HookNew::WRITE_VALUE(0x8203DEF4, StepableThread__DTOR); //( Crash , why?)


    // HookNew::WRITE_VALUE(0X824F9CCC, 0x60000000);

  //   HookNew::WRITE_VALUE(0x82000B60, DocMarathonState__Update);
 //    HookNew::WRITE_VALUE(0x8203DF00, StepableThread__OnStep);

  //  INSTALL_HOOKV3EX(sub_8257EAB0,-1,true,9);
  //  INSTALL_HOOKV3EX(sub_82582AE8,-1,true,9);
 //   HookNew::WRITE_VALUE(0x821FB0A8,POWERPC_ADDIS(3, 0, sizeof(FStepableThread)));
 //   HookNew::WRITE_VALUE(0x821FE1D0,POWERPC_ADDIS(3, 0, sizeof(FStepableThread)));
  //  HookNew::WRITE_VALUE(0x822B1D0C,POWERPC_ADDIS(3, 0, sizeof(FStepableThread)));
//    HookNew::WRITE_VALUE(0x823BF7C4,POWERPC_ADDIS(3, 0, sizeof(FStepableThread)));



   // new Sonicteam::SoX::Engine::Task(0);
 //   HookNew::WRITE_VALUE(0x82507BCC, POWERPC_ADDIS(11, 0, POWERPC_HI(SyncronzinzeThreadMain)));
//    HookNew::WRITE_VALUE(0x82507BD4, POWERPC_ORI(11, 11, POWERPC_LO(SyncronzinzeThreadMain)));

  //  HookNew::WRITE_VALUE(0x8203DC7C, sub_8203DC7C);

    */
}