#include "HUDOption.h"

using namespace Sonicteam;

Sonicteam::SoX::Engine::Doc* Sonicteam::HUDOption::GetCurrentDoc()
{
	return 	this->m_pDoc.get();
}


Sonicteam::HUDOption::HUDOption(Sonicteam::SoX::Engine::Doc* doc)
{

}

void Sonicteam::HUDOption::OnStep(float)
{

}

int Sonicteam::HUDOption::OnMessageRecieved(Sonicteam::SoX::IMessage*)
{

	return 0;
}


Sonicteam::HUDOption::~HUDOption()
{
	
}


