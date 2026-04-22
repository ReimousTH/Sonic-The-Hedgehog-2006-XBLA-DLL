#pragma once

#include <Marathon.inl>
#include <Sox/IResource2.h>

namespace Sonicteam
{
	class EventPlaybookMgr;

	class EventPlaybook : public SoX::IResource2<EventPlaybook, EventPlaybookMgr>
	{
	public:
		EventPlaybook(void);
		~EventPlaybook(void);

		MARATHON_DESTRUCTION_H(EventPlaybook); //0x0 + 0x10
	};
};
