#include "Machine2.h"

using namespace Sonicteam::Player::State;

Machine2::Machine2(void):IMachine()
{

}

Machine2::~Machine2(void)
{

}

void Machine2::Update(float)
{

}


unsigned int Machine2::ChangeState(int)
{
return 0;
}

unsigned int Machine2::ChangeStateQueue(int)
{

	return 0;
}

void Machine2::EnableQueueState()
{

}

void Machine2::EnableQueueStatePop()
{

}

unsigned int Machine2::ChangeStatePop(int)
{

	return 0;
}

unsigned int Machine2::GetStateID()
{
 return 0;
}

boost::shared_ptr<Sonicteam::Player::State::IContext> Machine2::GetStateContext()
{

	
	return this->m_spContext;
	
}

DESTRUCTION_CPP(Machine2);
