#include "FrameSynchronizerTask.h"
#include <FrameSynchronizerThread.h>

using namespace Sonicteam;

struct __KernelThreadInitializeStepCount
{
	uint32_t stepFrameCount;
	__KernelThreadInitializeStepCount(uint32_t stepFrameCount) : stepFrameCount(stepFrameCount) {}

	void operator()(FrameSynchronizerThread* pThread) const
	{
		pThread->m_StepFrameCount = stepFrameCount;
	}
};

void FrameSynchronizerTask::__KernelAddThread(FrameSynchronizerThread* pThread)
{
	m_llThreads.Insert((SoX::LinkNode<FrameSynchronizerThread>*)&pThread->m_lnThread);
	m_llThreads.ForEach(__KernelThreadInitializeStepCount(m_llThreads.m_Flags));
}

FrameSynchronizerTask::FrameSynchronizerTask(SoX::Engine::Task* pParentTask, SoX::Graphics::FrameSynchronizer* pSyncronizer) : Task(pParentTask)
{
	m_pSynchronizer = pSyncronizer;
	m_llThreads.InitializeList();

	if (m_apThreads[0])
		m_apThreads[0]->DestroyObject(1);

	m_apThreads[0] = new Sonicteam::FrameSynchronizerThread(m_pSynchronizer,0,1);

	if (m_apThreads[1])
		m_apThreads[1]->DestroyObject(1);

	m_apThreads[1] = new Sonicteam::FrameSynchronizerThread(m_pSynchronizer,1,1);

	if (m_apThreads[2])
		m_apThreads[2]->DestroyObject(1);

	m_apThreads[2] = new Sonicteam::FrameSynchronizerThread(m_pSynchronizer,2,1);

	if (m_apThreads[3])
		m_apThreads[3]->DestroyObject(1);

	m_apThreads[3] = new Sonicteam::FrameSynchronizerThread(m_pSynchronizer,3,1);

	if (m_apThreads[4])
		m_apThreads[4]->DestroyObject(1);

	m_apThreads[4] = new Sonicteam::FrameSynchronizerThread(m_pSynchronizer,4,1);

	if (m_apThreads[5])
		m_apThreads[5]->DestroyObject(1);

	m_apThreads[5] = new Sonicteam::FrameSynchronizerThread(m_pSynchronizer,5,1);

	__KernelAddThread(m_apThreads[0]);
	__KernelAddThread(m_apThreads[1]);
	__KernelAddThread(m_apThreads[2]);
	__KernelAddThread(m_apThreads[3]);
	__KernelAddThread(m_apThreads[4]);
	__KernelAddThread(m_apThreads[5]);
}

FrameSynchronizerTask::~FrameSynchronizerTask()
{
	for (int i = 0; i < 5; i++)
	{
		if (m_apThreads[i])
			m_apThreads[i]->DestroyObject(1);
	}
	m_llThreads.ResetListFast();
}

char* FrameSynchronizerTask::GetType()
{
	return "FrameSynchronizerTask";
}

struct __KernelThreadUpdateDeltaTime
{
	float deltaTime;
	__KernelThreadUpdateDeltaTime(float deltaTime) : deltaTime(deltaTime) {}

	void operator()(FrameSynchronizerThread* pThread) const
	{
		pThread->m_DeltaTime = deltaTime;
	}
};

void __KernelStepThread(FrameSynchronizerThread* pThread)
{
	if (pThread->m_IsSuspended)
	{
		pThread->m_IsSuspended = 0;
		ResumeThread(pThread->m_Handle);
	}
	SetEvent(pThread->m_StepFrameEvent);
}

void FrameSynchronizerTask::__KernelStepThreads(double deltaTime)
{
	m_llThreads.ForEach(__KernelThreadUpdateDeltaTime(deltaTime));
	m_llThreads.ForEach(__KernelStepThread);
	m_llThreads.m_Flags++;
}

// Just Wait Thread to finish his Step, so we could trigger Step Event once more
void __KernelSynchronizeThread(FrameSynchronizerThread* pThread)
{
	pThread->Syncronize();
}

void FrameSynchronizerTask::__KernelSynchronizeThreads()
{
	m_llThreads.ForEach(__KernelSynchronizeThread);
}

void FrameSynchronizerTask::OnStep(float deltaTime)
{
	__KernelStepThreads(deltaTime);
	__KernelSynchronizeThreads();
	m_pSynchronizer->SynchronizeFrames();
}
