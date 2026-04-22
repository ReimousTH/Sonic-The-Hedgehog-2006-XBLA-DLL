#ifndef SONICTEAM__PLAYER__ISTEPABLE
#define SONICTEAM__PLAYER__ISTEPABLE

#include <Marathon.inl>

namespace Sonicteam
{
	namespace Player
	{

		class IStepable
		{
		public:
			IStepable(void);
			~IStepable(void);


			MARATHON_DESTRUCTION_H(IStepable);;
			virtual void OnStepable(float) = 0;

		};

	}
}

#endif
