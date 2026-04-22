#ifndef SONICTEAM__PACKAGEMODEL__ANIMATIONCB
#define SONICTEAM__PACKAGEMODEL__ANIMATIONCB

#include <Sox/RefCountObject.h>
#include <Marathon.inl>

namespace Sonicteam{
	namespace Player{
		namespace PackageModel{

			class AnimationCB:public Sonicteam::SoX::RefCountObject{
			public:

				unsigned int unk0x8;
				unsigned int type; //0xC;
				unsigned int value; //0x10 ????

				MARATHON_DESTRUCTION_H(AnimationCB);;

			};


		};

	}
}




#endif



