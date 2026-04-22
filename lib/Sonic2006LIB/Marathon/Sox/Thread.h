#pragma once

#include <Marathon.inl>
#include <SoX/LinkNode.h>

namespace Sonicteam
{
	namespace SoX
	{

		class Thread 
		{
        public:
			SoX::LinkNode<Thread> m_lnThread;
            HANDLE m_StepFrameEvent; //0x10
            HANDLE m_StepFrameEventEnd; //0x14
            uint32_t m_ID;        //0x18 // ThreadID
            HANDLE m_Handle;  // 0x1C   Assuming HANDLE is used for thread identifier
			bool m_IsRunned; //0x20
			float m_DeltaTime; //Performance //0x24
			uint32_t m_ParentList; //0x28
			uint32_t m_StepFrameCount; //0x2c Perfomance scale * 0.00001 = delta time
			const char* m_Name;
			uint32_t m_StepTime ; //0x34 (miliseconds, Step Time )
			bool m_IsSuspended;//0x38
			bool m_IsSyncronize; //0x39
			uint32_t m_StepEndTime; //0x3C

        public:
            Thread(const char* Name,uint32_t Processor,bool IsSyncronize);
			~Thread();

			MARATHON_DESTRUCTION_H(Thread);
			virtual void Syncronize();
			virtual void Run();
			virtual void OnStep(double);

			void __ThreadKernelStep(Thread*);
			void Join();
		};

	};
};