#include "Profiler.h"

using namespace Sonicteam::SoX::Engine;

uint64_t Profiler::ms_PerfomanceFrequency = 0;

uint32_t Profiler::TicksToMicroseconds(uint64_t ticks)
{
	return (ticks * 0x000F4240) / Profiler::ms_PerfomanceFrequency;
}

uint64_t Profiler::QueryPerformanceFrequency(uint64_t perfomanceFrequency)
{
	::QueryPerformanceFrequency((LARGE_INTEGER*)&Profiler::ms_PerfomanceFrequency);
	return perfomanceFrequency;
}

Profiler::Profiler()
{

}

Profiler::~Profiler()
{

}
