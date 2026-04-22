#pragma once

#include <Marathon.inl>
#include <Sox/Graphics/Device.h>

namespace Sonicteam
{
	namespace SoX
	{
		namespace Engine
		{
			class Doc;

			class View
			{
			public:
				View(void); //no idea THRERE TWO OF THEM
				~View(void);
				MARATHON_DESTRUCTION_H(View);
				View* m_pParentDoc;
				Graphics::Device* m_pDevice;
			};
		};
	}
}
