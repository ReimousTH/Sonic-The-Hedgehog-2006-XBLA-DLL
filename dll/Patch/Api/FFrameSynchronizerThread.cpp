#include <Sox/Perfomance.h>
#include "FFrameSynchronizerThread.h"
#include <Sox/LinkNode.h>
#include <Sox/Graphics/Frame.h>
#include <FrameSynchronizerThread.h>>
#include <Core2/Debug.h>

FFrameSynchronizerThread::FFrameSynchronizerThread(Sonicteam::SoX::Graphics::FrameSynchronizer* pFrameSyncronizer,uint32_t Proccessor, bool IsSyncronize) : FThread("FrameSynchronizerThread", Proccessor, IsSyncronize)
{
	this->m_StepFrameEventEndAfter = CreateEvent(0, 0, 0, 0);
	this->m_SyncID = Proccessor;
	this->m_pFrameSyncronizer = pFrameSyncronizer;
}

FFrameSynchronizerThread::~FFrameSynchronizerThread()
{
	CloseHandle(this->m_StepFrameEventEndAfter);
}

void FFrameSynchronizerThread::Syncronize()
{
	FThread::Syncronize();
	SetEvent(m_StepFrameEventEndAfter);
}

void FFrameSynchronizerThread::Run()
{
	while (m_IsRunned)
	{
		if (m_IsSyncronize)
		{
			WaitForSingleObject(m_StepFrameEvent, -1);
			__ThreadKernelStep();
			SetEvent(m_StepFrameEventEnd);
			WaitForSingleObject(m_StepFrameEventEndAfter, -1); // Destructor Fix
		}
		else
		{
			__ThreadKernelStep();
			WaitForSingleObject(m_StepFrameEventEndAfter, -1); // Probably not need here
		}
			
	}
}


struct __KernelFrameSyncronizerStep
{
	XMMATRIX* defaultTransform;
	uint32_t flag;
	__KernelFrameSyncronizerStep(XMMATRIX* Transform, uint32_t flag) : defaultTransform(Transform), flag(flag) {}

	void operator()(Sonicteam::SoX::Graphics::Frame* pFrame) const
	{
		uint32_t flags = pFrame->m_Flag;
		pFrame->m_Flag &= ~0x3;
		if (((flags | flag) & 2) != 0)
		{
			pFrame->m_Flag |= 8;
			pFrame->Func18(defaultTransform);
		}
		XMMATRIX transform = pFrame->FuncC();
		if (((flags | flag) & 1) != 0)
		{
			for (Sonicteam::SoX::Graphics::Frame* child = pFrame->m_pFirstChild.get(); child; child = child->m_pNextSibling.get())
			{
				child->m_Flag |= 4;
				(__KernelFrameSyncronizerStep(&transform, flags | flag))(child);
			}
		}
	}
};

void __KernelFrameSyncronizerStep2(Sonicteam::SoX::Graphics::Frame* pFrame,uint32_t flag,XMMATRIX* transform)
{
	uint32_t flags = pFrame->m_Flag;
	pFrame->m_Flag &= ~0x3;
	if (((flags | flag) & 2) != 0)
	{
		pFrame->m_Flag |= 8;
		pFrame->Func18(transform);
	}
	XMMATRIX transform2 = pFrame->FuncC();
	if (((flags | flag) & 1) != 0)
	{
		for (Sonicteam::SoX::Graphics::Frame* child = pFrame->m_pFirstChild.get(); child; child = child->m_pNextSibling.get())
		{
			pFrame->m_Flag |= 4;
			__KernelFrameSyncronizerStep2(child, flags | flag, &transform2);
		}
	}
}

void __KernelFramesSyncronizerStep(Sonicteam::SoX::Graphics::FrameSynchronizer* pSynchronizer, uint32_t SyncID)
{
	Sonicteam::SoX::LinkedList<Sonicteam::SoX::Graphics::Frame>* pllFrames = &pSynchronizer->m_aollFrame[SyncID];
	if (pllFrames->begin() != pllFrames->end())
	{
		// This Should not happend but yet XBLA somehow DOES ( we just skip corrupted stuff like this )
		// TODO : Normal fix PLS
		if (pllFrames->m_pPrev == pllFrames && pllFrames->m_pNext != pllFrames)
		{
			HardwareBreakpoint(__FILE__, __LINE__, "Crash Incoming, ( F*** ) Synchronizer : %x SynchID : %x", pSynchronizer,SyncID);
			return;
		}

		for (Sonicteam::SoX::LinkNode<Sonicteam::SoX::Graphics::Frame>* it = pllFrames->m_pNext; it != pllFrames; it = it->m_pNext)
		{
			if (it->m_pThis)
				__KernelFrameSyncronizerStep2(it->m_pThis, 0, &XMMatrixIdentity());
		}
		//pllFrames->ForEach(__KernelFrameSyncronizerStep(&XMMatrixIdentity(), 0));
	}
}

void FFrameSynchronizerThread::OnStep(double deltaTime)
{
	//HardwareBreakpoint(__FILE__, __LINE__, "FFrameSynchronizerThread Step");
	__KernelFramesSyncronizerStep(m_pFrameSyncronizer, m_SyncID);
	//BranchTo(0x82507B80, void, m_pFrameSyncronizer, m_SyncID, deltaTime);
}
