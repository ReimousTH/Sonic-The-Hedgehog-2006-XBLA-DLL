#pragma once

#include <Sox/Engine/Defs.h>
#include "../MessageReceiver.h"
#include "GTask.h"
#include <xtl.h>
#include <Sox/Engine/Task.h>
#include <Sox/RefCountObject.h>
#include <Marathon.inl>

namespace Sonicteam
{

	namespace SoX
	{
		namespace Engine
		{
			class Task;
			class DocMode;
			class Application;
			class RenderScheduler;

			namespace
			{

				class RootTask : public Sonicteam::SoX::Engine::Task 
				{
				public:
					virtual int OnMessageRecieved(Sonicteam::SoX::IMessage*);
					virtual char* GetType(); //return change a later
					virtual void OnStep();
				};
				class RootGTask :Sonicteam::SoX::Engine::GTask {
				public:
					virtual char* GetType(); //return change a later

				};
				class DocModeExecutor :Sonicteam::SoX::Engine::Task {
				public:
					virtual int OnMessageRecieved(Sonicteam::SoX::IMessage*);
					virtual char* GetType(); //return change a later 
					virtual void OnStep();
				};
			};

			class Doc
			{

			public:
				~Doc(void);

				MARATHON_DESTRUCTION_H(Doc);
				virtual void Update(double);
				virtual void UpdateRender();

				void __KernelUpdateRootTask(double);
				void __KernelUpdateRootGTask(double);
				void __KernelUpdateRootGTask();

				bool m_Field4;
				DocMode* m_pDocMode; //CurrentMode (GameMode,MainMode)
				RootTask* m_pRootTask; //0xC
				RootGTask* m_pRootGTask;
				Application* m_pApplication; //ApplicationMarathon (reference &)
				View* m_pView;
				DocModeExecutor* m_pModeExecutor;
				RenderScheduler* m_RenderScheduler;
				CRITICAL_SECTION m_pTasksSyncronizer;
				CRITICAL_SECTION m_pGraphicsSynronizer;

				template <typename T>
				inline T* GetDocMode()
				{
					return (T*)m_pDocMode;
				}

			};
		};
	}
}
