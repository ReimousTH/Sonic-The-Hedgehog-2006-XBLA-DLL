#pragma once

#include <Marathon.inl>
#include <Patch/Api/FThread.h>
#include <Sox/Physics/Havok/WorldHavok.h>
#include <Sox/RefSharedPointer.h>

class FHavokRunnerThread : public FThread
{
public:
	FHavokRunnerThread(Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Physics::Havok::WorldHavok>&);
	~FHavokRunnerThread();

	MARATHON_DESTRUCTION_H(FHavokRunnerThread);
	virtual void Syncronize();
	virtual void Run();
	virtual void OnStep(double);

	Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Physics::Havok::WorldHavok> m_spWorld;
};