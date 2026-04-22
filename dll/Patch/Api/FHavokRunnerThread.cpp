#include <Sox/Perfomance.h>
#include "FHavokRunnerThread.h"

FHavokRunnerThread::FHavokRunnerThread(Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Physics::Havok::WorldHavok>& world) : FThread("FHavokRunnerThread", 1, 1), m_spWorld(world)
{

}

FHavokRunnerThread::~FHavokRunnerThread()
{
	
}

void FHavokRunnerThread::Syncronize()
{
    FThread::Syncronize();
}

void FHavokRunnerThread::Run()
{
    char v4[848];
    uint32_t& dword_82D2BAF0 = *reinterpret_cast<uint32_t*>(0x82D2BAF0);

    BranchTo(0x828E6C08,int,&v4, dword_82D2BAF0, 0x10);
    BranchTo(0x828E7518,int,&v4);

    int v2 = (**(int(__fastcall***)(int, int, int))dword_82D2BAF0)(dword_82D2BAF0, 0x100000, 1);
    BranchTo(0x828E6BC8,int,&v4, v2, 0x100000);

    FThread::Run();
    BranchTo(0x828E76A8,int,this);

    (*(void(__fastcall**)(int, int))(*(uint32_t*)dword_82D2BAF0 + 4))(dword_82D2BAF0, v2);
    BranchTo(0x828E6B90,int,&v4);
}

void FHavokRunnerThread::OnStep(double deltaTime)
{
    if (this->m_spWorld.get())
        this->m_spWorld->PhysicsWorldUnk03(deltaTime);
}
