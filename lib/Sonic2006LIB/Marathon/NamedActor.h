#pragma once

#include <Marathon.inl>
#include <Actor.h>

namespace Sonicteam
{
	class NamedActor : public Sonicteam::Actor
	{

	public:
		//Sonicteam::Game(base class missing)
		NamedActor(Sonicteam::Actor*, boost::shared_ptr<Sonicteam::GameImp>, const char*);
		~NamedActor();

		std::string ActorName;

		MARATHON_DESTRUCTION_H(NamedActor);;

		virtual void OnStep(float);
		void UpdateKhronoTime(double);
	};
};