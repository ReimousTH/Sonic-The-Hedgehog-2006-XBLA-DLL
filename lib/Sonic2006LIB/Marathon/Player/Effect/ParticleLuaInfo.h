#pragma once

#include <Marathon.inl>

namespace Sonicteam
{
	namespace Player
	{
		namespace Effect
		{
			struct ParticleLuaInfo
			{
				uint16_t BankID;
				uint16_t ParticleID;
				bool IsAcroarts;
				bool IsReverse;
				uint32_t Direction;
				const char* pNodeName;
				float OffsetX;
				float OffsetY;
				float OffsetZ;
			};
		};
	}
}

