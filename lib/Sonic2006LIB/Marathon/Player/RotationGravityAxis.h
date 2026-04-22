#ifndef SONIC__PLAYER__ROTATIONGRAVITYAXIS
#define SONIC__PLAYER__ROTATIONGRAVITYAXIS


#include <Marathon.inl>
#include <Player/IPosturePlugIn.h>


namespace Sonicteam{
	namespace Player{

		class RotationGravityAxis:Sonicteam::Player::IPosturePlugIn
		{
		public:
			RotationGravityAxis(void);
			~RotationGravityAxis(void);
			MARATHON_DESTRUCTION_H(RotationGravityAxis);;
			XMVECTOR GravityVector; //821FB8EC (where it stores)
			

		};
	}
}

#endif

