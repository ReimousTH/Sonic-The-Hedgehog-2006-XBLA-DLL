#pragma  once
#include <Marathon.inl>
#include <Sox/MessageReceiver.h>
#include <Marathon.inl>


namespace Sonicteam
{
	namespace Player
	{
		namespace AI
		{
			class IBase
			{

				IBase(void);
				MARATHON_DESTRUCTION_H(IBase);
				virtual void AIOnMessageRecieved(int UnkFlag,Sonicteam::SoX::MessageReceiver*) = 0;
				virtual void AIBaseFlagExport() = 0;
			};

		}

	}
}
