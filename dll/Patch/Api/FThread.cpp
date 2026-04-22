#include "FThread.h"
#include <Sox/Perfomance.h>

DWORD WINAPI __ThreadKernelRun(LPVOID lpParameter)
{

	if (lpParameter)
		((FThread*)lpParameter)->Run();

	return 0;

}

void SetThreadName(DWORD dwThreadID, const char* threadName) 
{
#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO {
		uint32_t dwType;      // Must be 0x1000
		const char* szName;     // Pointer to name (ASCII)
		uint32_t dwThreadID;  // Thread ID (-1 = current thread)
		uint32_t dwFlags;     // Reserved (0)
	} THREADNAME_INFO;
#pragma pack(pop)


	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try {
		RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR),
			(ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {}
}

FThread::FThread(const char* Name, uint32_t Proccessor, bool IsSyncronize)
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
	SetThreadName(this->m_ID, Name);

	// Function to set Thread Name (also misisng )
	XSetThreadProcessor(this->m_Handle, Proccessor);
}

FThread::~FThread()
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
		CloseHandle(m_Handle); 
	}
	CloseHandle(m_StepFrameEvent);
	CloseHandle(m_StepFrameEventEnd);
	if (m_ParentList && m_lnThread.m_pNext != 0)
	{
		m_lnThread.ResetFast();
	}
	m_lnThread.ResetQuick();
}

void FThread::Syncronize()
{
	uint64_t startTime = Sonicteam::SoX::PerformanceFrequency::getInstance().GetTimeBase();
	WaitForSingleObject(this->m_StepFrameEventEnd, -1);
	uint64_t endTime = Sonicteam::SoX::PerformanceFrequency::getInstance().GetTimeBase();
	this->m_StepEndTime = ((endTime - startTime) * 0x000F4240) / Sonicteam::SoX::PerformanceFrequency::getInstance().GetPerformanceFrequency().QuadPart;
}

void FThread::__ThreadKernelStep()
{
	uint64_t startTime = Sonicteam::SoX::PerformanceFrequency::getInstance().GetTimeBase();
	OnStep(m_DeltaTime);
	uint64_t endTime = Sonicteam::SoX::PerformanceFrequency::getInstance().GetTimeBase();
	m_StepTime = ((endTime - startTime) * 0x000F4240) / Sonicteam::SoX::PerformanceFrequency::getInstance().GetPerformanceFrequency().QuadPart;
	m_StepFrameCount++;
}

void FThread::Run()
{
	while (m_IsRunned)
	{
		if (m_IsSyncronize)
		{
			WaitForSingleObject(m_StepFrameEvent, -1);
			__ThreadKernelStep();
			SetEvent(m_StepFrameEventEnd);
		}
		else
			__ThreadKernelStep();
	}
}

void FThread::OnStep(double)
{

}
