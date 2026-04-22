#pragma once

#include <Marathon.inl>
#include <Sox/Engine/Task.h>

namespace Sonicteam
{
	class AudioRoomTask : public Sonicteam::SoX::Engine::Task
	{
	public:
		uint32_t m_State;
	};
};

