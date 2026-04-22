#pragma once

#include <Marathon.inl>
#include "Object.h"
#include <Sox/Misc/LinkSoxNode.h>
#include <Marathon.inl>
#include <Sox/LinkNode.h>

namespace Sonicteam
{
	namespace SoX
	{

		class Component : public Object
		{
		public:
			Component();
			Component(Component*);
			~Component(void);

			Component* m_pParent; //0x4


			SoX::LinkNode<Component> m_lnComponent; //0x8,0xC,0x10
			SoX::LinkedList<Component> m_llComponents; //0x14,0x18,0x1C

			virtual char* GetType() override; 
			MARATHON_DESTRUCTION_H(Component);
		};
	}
}
