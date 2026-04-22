#pragma once

#include <Marathon.inl>
#include <Patch/Api/FThread.h>
#include <Boost/function.hpp>
#include <Boost/shared_ptr.hpp>

struct FStepableThreadEmpty
{
	MARATHON_INSERT_PADDING(4);
};

class FStepableThread : public FThread
{
public:
	FStepableThread(boost::function<void()> OnStart, boost::function<void()> OnEnd, uint32_t Proccessor);
	~FStepableThread();

	MARATHON_DESTRUCTION_H(FStepableThread);
	virtual void Syncronize();
	virtual void Run();
	virtual void OnStep(double);

	boost::function<void()> m_OnStart;
	boost::function<void()> m_OnEnd;
	bool m_IsStepped; // not sure why then need it soo bad
	HANDLE m_StepFrameEventEndAfter;
};