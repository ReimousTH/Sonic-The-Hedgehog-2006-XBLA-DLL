#pragma once

#include <Marathon.inl>

namespace Sonicteam
{
	namespace SoX
	{
		namespace Engine
		{
			class Profiler
			{
			public:
				Profiler(void);
				~Profiler(void);
				static uint64_t ms_PerfomanceFrequency;
				static uint32_t TicksToMicroseconds(uint64_t);
				static uint64_t QueryPerformanceFrequency(uint64_t);
			};
		};
	}
}
