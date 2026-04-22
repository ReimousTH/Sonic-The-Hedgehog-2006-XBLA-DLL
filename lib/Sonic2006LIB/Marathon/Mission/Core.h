#pragma once

#include <Sox/MessageReceiver.h>

namespace Sonicteam
{
	namespace Mission
	{

		class Core : Sonicteam::SoX::MessageReceiver
		{

		public:
			Core();
			~Core();

			MARATHON_DESTRUCTION_H(Core);
		};

	}
}