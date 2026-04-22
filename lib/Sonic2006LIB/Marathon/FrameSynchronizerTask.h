#pragma once

#include <Marathon.inl>
#include <Sox/Graphics/Frame.h>
#include <Sox/Engine/Task.h>
#include <Sox/LinkNode.h>

namespace Sonicteam
{
	class FrameSynchronizerThread;

	class FrameSynchronizerTask : public SoX::Engine::Task
	{
	public:

		FrameSynchronizerTask(SoX::Engine::Task* pParentTask,SoX::Graphics::FrameSynchronizer* pSyncronizer);
		~FrameSynchronizerTask();

		MARATHON_DESTRUCTION_H(FrameSynchronizerTask);
		virtual char* GetType() override;
		virtual void OnStep(float) override;
		void __KernelAddThread(FrameSynchronizerThread*);
		void __KernelStepThreads(double);
		void __KernelSynchronizeThreads();

		SoX::Graphics::FrameSynchronizer* m_pSynchronizer;
		SoX::LinkedListExtended<FrameSynchronizerThread> m_llThreads;
		FrameSynchronizerThread* m_apThreads[6];
	};

};
