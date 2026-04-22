#pragma once

#include <Marathon.inl>
#include <Sox/RefCountObject.h>

namespace Sonicteam
{
	namespace Prop
	{
		class AxisAlignedBoxShape;

		class ActiveArea : public Sonicteam::SoX::RefCountObject
		{

			ActiveArea();
			~ActiveArea();
			MARATHON_DESTRUCTION_H(ActiveArea);

			MARATHON_INSERT_PADDING(0x10);
			AxisAlignedBoxShape* m_pArea;
			MARATHON_INSERT_PADDING(0x64);
		};

	}
}
