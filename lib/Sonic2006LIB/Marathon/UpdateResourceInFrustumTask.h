#pragma once

#include <Marathon.inl>
#include <UpdateResourceInFrustumThread.h>
#include <Sox/Engine/Task.h>
#include <Sox/LinkNode.h>

namespace Sonicteam
{
	class DocMarathonImp;

	class UpdateResourceInFrustumTask : public SoX::Engine::Task
	{
	public:
		UpdateResourceInFrustumTask(Task*, DocMarathonImp*);
		~UpdateResourceInFrustumTask(void);

		MARATHON_DESTRUCTION_H(UpdateResourceInFrustumTask);
		virtual void OnStep(float);


		DocMarathonImp* m_pDoc; // 0x4C
		SoX::LinkedList<UpdateResourceInFrustumThread> m_llThreads;
		UpdateResourceInFrustumThread* m_apThreads[5]; // 0x60
		MARATHON_INSERT_PADDING(4);
		uint32_t m_ThreadsCount;
	};
};
