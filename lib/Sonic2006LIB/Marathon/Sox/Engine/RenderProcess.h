#pragma once

#include <Marathon.inl>
#include <list>
#include <string>
#include <boost/shared_ptr.hpp>
#include <Sox/LinkNode.h>

namespace Sonicteam
{
	namespace SoX
	{
		namespace Engine
		{
			class GTask;
			class RenderScheduler;
			class View;

			class RenderProcess
			{
			public:
				RenderProcess(void);
				~RenderProcess(void);

				MARATHON_DESTRUCTION_H(RenderProcess);
				virtual bool Func4();
				virtual void Func8();
				virtual void FuncC();

				uint32_t m_Field4;
				GTask* m_pGTask;
				uint32_t m_FieldC;
				uint32_t m_Field10;
				View* m_pView;
				float m_Field18; // delta-time ?
				MARATHON_INSERT_PADDING(4);
				RenderScheduler* m_pRenderScheduler;
				SoX::LinkNode<RenderProcess> m_lnRenderProcess;
			};
		};
	}
}
