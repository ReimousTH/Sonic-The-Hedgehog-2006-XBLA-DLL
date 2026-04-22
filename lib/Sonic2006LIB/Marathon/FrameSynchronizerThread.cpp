#include <Sox/Perfomance.h>
#include "FrameSynchronizerThread.h"

using namespace Sonicteam;

FrameSynchronizerThread::FrameSynchronizerThread(Sonicteam::SoX::Graphics::FrameSynchronizer* pFrameSyncronizer,uint32_t Proccessor, bool IsSyncronize) : Thread("FrameSynchronizerThread", Proccessor, IsSyncronize)
{
	this->m_SyncID = Proccessor;
	this->m_pFrameSyncronizer = pFrameSyncronizer;
}

FrameSynchronizerThread::~FrameSynchronizerThread()
{
}

void FrameSynchronizerThread::Syncronize()
{
	Thread::Syncronize();
}

void FrameSynchronizerThread::Run()
{
	while (m_IsRunned)
	{
		if (m_IsSyncronize)
		{
			WaitForSingleObject(m_StepFrameEvent, -1);
			__ThreadKernelStep(this);
			SetEvent(m_StepFrameEventEnd);
		}
		else
		{
			__ThreadKernelStep(this);
		}
			
	}
}

void FrameSynchronizerThread::OnStep(double deltaTime)
{
	BranchTo(0x82507B80, void, m_pFrameSyncronizer, m_SyncID, deltaTime);
}
