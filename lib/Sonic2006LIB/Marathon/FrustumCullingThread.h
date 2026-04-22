#pragma once

#include <Marathon.inl>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <Sox/Thread.h>
#include <Sox/Scenery/World.h>

namespace Sonicteam
{
	class FrustumCullingThread : public SoX::Thread
	{
	public:
		FrustumCullingThread(uint32_t);
		~FrustumCullingThread(void);

		MARATHON_DESTRUCTION_H(FrustumCullingThread);
		virtual void OnStep(double);

		std::vector<boost::shared_ptr<SoX::Scenery::World>> m_vspWorlds; // 0x40

		static uint32_t ms_FrustumCullingThreadProcessor[6];
	};
};
