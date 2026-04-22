#pragma once

#include <Marathon.inl>
#include <Sox/Thread.h>
#include <Sox/Graphics/Frame.h>

namespace Sonicteam
{
	class FrameSynchronizerThread : public SoX::Thread
	{
	public:
		FrameSynchronizerThread(Sonicteam::SoX::Graphics::FrameSynchronizer* pFrameSyncronizer, uint32_t Proccessor, bool IsSyncronize);
		~FrameSynchronizerThread();

		MARATHON_DESTRUCTION_H(FrameSynchronizerThread);
		virtual void Syncronize();
		virtual void Run();
		virtual void OnStep(double);

		Sonicteam::SoX::Graphics::FrameSynchronizer* m_pFrameSyncronizer;
		uint32_t m_SyncID;
	};
}