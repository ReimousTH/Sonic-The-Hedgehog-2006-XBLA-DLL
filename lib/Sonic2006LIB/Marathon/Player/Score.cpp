#include "Score.h"

using namespace Sonicteam::Player;




Score::~Score(void)
{

}

//GameModules (GameImpulse,PackageBinary)
Score::Score(int* PtrObjectPlayer,int* m_spRootFrame,int DefaultValue,int* GameModules)
{

}

void Score::AddRings(uint32_t value)
{
	this->RingsCount+=value;
}

void Score::TakeDamage(void)
{
	
}


void Score::TakeDamageLittle()
{
	
}

uint32_t Score::GetRings()
{
	return this->RingsCount;
}

void Score::SetRings(uint32_t value)
{
	this->RingsCount = value;
}

void Score::SetUnknownVector(XMFLOAT4* value)
{
	this->unkvector = *value;
}

void Score::OnVarible(void)
{
	
}

void Score::OnStepable(float)
{
	
}

