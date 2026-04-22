#pragma once

#include <Marathon.inl>
#include <Sox/RefCountObject.h>

namespace Sonicteam 
{
	namespace SoX 
	{
		namespace Physics 
		{
			class CollisionFilter : public Sonicteam::SoX::RefCountObject
			{
			public:
				//size0x60
				CollisionFilter(void);
				~CollisionFilter(void);

				MARATHON_DESTRUCTION_H(CollisionFilter);
				virtual bool IsCollisionAllowed() = 0;
			};
		};
	}
}
