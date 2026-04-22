#include "Perfomance.h"
#include <Sox/Engine/Profiler.h>

using namespace Sonicteam::SoX;

Sonicteam::SoX::PerformanceFrequency::PerformanceFrequency()
{
	SoX::Engine::Profiler::QueryPerformanceFrequency(0);
}

PerformanceFrequency::~PerformanceFrequency()
{

}

void Sonicteam::SoX::PerformanceFrequency::QueryFrequency()
{
	SoX::Engine::Profiler::QueryPerformanceFrequency(0);
}

LARGE_INTEGER PerformanceFrequency::GetPerformanceFrequency()
{
	SoX::Engine::Profiler::QueryPerformanceFrequency(0);
	return *(LARGE_INTEGER*)&SoX::Engine::Profiler::ms_PerfomanceFrequency;
}

unsigned __int64 PerformanceFrequency::GetTimeBase()
{
	return __mftb();
}

//replace to unsigned __int64 (if R3 != counter ) (r3 = is counter all okay ;)) (original have optimize or some?)
LARGE_INTEGER PerformanceFrequency::PerfomanceScale(LARGE_INTEGER* counter)
{
	LARGE_INTEGER _return; _return.QuadPart = counter->QuadPart / Sonicteam::SoX::Engine::Profiler::ms_PerfomanceFrequency;
	return _return;
}

LARGE_INTEGER PerformanceFrequency::PerfomanceScaleEX(LARGE_INTEGER* counter)
{
	LARGE_INTEGER _return; _return.QuadPart = (counter->QuadPart * 0x000F4240)/ Sonicteam::SoX::Engine::Profiler::ms_PerfomanceFrequency;
	counter->QuadPart = _return.QuadPart;
	return _return;
}

void PerformanceFrequency::QPerfomanceCounterEx(LARGE_INTEGER* counter)
{
	counter->QuadPart = __mftb();
}

LARGE_INTEGER* PerformanceFrequency::QPerfomanceCounter(LARGE_INTEGER* counter)
{
	QueryPerformanceCounter(counter);
	return counter;
}

