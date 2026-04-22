#include "FrustumCullingTask.h"

using namespace Sonicteam;

struct __KernelFrustumUpdateCount
{
	uint32_t count;
	__KernelFrustumUpdateCount(uint32_t count) : count(count) {}

	void operator()(FrustumCullingThread* Thread) const
	{
		Thread->m_StepFrameCount = count;
	}
};

Sonicteam::FrustumCullingTask::FrustumCullingTask(Task* parent, DocMarathonImp* doc) : Task(parent)
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

		this->m_apThreads[i] = new FrustumCullingThread(FrustumCullingThread::ms_FrustumCullingThreadProcessor[i]);
		// Should Be Method ( for multiple )
		{
			Sonicteam::SoX::Thread* pThread = this->m_apThreads[i];
			pThread->m_ParentList = (uint32_t)&this->m_llThreads;
			this->m_llThreads.InsertFast((SoX::LinkNode<FrustumCullingThread>*)&pThread->m_lnThread);
			this->m_llThreads.m_pThis++;
			this->m_llThreads.ForEach(__KernelFrustumUpdateCount((uint32_t)this->m_llThreads.m_pThis));
		}
	}
}

Sonicteam::FrustumCullingTask::~FrustumCullingTask(void)
{
	for (int i = 0; i < 5; i++)
	{
		m_apThreads[i]->DestroyObject(1);
	}
	m_llThreads.ResetList();
}
