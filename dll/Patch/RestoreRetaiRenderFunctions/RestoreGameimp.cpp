#include "RestoreGameimp.h"

#include <Core2/Hook.h>
#include <MainMenuTask.h>
#include <AudioRoomTask.h>
#include <SoX/Audio/AudioPlayerXenon.h>
#include <SoX/Audio/PlayerImp.h>
#include <SoX/Audio/Cue.h>
#include <SoX/Engine/Task.h>
#include <UpdateResourceInFrustumTask.h>
#include <GameImp.h>

using namespace Patch::RestoreGameimp;


// GameImp, Proper Cleanup, forgotten Actor
HOOKV3(0x82147718, int, sub_82147718,
    (Sonicteam::GameImp*, int, int), (a1, NextState, a3), Sonicteam::GameImp* a1, int NextState, int a3)
{

    if (a1->m_pDependGame)
    {
        AddMessage2(Logging::ms_LoggingGameImpClearActors, "%s - %d - sub_82147718 %p", __FILE__, __LINE__, a1->m_pDependGame);
        HookBranchTo(0x824F53A8, int, a1->m_pDependGame);
    }
    return 0;
}

Sonicteam::GameImp* sub_821566C0(Sonicteam::GameImp* a1, int a2)
{
    AddMessage2(false, "%s - %d sub_821566C0 New Argument %x", __FILE__, __LINE__, a2);

    BranchTo(0x821566C0, int, a1);

    if (a1->m_IsStageLoaded)
    {
        if (a2)
        {
            Sonicteam::SoX::Thread* pHavokThread = *(Sonicteam::SoX::Thread**)(a1 + 0x1264);
            *(Sonicteam::SoX::Thread**)(a1 + 0x1264) = 0;
            if (pHavokThread)
                pHavokThread->DestroyObject(1);

            // Not RefCount but since Destroy at 0 we work with it
            Sonicteam::SoX::RefCountObject* pPhysicsMaterialMgr = *(Sonicteam::SoX::RefCountObject**)(a1 + 0x1578);
            *(uint32_t*)(a1 + 0x1578) = 0;
            if (pPhysicsMaterialMgr)
                pPhysicsMaterialMgr->DestroyObject(1);

            Sonicteam::SoX::RefCountObject* pWorldHavok = *(Sonicteam::SoX::RefCountObject**)(a1 + 0x15E0);
            *(Sonicteam::SoX::RefCountObject**)(a1 + 0x15E0) = 0;
            if (pWorldHavok)
                pWorldHavok->Release();
        }
        a1->m_IsStageLoaded = 0;
    }
    return a1;
}




//a1->m_pDoc->DocFrameSynchronizer.m_aollFrame Cleared After Exit Stage For Sure
HOOKV3(0x8215AC18, Sonicteam::GameImp*, sub_8215AC18,
    (Sonicteam::GameImp*, uint32_t), (a1, NextState), Sonicteam::GameImp* a1, uint32_t NextState)
{
    uint32_t oldState = a1->m_PostState;

    a1->m_PostState = NextState;
    a1->m_CurrentState = NextState;

    //ADDITIONAL DEBUGGING

    AddMessage2(false, " a1->m_PostState = %d, a1->m_CurrentState = %d", oldState, NextState);
    for (int i = 0; i < a1->m_pDoc->DocFrameSynchronizer.m_Count; i++)
    {

        // Force Clear
        // Hack, honestly i should not do this, because issues still presents
        // ISSUE #1 Some Frame(FrameGP,... Node) is corrupted which is List still points to released Frame (and cause of it ThreadSyncronizer and next iterrator code, could just crash)
        if (a1->m_pDoc->DocFrameSynchronizer.m_aollFrame[i].begin() != a1->m_pDoc->DocFrameSynchronizer.m_aollFrame[i].end())
        {
           // a1->m_pDoc->DocFrameSynchronizer.m_aollFrame[i].ResetQuick();
           // a1->m_pDoc->DocFrameSynchronizer.m_aollFrame[i].InitializeList();
        }

       // a1->m_pDoc->DocFrameSynchronizer.m_aollFrame[i].ResetQuick();
       // a1->m_pDoc->DocFrameSynchronizer.m_aollFrame[i].InitializeList();
        
        // IT SHOULD BE EMPTY
        for (Sonicteam::SoX::LinkedList<Sonicteam::SoX::Graphics::Frame>::iterator it = a1->m_pDoc->DocFrameSynchronizer.m_aollFrame[i].begin(); it != a1->m_pDoc->DocFrameSynchronizer.m_aollFrame[i].end();)
        { 
            Sonicteam::SoX::Graphics::Frame* pFrame = (*it);
            HardwareBreakpoint(__FILE__, __LINE__, "(NOT SHOULD HAPPEND) : a1->m_pDoc->DocFrameSynchronizer.m_aollFrame[%d]->%x (%s)", i, pFrame, GetClassName(pFrame));
            ++it;
            AddMessage2(false, "a1->m_pDoc->DocFrameSynchronizer.m_aollFrame[%d]->%x (%s)", i, pFrame, GetClassName(pFrame));
        }
    }

    // First switch - based on old state
    switch (oldState)
    {
    case Sonicteam::GameImp::GameState_Stage:  // 1
        switch (NextState)
        {
        case Sonicteam::GameImp::GameState_Event:   // 2
        case Sonicteam::GameImp::GameState_Movie:   // 3
        case Sonicteam::GameImp::GameState_6:       // 6
            goto LABEL_3;
        case Sonicteam::GameImp::GameState_Result:  // 4
            goto LABEL_12;
        default:
            goto LABEL_4;
        }
        break;

    case Sonicteam::GameImp::GameState_Event:  // 2
        BranchTo(0x82156F28, void, a1);
        break;

    case Sonicteam::GameImp::GameState_Movie:  // 3
        BranchTo(0x821570B8, void, a1);
        break;

    case Sonicteam::GameImp::GameState_Result:  // 4
        if (NextState >= 2 && (NextState <= 3 || NextState == 6))
        {
        LABEL_3:
            BranchTo(0x8214C4D8, void, a1);
            BranchTo(0x821566C0, void, a1);
        }
        else
        {
        LABEL_4:          
            BranchTo(0x8214C4D8, void, a1);
        }
        break;

    case Sonicteam::GameImp::GameState_6:  // 6
        BranchTo(0x82157178, void, a1);
        break;

    default:
        break;
    }

LABEL_12:
    // Second switch - based on new state
    switch (NextState)
    {
    case Sonicteam::GameImp::GameState_Stage:  // 1
        switch (oldState)
        {
        case 0:  // MainMenu
        case 2:  // Event
        case 3:  // Movie
        case 5:  // Message
        case 6:  // GameState_6
            if (a1->m_IsStageLoaded)
            {
                if (!a1->m_pDependGame->Find("PhysicsMaterialMgrRunner"))
                {
                    BranchTo(0x82567EF8, void, ::operator new(0x50), a1->m_pDependGame, a1->m_pPhysicsMaterialMgr);
                }
                if (!a1->m_pDependGame->Find("StageSetManagerRunner"))
                {
                    BranchTo(0x82404460, void, ::operator new(0x54), a1->m_pDependGame, &a1->m_spPropManager);
                }
                if (!a1->m_pDependGame->Find("ActivationMgrRunner"))
                {
                    BranchTo(0x823FAD48, void, ::operator new(0x54), a1->m_pDependGame, &a1->m_spActivationMgr);
                }
            }
            BranchTo(0x821587C8, void, a1);
            break;
        default:
            break;
        }
        BranchTo(0x82152560, void, a1);
        break;

    case Sonicteam::GameImp::GameState_Event:  // 2
        BranchTo(0x82157FC8, void, a1);
        break;

    case Sonicteam::GameImp::GameState_Movie:  // 3
        BranchTo(0x82158438, void, a1);
        break;

    case Sonicteam::GameImp::GameState_Result:  // 4
        if (oldState >= 2 && oldState <= 3)
            BranchTo(0x821587C8, void, a1);
        BranchTo(0x8214ED28, void, a1);
        break;

    case Sonicteam::GameImp::GameState_Message:  // 5
        a1->m_Flags &= ~0x400u;
        a1->m_FieldF2C = 0;
        break;

    case Sonicteam::GameImp::GameState_6:  // 6
        BranchTo(0x82158538, void, a1);
        break;

    case Sonicteam::GameImp::GameState_Save:        // 7
    case Sonicteam::GameImp::GameState_ReturnToMainMenu:  // 8
        BranchTo(0x8214C4D8, void, a1);
        BranchTo(0x821566C0, void, a1);
        BranchTo(0x82149C58, void, a1);
        break;

    default:
        break;
    }

    //HardwareBreakpoint(__FILE__, __LINE__, "After GameImp::OnStateChange(%x)",NextState);
    return a1;
}

void __fastcall GameProcessGlobalEventActions(Sonicteam::GameImp* result, double a2)
{
    int currentState; // r4

    if ((result->m_Flags & 0x4000u) != 0)
    {
        result->m_Flags &= ~0x4000u;
        BranchTo(0x8215AC18, int, result, 0); //         EngineEventsActions((int)result, 0);
        sub_821566C0(result, 1); //BranchTo(0x821566C0,int,result,1);
        BranchTo(0x82152810, int, result);
        BranchTo(0x8214F2B8, int, result);
    }
    if ((result->m_Flags & 0x800u) != 0)
    {
        result->m_Flags &= ~0x800u;
        BranchTo(0x8215AC18, int, result, 0); //         EngineEventsActions((int)result, 0);
        sub_821566C0(result, 0); //BranchTo(0x821566C0, int, result, 0);
        BranchTo(0x8214F2B8, int, result);
    }
    currentState = result->m_CurrentState;
    if (result->m_PostState != currentState)
        BranchTo(0x8215AC18, int, result, currentState);
    switch (result->m_PostState)
    {
    case Sonicteam::GameImp::GameState_Stage:
        BranchTo(0x8215B4A8, int, result, a2);
        break;
    case Sonicteam::GameImp::GameState_Event:
        BranchTo(0x8215AFD8, int, result, a2);
        break;
    case Sonicteam::GameImp::GameState_Movie:
        BranchTo(0x8215AE48, int, (int)result);
        break;
    case Sonicteam::GameImp::GameState_Result:
        BranchTo(0x8214FF98, int, (int)result);
        break;
    case Sonicteam::GameImp::GameState_Message:
        BranchTo(0x8214A7C0, int, result, a2);
        break;
    case Sonicteam::GameImp::GameState_6:
        BranchTo(0x82150038, int, result, a2);
        break;
    default:
        return;
    }
}


// CHECK THAT ALL THINGS THAT ADD FRAME DOESNT ADD ADITIONAL COUNTER BEFORE
void AddFrame(Sonicteam::DocMarathonImp* _this,Sonicteam::SoX::Graphics::Frame** frame)
{
    //HardwareBreakpoint(__FILE__, __LINE__, "AddFrame %x", frame);
    Sonicteam::SoX::Graphics::FrameSynchronizer* pSyncronizer = &_this->DocFrameSynchronizer;
    pSyncronizer->m_llFrames.Insert(&(*frame)->m_lnFrameSync);
}

void FrameSynchronizerTaskUpdate(uint32_t _this, double delta)
{
    Sonicteam::SoX::Graphics::FrameSynchronizer* pSyncronizer = *(Sonicteam::SoX::Graphics::FrameSynchronizer**)(_this + 0x4C);
   
    // Frame Update
    for (Sonicteam::SoX::LinkedList<Sonicteam::SoX::Graphics::Frame>::iterator it = pSyncronizer->m_llFrames.begin(); it != pSyncronizer->m_llFrames.end(); it++)
    {
        // Frame, Flag, Transform(Default)
        BranchTo(0x82506F68, void, *it,0,0x82D26980);
    }

    // Observer Update
    for (Sonicteam::SoX::LinkedList<Sonicteam::SoX::Graphics::Frame>::iterator it = pSyncronizer->m_llFrames.begin(); it != pSyncronizer->m_llFrames.end(); it++)
    {
        BranchTo(0x82507C10, void, *it, 0);
    }
}

void UpdateResourceInFrustumTaskUpdate(Sonicteam::UpdateResourceInFrustumTask* _this, double delta)
{
    for (int i = 0; i < 0xD; i++)
    {
        boost::shared_ptr<Sonicteam::SoX::Scenery::World> spWorld = _this->m_pDoc->DocGetWorld(i);
        if (spWorld.get())
            spWorld->WorldProcessUnk13();
    }
}

static CRITICAL_SECTION ms_GetWorldCritical;
static DWORD s_MainThreadId = GetCurrentThreadId();

boost::shared_ptr<Sonicteam::SoX::Scenery::World> DocGetWorld(Sonicteam::DocMarathonImp* _this, uint32_t id)
{
    if (GetCurrentThreadId() != s_MainThreadId)
    {
        HardwareBreakpoint(__FILE__, __LINE__, "::GetWorld(uin32_t) called from %x", GetCurrentThreadId());
    }

    EnterCriticalSection(&ms_GetWorldCritical);
    boost::shared_ptr<Sonicteam::SoX::Scenery::World> result = (*_this->WorldCollection)[id].lock();
    LeaveCriticalSection(&ms_GetWorldCritical);
    return result;
}

// Hook boost::detail::sp_counted_base::add_ref
HOOKV3(0x82140140, void*, sub_82140140,
    (void*), (this_ptr), void* this_ptr)
{
    InterlockedIncrement((volatile LONG*)((char*)this_ptr + 4)); // use_count
    InterlockedIncrement((volatile LONG*)((char*)this_ptr + 8)); // weak_count_
    return 0;
}

// Hook boost::detail::sp_counted_base::release
HOOKV3(0x821401B8, void*, sub_821401B8,
    (void*), (this_ptr), void* this_ptr)
{
    LONG use_count = InterlockedDecrement((volatile LONG*)((char*)this_ptr + 4)); // use_count_ at +4

    if (use_count == 0)
    {
        // Call dispose from vtable
        void** vtable = *(void***)this_ptr;
        void* dispose_func = vtable[1]; // dispose at vtable offset 4 (index 1)
        ((void(__fastcall*)(void*))dispose_func)(this_ptr);

        // Decrement weak_count_
        LONG weak_count = InterlockedDecrement((volatile LONG*)((char*)this_ptr + 8)); // weak_count_ at +8

        if (weak_count == 0)
        {
            // Call destruct from vtable
            void* destruct_func = vtable[2]; // destruct at vtable offset 8 (index 2)
            ((void(__fastcall*)(void*))destruct_func)(this_ptr);
        }
    }
    else
    {
        // Just decrement weak_count_
        InterlockedDecrement((volatile LONG*)((char*)this_ptr + 8)); // weak_count_ at +8
    }
    return 0;
}

// Hook boost::detail::shared_count::shared_count
HOOKV3(0x82595380, void*, sub_82595380,
    (void*, void*), (a1, a2), void* a1, void* a2)
{
    uint32_t* this_ptr = (uint32_t*)a1;
    uint32_t* other = (uint32_t*)a2;

    void* sp_counted = (void*)other[1];
    this_ptr[1] = (uint32_t)sp_counted;

    if (sp_counted)
    {
        // This will call your hooked add_ref
        sub_82140140(sp_counted);
    }
    else
    {
        // Call original for error handling
        return (void*)sub_82595380(a1, a2);
    }

    this_ptr[0] = other[0];
    return a1;
}


// Hook RefCountObject::AddReference()
HOOKV3(0x824F5B30, void*, sub_824F5B30,
    (void*), (_this), void* _this)
{
    InterlockedIncrement((volatile LONG*)((char*)_this + 4));
    return 0;
}


// Hook RefCountObject::AddReference()
HOOKV3(0x825C0118, void*, sub_825C0118,
    (Sonicteam::SoX::RefCountObject*), (_this), Sonicteam::SoX::RefCountObject* _this)
{
    if (InterlockedDecrement((volatile LONG*)((char*)_this + 4)) == 0)
        _this->DestroyObject(1);

    return 0;
}



// CSDLink::Function4
int __fastcall sub_825809E0(int result)
{

    EnterCriticalSection((CRITICAL_SECTION*)(*(char**)(0x82D26D80) + 0x40));
    if ((*(uint32_t*)(result + 0x18) & 0x418) == 0)
        BranchTo(0x8253A650, int, *(uint32_t*)(result + 0x1C));

    LeaveCriticalSection((CRITICAL_SECTION*)(*(char**)(0x82D26D80) + 0x40));
    return result;
}

// CSDLink::Function8
int __fastcall sub_825809C0(int result)
{
    EnterCriticalSection((CRITICAL_SECTION*)(*(char**)(0x82D26D80) + 0x40));
    if ((*(uint32_t*)(result + 0x18) & 0x118) == 0)
        BranchTo(0x8253C770, int, *(uint32_t*)(result + 0x1C));

    LeaveCriticalSection((CRITICAL_SECTION*)(*(char**)(0x82D26D80) + 0x40));
    return result;
}

void Patch::RestoreGameimp::INSTALL()
{
    /// Syncronization Sheni
    InitializeCriticalSection(&ms_GetWorldCritical);
    s_MainThreadId = GetCurrentThreadId();
   
    // Disable Frame Syncronizer at all
    HookNew::WRITE_VALUE(0x82000B8C, AddFrame);
    HookNew::WRITE_VALUE(0x8203DC7C, FrameSynchronizerTaskUpdate);
  
    // No Frustrum Threads
    HookNew::WRITE_VALUE(0x8257F304, 0x39200000);
    HookNew::WRITE_VALUE(0x8257F340, 0x48000088);

    // No Frustrum Update Threads
    HookNew::WRITE_VALUE(0x8257F4D4, 0x39200000);
    HookNew::WRITE_VALUE(0x8257F510, 0x48000088); // Need Custom Code 

    HookNew::WRITE_VALUE(0x8203DD30, UpdateResourceInFrustumTaskUpdate);
   
    //HookNew::WRITE_VALUE(0x82000B74, DocGetWorld); 

    // Restore Boost Synchronization
    INSTALL_HOOKV3EX(sub_82140140, -1, true, 9);
    INSTALL_HOOKV3EX(sub_821401B8, -1, true, 9);

    // Restore RefCountObject Synchronization
    //INSTALL_HOOKV3EX(sub_824F5B30, -1, true, 9);
    //INSTALL_HOOKV3EX(sub_825C0118, -1, true, 9);

    // Syncronize CSDLink Methods (not like this, its doesnt work properly)
    //HookNew::WRITE_VALUE(0x8203DE38, sub_825809E0);
    //HookNew::WRITE_VALUE(0x8203DE3C, sub_825809C0);

    // END OF SHENI
   
    
    //INSTALL_HOOKV3EX(sub_82595380, -1, true, 9);


    // GameImp Restores Missing Actor after restart
    INSTALL_HOOKV3EX(sub_82147718, 1, false, 9);
    INSTALL_HOOKV3EX(sub_8215AC18, -1, true, 9);
    /*
    // GameImp Clear Missing Actor
    INSTALL_HOOKV3EX(sub_8215AC18, -1, true, 9);

    // Restore Retail Argument
    //INSTALL_HOOKV3EX(sub_821566C0, -1, true, 9);
    HookNew::WRITE_VALUE(0x82156EE4, 0x60000000);
    HookNew::WRITE_VALUE(0x82156EE8, 0x60000000);
    HookNew::WRITE_VALUE(0x82156EF0, 0x60000000);
    HookNew::WRITE_VALUE(0x82156EF4, 0x60000000);
    HookNew::WRITE_VALUE(0x82156EF8, 0x60000000);
    HookNew::WRITE_VALUE(0x82156EFC, 0x60000000);
    HookNew::WRITE_VALUE(0x82156F00, 0x60000000);
    HookNew::WRITE_VALUE(0x82156F04, 0x60000000);
    HookNew::WRITE_VALUE(0x82156F08, 0x60000000);
    HookNew::WRITE_VALUE(0x82156F0C, 0x60000000);
    HookNew::WRITE_VALUE(0x82156F10, 0x60000000);
    HookNew::WRITE_VALUE(0x82156F14, 0x60000000);
    HookNew::WRITE_VALUE(0x82156F18, 0x60000000);
    HookNew::WRITE_VALUE(0x82156F1C, 0x60000000);
    HookNew::WRITE_VALUE(0x8200165C, GameProcessGlobalEventActions);

    // GameImp Clear Missing sTUFF
  //  INSTALL_HOOKV3EX(sub_8214C4D8, -1, false, 9);

    // GameImp CTOR
   // INSTALL_HOOKV3EX(sub_82155528, -1, false, 9);
   */
}