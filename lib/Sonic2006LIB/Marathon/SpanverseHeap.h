#ifndef SONICTEAM__SPANVERSEHEAP
#define SONICTEAM__SPANVERSEHEAP

#include <Marathon.inl>
#include <Heap.h>

namespace Sonicteam{
	class SpanverseHeap:public Sonicteam::Heap{
			public:
			SpanverseHeap();
			~SpanverseHeap();



			void Initialize(void*);


			static SpanverseHeap* SpanInstance;
			static bool Initialized;
			MARATHON_DESTRUCTION_H(SpanverseHeap);;
			
	};
		
		//nothing new
	
};


#endif