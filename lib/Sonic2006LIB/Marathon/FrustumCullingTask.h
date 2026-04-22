#pragma once

#include <Marathon.inl>
#include <FrustumCullingThread.h>
#include <Sox/Engine/Task.h>
#include <Sox/LinkNode.h>

namespace Sonicteam
{
	class DocMarathonImp;

	class FrustumCullingTask : public SoX::Engine::Task
	{
	public:
		FrustumCullingTask(Task*, DocMarathonImp*);
		~FrustumCullingTask(void);

		MARATHON_DESTRUCTION_H(FrustumCullingTask);

		DocMarathonImp* m_pDoc; // 0x4C
		SoX::LinkedList<FrustumCullingThread> m_llThreads;
		FrustumCullingThread* m_apThreads[5]; // 0x60
		MARATHON_INSERT_PADDING(4);
		uint32_t m_ThreadsCount;
	};
};
