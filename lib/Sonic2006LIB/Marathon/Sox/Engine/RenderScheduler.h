#pragma once

#include <Marathon.inl>
#include <list>
#include <string>
#include <boost/shared_ptr.hpp>

namespace Sonicteam
{
	namespace SoX
	{
		namespace Engine
		{
			class RenderProcess;
			class View;

			class RenderScheduler
			{
			public:
				RenderScheduler(void);
				~RenderScheduler(void);
				MARATHON_DESTRUCTION_H(RenderScheduler);
				virtual void Update(View*, double);
				std::list<std::pair<std::string, boost::shared_ptr<RenderProcess>>> m_lRenderProcesses;
			};
		};
	}
}
