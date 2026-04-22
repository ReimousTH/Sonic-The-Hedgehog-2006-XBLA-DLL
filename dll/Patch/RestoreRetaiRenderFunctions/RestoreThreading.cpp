#include "RestoreThreading.h"

#include <Core2/Debug.h>
#include <Core2/Hook.h>

#include <Sox/Thread.h>
#include <Sox/Engine/Task.h>
#include <Patch/Api/FFrameSynchronizerThread.h>
#include <Patch/Api/FStepableThread.h>
#include <Patch/Api/FHavokRunnerThread.h>

using namespace Patch::RestoreThreading;

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
    }
}

// Thread Destructor
HOOKV3(0x824F9CE8, Sonicteam::SoX::Thread*, sub_824F9CE8, (Sonicteam::SoX::Thread*), (result), Sonicteam::SoX::Thread* result)
{
    ThreadJoin(result, 1);
    CloseHandle(result->m_StepFrameEvent);
    CloseHandle(result->m_StepFrameEventEnd);
    if (result->m_ParentList && result->m_lnThread.m_pNext)
    {
        result->m_lnThread.ResetFast();
    }

    result->m_lnThread.ResetQuick();
    return result;
}

HOOKV3(0x824F9B10, int, sub_824F9B10, (Sonicteam::SoX::Thread*), (result), Sonicteam::SoX::Thread* result)
{
    AddMessage2(Logging::ms_LoggingThreadJoin, "%s - %d - sub_824F9B10 %x ThreadJoin Handle %x", __FILE__, __LINE__, result, result->m_Handle);
    ThreadJoin(result, 0);
    return 0;
}

HOOKV3(0x82582C30, void*, sub_82582C30, (FStepableThread*,int), (result,a2), FStepableThread* result,int a2)
{
    AddMessage2(false, "%s - %d - sub_82582C30 %x StepableThread::Dtor Handle %x", __FILE__, __LINE__, result, result->m_Handle);
    BranchTo(0X82582BA8, int, result); // Natural DTOR

    CloseHandle(result->m_Handle);
    result->m_Handle = INVALID_HANDLE_VALUE;

    if ((a2 & 1) != 0)
    {
        free(result);
        return result;
    }

    return result;
}

CRITICAL_SECTION ms_test;
CRITICAL_SECTION ms_test2;

// Well Almost Works ;), better try just do same :0
HOOKV3(0x8257E638, void*, sub_8257E638,
    (void*, void*), (a1, a2),
    void* a1, void* a2)
{
    EnterCriticalSection(&ms_test);
    AddMessage2(false, "%s - %d sub_8257E638 = Task::DestroyObject(%x,%d), class %s", __FILE__, __LINE__,a1,a2,GetClassName(a1));
    ((Sonicteam::SoX::Engine::Task*)a1)->~Task();
    LeaveCriticalSection(&ms_test);
    return a1;
}

HOOKV3(0x825070E8, void*, sub_825070E8,
    (void*, void*), (a1, a2),
    void* a1, void* a2)
{
    EnterCriticalSection(&ms_test);
    AddMessage2(false, "%s - %d sub_825070E8 = Frame::UpdateFrame() %x class %s, Thread %d", __FILE__, __LINE__,a1,GetClassName(a1), GetCurrentThreadId());

    int v1; // r10
    int v2; // r11
    int* v3; // r10
    int result = (int)a1;
    v1 = *(int*)(result + 0xC);
    if (!((*(int*)(v1 + 0x44))))
    {
        v2 = *(int*)(v1 + 0x2C);
        if (v2)
        {
            v3 = (int*)(v1 + 0x40);
            *v3 = v2;
            v3[1] = *(int*)(v2 + 4);
            **(int**)(v2 + 4) = (int)v3;
            *(int*)(v2 + 4) = (int)v3;
        }
    }
    *(int*)(*(int*)(result + 0xC) + 8) |= 1u;
    *(int*)(result + 8) |= 2u;


    LeaveCriticalSection(&ms_test);
    return a1;
}

// Well Almost Works ;), better try just do same :0
HOOKV3(0x825074E0, void*, sub_825074E0,
    (void*, void*), (a1, a2),
    void* a1, void* a2)
{
    EnterCriticalSection(&ms_test2);
    AddMessage2(false, "%s - %d 0x825074E0 = Frame::DestroyFromParent(%x,%d) %s", __FILE__, __LINE__,a1,a2,GetClassName(a1));
    ((Sonicteam::SoX::Graphics::Frame*)a1)->RemoveFromParent();
    LeaveCriticalSection(&ms_test2);
    return a1;
}

HOOKV3(0x8257EAB0, void*, sub_8257EAB0,
    (void*, void*, void*), (a1, a2, a3),
    void* a1, void* a2, void* a3)
{
    memset(a1, 0, 0x78);
    BranchTo(0x824F55A8, void, a1, a2);

    *(uint32_t*)a1 = 0x8203DC74;
    *(uint32_t*)((uint32_t)a1 + 0x20) = 0x8203DC68;

    *(uint32_t*)((uint32_t)a1 + 0x4C) = (uint32_t)a3;
    BranchTo(0x824F9DE8, void, (uint32_t)a1 + 0x50);

    *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x60) = new FFrameSynchronizerThread((Sonicteam::SoX::Graphics::FrameSynchronizer*)a3, 0, 1);
    *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x64) = new FFrameSynchronizerThread((Sonicteam::SoX::Graphics::FrameSynchronizer*)a3, 1, 1);
    *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x68) = new FFrameSynchronizerThread((Sonicteam::SoX::Graphics::FrameSynchronizer*)a3, 2, 1);
    *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x6C) = new FFrameSynchronizerThread((Sonicteam::SoX::Graphics::FrameSynchronizer*)a3, 3, 1);
    *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x70) = new FFrameSynchronizerThread((Sonicteam::SoX::Graphics::FrameSynchronizer*)a3, 4, 1);
    *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x74) = new FFrameSynchronizerThread((Sonicteam::SoX::Graphics::FrameSynchronizer*)a3, 5, 1);

    BranchTo(0x824FA1F8, void, (uint32_t)a1 + 0x50, *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x60));
    BranchTo(0x824FA1F8, void, (uint32_t)a1 + 0x50, *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x64));
    BranchTo(0x824FA1F8, void, (uint32_t)a1 + 0x50, *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x68));
    BranchTo(0x824FA1F8, void, (uint32_t)a1 + 0x50, *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x6C));
    BranchTo(0x824FA1F8, void, (uint32_t)a1 + 0x50, *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x70));
    BranchTo(0x824FA1F8, void, (uint32_t)a1 + 0x50, *(FFrameSynchronizerThread**)((uint32_t)a1 + 0x74));

    return a1;
}


HOOKV3(0x82582AE8, FStepableThread*, sub_82582AE8,
    (FStepableThread*, boost::function<void()>&, boost::function<void()>&, uint32_t), (pThread, OnStart, OnEnd, Proccessor),
    FStepableThread* pThread, boost::function<void()>& OnStart, boost::function<void()>& OnEnd, uint32_t Proccessor)
{
    //HardwareBreakpoint(__FILE__, __LINE__, "LOL2");
    new (pThread) FStepableThread(OnStart, OnEnd, Proccessor);
    return pThread;
}

HOOKV3(0x82567A30, FHavokRunnerThread*, sub_82567A30,
    (FHavokRunnerThread*, Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Physics::Havok::WorldHavok>&), (pThread, spWorld),
    FHavokRunnerThread* pThread, Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Physics::Havok::WorldHavok>& spWorld)
{
    new (pThread) FHavokRunnerThread(spWorld);
    return pThread;
}


void Patch::RestoreThreading::INSTALL()
{

    // Threading
    INSTALL_HOOKV3EX(sub_82582AE8, -1, true, 9);
    INSTALL_HOOKV3EX(sub_82567A30, -1, true, 9);
    // .. Others Too

    // Fix Others Issues

    // Thread Join Fix
    INSTALL_HOOKV3EX(sub_824F9B10, -1, true, 5);
    INSTALL_HOOKV3EX(sub_824F9CE8, -1, true, 5);

    /// Alright These 3, produced same result, Crash still in same place where it was before, like Frames issues, no idea why corrupt even happends
    // Thread Destructor Fix
  //  INSTALL_HOOKV3EX(sub_824F9CE8, -1, true, 5);

   // HookNew::WriteBranch((void*)0x824F9CE8, sub_824F9CE8, 5);




  //  INSTALL_HOOKV3EX(sub_82582C30, -1, true, 5);
    /*   
   
    //StepableThread (set 0x58 to 1, 0x825829D0) <- IStepable Model
    // where set to 0 , 0x82582D00

  

    // Task Destructor (Test)
    //INSTALL_HOOKV3EX(sub_8257E638, -1, true, 5);

    // Frame Remove Test
    INSTALL_HOOKV3EX(sub_825074E0, -1, true, 5);

    // ADITIONAL (That changes FrameSync Node of list)
    INSTALL_HOOKV3EX(sub_825070E8, -1, true, 5);

    // FrameSyncronizerTask ( replace threads to custom variants )
    INSTALL_HOOKV3EX(sub_8257EAB0, -1, true, 9);

    // Replace Stepable Thread to Custom One
    INSTALL_HOOKV3EX(sub_82582AE8, -1, true, 9);
         */
    // Adjust memory allocation to new type
    HookNew::WRITE_VALUE(0x821FB0A8, POWERPC_ADDIS(3, 0, sizeof(FStepableThread)));
    HookNew::WRITE_VALUE(0x821FE1D0, POWERPC_ADDIS(3, 0, sizeof(FStepableThread)));
    HookNew::WRITE_VALUE(0x822B1D0C, POWERPC_ADDIS(3, 0, sizeof(FStepableThread)));
    HookNew::WRITE_VALUE(0x823BF7C4, POWERPC_ADDIS(3, 0, sizeof(FStepableThread)));

    HookNew::WRITE_VALUE(0x8214CA28, POWERPC_ADDIS(3, 0, sizeof(FHavokRunnerThread)));



    InitializeCriticalSection(&ms_test);
    InitializeCriticalSection(&ms_test2);
}