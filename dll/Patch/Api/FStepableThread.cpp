#include <Sox/Perfomance.h>
#include "FStepableThread.h"

FStepableThread::FStepableThread(boost::function<void()> OnStart, boost::function<void()> OnEnd, uint32_t Proccessor) : FThread("StepableThread", Proccessor, 1)
{
	this->m_OnStart = OnStart;
	this->m_OnEnd = OnEnd;
	this->m_StepFrameEventEndAfter = CreateEvent(0, 0, 0, 0);
}

FStepableThread::~FStepableThread()
{
	CloseHandle(this->m_StepFrameEventEndAfter);
}

void FStepableThread::Syncronize()
{
	if (m_IsStepped)
	{
		m_IsStepped = 0;
		FThread::Syncronize();
		SetEvent(m_StepFrameEventEndAfter);
		m_OnEnd();
	}
	else
		SetEvent(m_StepFrameEventEndAfter);
}

void FStepableThread::Run()
{
	while (m_IsRunned)
	{
		if (m_IsSyncronize)
		{
			WaitForSingleObject(m_StepFrameEvent, -1);
			__ThreadKernelStep();
			SetEvent(m_StepFrameEventEnd);
			//WaitForSingleObject(m_StepFrameEventEndAfter, -1); // Destructor Fix
		}
		else
		{
			__ThreadKernelStep();
			//WaitForSingleObject(m_StepFrameEventEndAfter, -1); // Probably not need here
		}
	}
}

void FStepableThread::OnStep(double deltaTime)
{
	m_OnStart();
}
