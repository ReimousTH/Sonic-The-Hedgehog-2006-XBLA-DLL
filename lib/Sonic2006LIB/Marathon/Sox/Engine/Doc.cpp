#include "Doc.h"
#include <Sox/Engine/Profiler.h>
#include <Sox/Perfomance.h>
#include <Sox/Graphics/Device.h>
#include <Sox/Engine/View.h>
#include <Sox/Engine/RenderScheduler.h>

using namespace Sonicteam::SoX::Engine;

void __KernelUpdateRootTask1(Task* pTask,double delta)
{
	if (pTask->m_pDependencies)
	{
		for (Task::iterator it = pTask->begin(); it != pTask->end();)
		{
			Task* pChildTask = &*it;
			++it;
			uint32_t TaskFlags = pChildTask->m_Flag1 >> 24;
			if ((TaskFlags & 1) != 0)
				pChildTask->DestroyObject(1);
			
			else if ((TaskFlags & 4) == 0)
			{
				uint64_t startTicks = __mftb();
				pChildTask->OnStep(delta);
				uint64_t ticksPassed = __mftb() - startTicks;
				pChildTask->m_Timestamp = Profiler::TicksToMicroseconds(ticksPassed);
				if (pChildTask->m_pDependencies)
					__KernelUpdateRootTask1(pChildTask, delta);
			}
		}
	}
}

void __KernelUpdateRootTask2(Task* pTask,double delta)
{
	if (pTask->m_pDependencies)
	{
		for (Task::iterator it = pTask->begin(); it != pTask->end();)
		{
			Task* pChildTask = &*it;
			++it;
			uint32_t TaskFlags = pChildTask->m_Flag1 >> 24;
			if ((TaskFlags & 1) != 0)
				pChildTask->DestroyObject(1);
			
			else if ((TaskFlags & 4) == 0)
			{
				pChildTask->OnStep(delta);
				if (pChildTask->m_pDependencies)
					__KernelUpdateRootTask1(pChildTask, delta);
			}
		}
	}
}

void Doc::__KernelUpdateRootTask(double deltaTime)
{
	EnterCriticalSection(&m_pTasksSyncronizer);
	if ((m_Field4 & 1) != 0)
	{
		Sonicteam::SoX::PerformanceFrequency::getInstance();
		__KernelUpdateRootTask1(m_pRootTask, deltaTime);
		LeaveCriticalSection(&m_pTasksSyncronizer);
	}
	else
	{
		__KernelUpdateRootTask2(m_pRootTask, deltaTime);
		LeaveCriticalSection(&m_pTasksSyncronizer);
	}
}

void Doc::__KernelUpdateRootGTask(double deltaTime)
{
	EnterCriticalSection(&m_pGraphicsSynronizer);
	if (m_pView)
	{
		SoX::Graphics::Device* pDevice = m_pView->m_pDevice;
		if (pDevice->Begin1())
		{
			m_RenderScheduler->Update(m_pView, deltaTime);
			pDevice->End1();
		}
	}
	LeaveCriticalSection(&m_pGraphicsSynronizer);
}

void Doc::Update(double deltaTime)
{
	__KernelUpdateRootTask(deltaTime);
	__KernelUpdateRootGTask(deltaTime);
}


void Doc::__KernelUpdateRootGTask()
{
	EnterCriticalSection(&m_pGraphicsSynronizer);
	if (m_pView)
	{
		SoX::Graphics::Device* pDevice = m_pView->m_pDevice;
		if (pDevice->Begin2())
		{
			m_RenderScheduler->Update(m_pView, 0.0);
			pDevice->End2();
		}
	}
	LeaveCriticalSection(&m_pGraphicsSynronizer);
}

void Doc::UpdateRender()
{
	__KernelUpdateRootGTask();
}

