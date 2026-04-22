#pragma once

#include <Marathon.inl>

namespace Sonicteam
{
	class ObjInfo
	{
	public:
		ObjInfo(void);
		~ObjInfo(void);

		MARATHON_DESTRUCTION_H(ObjInfo);
		virtual bool Function4();
		virtual bool Function8();
	};
};
