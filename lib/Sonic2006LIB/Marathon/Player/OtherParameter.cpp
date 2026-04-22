#include "OtherParameter.h"
using namespace Sonicteam::Player;

Sonicteam::Player::OtherParameter::OtherParameter(void):
IPlugIn("other parameter")
{

}

Sonicteam::Player::OtherParameter::OtherParameter(void* m_pCameraman):
CameramanPlayer(m_pCameraman),
IPlugIn("other parameter")
{

}

Sonicteam::Player::OtherParameter::~OtherParameter(void)
{

}

DESTRUCTION_CPP(OtherParameter);