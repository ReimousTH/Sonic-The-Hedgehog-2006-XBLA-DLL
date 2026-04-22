#pragma once

#include "IPlugIn.h"
#include "IVariable.h"
#include "IStepable.h"
#include <xtl.h> //replace to only XMFLOAT4
#include <Marathon.inl>

namespace Sonicteam{
	namespace Player{

		
		class IScore:public IPlugIn, public IVariable, public IStepable
		{
		public:
			IScore(void);
			~IScore(void);

			MARATHON_DESTRUCTION_H(IScore);;
			virtual void AddRings(uint32_t) = 0;
			virtual void TakeDamage(void) = 0;
			virtual void TakeDamageLittle() = 0;
			virtual uint32_t GetRings() = 0;
			virtual void SetRings(uint32_t) = 0;
			virtual void SetUnknownVector(XMFLOAT4*) = 0; //have no idea

	





		

		};

	}
}

