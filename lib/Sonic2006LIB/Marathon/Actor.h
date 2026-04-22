#pragma once

#include <Marathon.inl>
#include "ActorManager.h"
#include <Sox/Memory/IDestructible.h>
#include <Sox/Engine/Task.h>
#include <boost/weak_ptr.hpp>

namespace Sonicteam
{
	class GameImp;

	class Actor :public Sonicteam::SoX::Engine::Task
	{
	public:
		~Actor();
		Actor();
		Actor(Sonicteam::Actor* other, boost::weak_ptr<Sonicteam::GameImp> gameimp);

		boost::weak_ptr<Sonicteam::GameImp> GameImp; //0x4C-0x50
		uint32_t ActorID; //0x54
		MARATHON_DESTRUCTION_H(Actor);
	};
};
