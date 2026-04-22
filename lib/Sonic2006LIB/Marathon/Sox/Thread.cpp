#include "Thread.h"
#include <Sox/Perfomance.h>

using namespace Sonicteam::SoX;

DWORD WINAPI __ThreadKernelRun(LPVOID lpParameter)
{

	if (lpParameter)
		((Sonicteam::SoX::Thread*)lpParameter)->Run();

	return 0;

}

Thread::Thread(const char* Name, uint32_t Processor, bool IsSyncronize)
{
	this->m_lnThread.m_pNext = 0;
	this->m_lnThread.m_pPrev = 0;
	this->m_lnThread.m_pThis = this;
	this->m_DeltaTime = 0;
	this->m_IsRunned = 1;
	this->m_ParentList = 0;
	this->m_StepFrameCount = 0;
	this->m_Name = Name;
	this->m_StepTime = 0;
	this->m_IsSuspended = 1;
	this->m_IsSyncronize = IsSyncronize;
	this->m_StepFrameEvent = CreateEvent(0, 0, 0, 0);
	this->m_StepFrameEventEnd = CreateEvent(0, 0, 0, 0);
	this->m_Handle = CreateThread(0, 0, __ThreadKernelRun, this, CREATE_SUSPENDED, (LPDWORD)&this->m_ID);
	// Function to set Thread Name (also misisng )
	XSetThreadProcessor(this->m_Handle, Processor);
}

Thread::~Thread()
{
	if (m_IsRunned)
	{
		m_IsRunned = 0;
		if (m_IsSuspended)
		{
			m_IsSuspended = 0;
			ResumeThread(m_Handle);
		}
		SetEvent(m_StepFrameEvent);
		WaitForSingleObject(m_Handle, -1);
		//CloseHandle(m_Handle); Retail Behaviour
	}
	CloseHandle(m_StepFrameEvent);
	CloseHandle(m_StepFrameEventEnd);
	if (m_ParentList && m_lnThread.m_pNext != 0)
	{
		m_lnThread.ResetFast();
	}
	m_lnThread.Reset();
}

void Thread::Syncronize()
{
	uint64_t startTime = Sonicteam::SoX::PerformanceFrequency::getInstance().GetTimeBase();
	WaitForSingleObject(this->m_StepFrameEventEnd, -1);
	uint64_t endTime = Sonicteam::SoX::PerformanceFrequency::getInstance().GetTimeBase();
	this->m_StepEndTime = ((endTime - startTime) * 0x000F4240) / Sonicteam::SoX::PerformanceFrequency::getInstance().GetPerformanceFrequency().QuadPart;
}

void Thread::__ThreadKernelStep(Thread* pThread)
{
	uint64_t startTime = Sonicteam::SoX::PerformanceFrequency::getInstance().GetTimeBase();
	pThread->OnStep(pThread->m_DeltaTime);
	uint64_t endTime = Sonicteam::SoX::PerformanceFrequency::getInstance().GetTimeBase();
	pThread->m_StepTime = ((endTime - startTime) * 0x000F4240) / Sonicteam::SoX::PerformanceFrequency::getInstance().GetPerformanceFrequency().QuadPart;
	pThread->m_StepFrameCount++;
}

void Thread::Run()
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
			__ThreadKernelStep(this);
	}
}

void Thread::OnStep(double)
{

}

void Thread::Join()
{
	if (m_IsSuspended)
	{
		m_IsSuspended = false;
		ResumeThread(m_Handle);
	}
	SetEvent(m_StepFrameEvent);
}
