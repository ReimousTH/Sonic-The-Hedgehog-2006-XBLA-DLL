#include "MainMenuTask.h"

using namespace Sonicteam;

int MainMenuTask::OnMessageRecieved(SoXMessageType)
{
	return 0;
}

void MainMenuTask::OnStep(float)
{
	
}

Sonicteam::DocMarathonImp* Sonicteam::MainMenuTask::GetCurrentDoc()
{
	return (DocMarathonImp*)this->m_pDoc.get();
}

