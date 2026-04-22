#pragma once

#include <Marathon.inl>
#include <Patch/Api/FThread.h>
#include <Sox/Graphics/Frame.h>
#include <Boost/function.hpp>
#include <Boost/shared_ptr.hpp>

class FFrameSynchronizerThread : FThread
{
public:
	FFrameSynchronizerThread(Sonicteam::SoX::Graphics::FrameSynchronizer* pFrameSyncronizer, uint32_t Proccessor, bool IsSyncronize);
	~FFrameSynchronizerThread();

	MARATHON_DESTRUCTION_H(FFrameSynchronizerThread);
	virtual void Syncronize();
	virtual void Run();
	virtual void OnStep(double);

	Sonicteam::SoX::Graphics::FrameSynchronizer* m_pFrameSyncronizer;
	uint32_t m_SyncID;
	HANDLE m_StepFrameEventEndAfter;
};