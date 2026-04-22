#pragma once
#include <string>
#include <Marathon.inl>



namespace Sonicteam{

	namespace Player{

		class IPlugIn
		{
		public:
			IPlugIn(const char*);
			~IPlugIn(void);

			MARATHON_DESTRUCTION_H(IPlugIn);

			std::string PluginName;

		};
	};
};

