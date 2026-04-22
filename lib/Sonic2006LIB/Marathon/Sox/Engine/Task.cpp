#include "Task.h"

using namespace Sonicteam::SoX::Engine;

int Task::OnMessageRecieved(SoXMessageType)
{
	return 0;
}

void Task::OnStep(float)
{

}

Task::Task()
{

}
Task::Task(Task* pParent) : Component(pParent)
{
	m_pDependency = pParent;
	m_pDependencies = 0;
	m_pDoc = 0;
	m_llTask.InitializeList();
	pParent->AddChild(this);
}

void __TaskKernelDestroy(Task* pTask)
{
	// probably don't matter cuz its dumb
}

Task::~Task()
{
	RemoveDependencies();
	RemoveFromParent();
	m_llTask.ForEach(__TaskKernelDestroy);
	m_llTask.ResetListFast();
}
