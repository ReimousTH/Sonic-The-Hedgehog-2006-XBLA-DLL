#include "UpdateResourceInFrustumTask.h"
#include "FrustumCullingThread.h"
#include "DocMarathonImp.h"

using namespace Sonicteam;

struct __KernelFrustumUpdateCount
{
	uint32_t count;
	__KernelFrustumUpdateCount(uint32_t count) : count(count) {}

	void operator()(UpdateResourceInFrustumThread* Thread) const
	{
		Thread->m_StepFrameCount = count;
	}
};

struct __KernelFrustumUpdateDelta
{
	double deltaTime;
	__KernelFrustumUpdateDelta(double deltaTime) : deltaTime(deltaTime) {}

	void operator()(UpdateResourceInFrustumThread* Thread) const
	{
		Thread->m_DeltaTime = deltaTime;
	}
};

struct __KernelFrustumUpdateThread
{
	__KernelFrustumUpdateThread() {}

	void operator()(UpdateResourceInFrustumThread* Thread) const
	{
		Thread->Join();
	}
};

struct __KernelFrustumSyncronizeThread
{
	__KernelFrustumSyncronizeThread() {}

	void operator()(UpdateResourceInFrustumThread* Thread) const
	{
		Thread->Syncronize();
	}
};

Sonicteam::UpdateResourceInFrustumTask::UpdateResourceInFrustumTask(Task* parent, DocMarathonImp* doc) : Task(parent)
{
	this->m_pDoc = doc;
	this->m_llThreads.InitializeList();
	this->m_llThreads.m_pThis = 0;
	this->m_apThreads[0] = 0;
	this->m_apThreads[1] = 0;
	this->m_apThreads[2] = 0;
	this->m_apThreads[3] = 0;
	this->m_apThreads[4] = 0;
	this->m_ThreadsCount = 5;
	for (int i = 0; i < m_ThreadsCount; i++)
	{
		if (this->m_apThreads[i])
			this->m_apThreads[i]->DestroyObject(1);

		this->m_apThreads[i] = new UpdateResourceInFrustumThread(FrustumCullingThread::ms_FrustumCullingThreadProcessor[i]);
		// Should Be Method ( for multiple )
		{
			Sonicteam::SoX::Thread* pThread = this->m_apThreads[i];
			pThread->m_ParentList = (uint32_t)&this->m_llThreads;
			this->m_llThreads.InsertFast((SoX::LinkNode<UpdateResourceInFrustumThread>*)&pThread->m_lnThread);
			this->m_llThreads.m_pThis++;
			this->m_llThreads.ForEach(__KernelFrustumUpdateCount((uint32_t)this->m_llThreads.m_pThis));
		}
	}
}

Sonicteam::UpdateResourceInFrustumTask::~UpdateResourceInFrustumTask(void)
{
	for (int i = 0; i < 5; i++)
	{
		m_apThreads[i]->DestroyObject(1);
	}
	m_llThreads.ResetList();
}

void Sonicteam::UpdateResourceInFrustumTask::OnStep(float deltaTime)
{
	uint32_t threadId = 0;
	for (int i = 0; i < 0xD; i++)
	{
		boost::shared_ptr<Sonicteam::SoX::Scenery::World> spWorld = m_pDoc->DocGetWorld(i);
		if (spWorld.get())
		{
			m_apThreads[threadId]->m_vspWorlds.push_back(spWorld);
		}
		threadId = (threadId + 1) % m_ThreadsCount;
	}

	{
		m_llThreads.ForEach(__KernelFrustumUpdateDelta((double)deltaTime));
		m_llThreads.ForEach(__KernelFrustumUpdateThread());
		(*(uint32_t*)(&m_llThreads.m_pThis))++;
	}

	{
		m_llThreads.ForEach(__KernelFrustumSyncronizeThread());
	}

	for (int i = 0; i < m_ThreadsCount; i++)
	{
		UpdateResourceInFrustumThread* pThread = m_apThreads[i];
		pThread->m_vspWorlds.clear();
	}
}
