#include "GTask.h"

using namespace Sonicteam::SoX::Engine;

GTask::GTask(GTask* a1): Component(a1)
{
	m_Timestamp = 0;
	m_pPrevSibling = a1;
	m_pNextSibling = 0;
	m_pDependency = 0;
	m_pDependencies = 0;

}

GTask::~GTask()
{
	RemoveDependencies();
	RemoveFromParent();
}

char* Sonicteam::SoX::Engine::GTask::GetType()
{
	return "GTask";
}

