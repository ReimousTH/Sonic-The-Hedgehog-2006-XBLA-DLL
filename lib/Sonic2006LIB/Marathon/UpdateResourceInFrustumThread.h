#pragma once

#include <Marathon.inl>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <Sox/Thread.h>
#include <Sox/Scenery/World.h>

namespace Sonicteam
{
	class UpdateResourceInFrustumThread : public SoX::Thread
	{
	public:
		UpdateResourceInFrustumThread(uint32_t);
		~UpdateResourceInFrustumThread(void);

		MARATHON_DESTRUCTION_H(UpdateResourceInFrustumThread);
		virtual void OnStep(double);

		std::vector<boost::shared_ptr<SoX::Scenery::World>> m_vspWorlds;

		static uint32_t ms_FrustumCullingThreadProcessor[6];
	};
};
