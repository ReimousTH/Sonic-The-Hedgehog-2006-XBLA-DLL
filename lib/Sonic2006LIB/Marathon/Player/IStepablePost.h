#ifndef SONICTEAM__PLAYER__ISTEPABLEPOST
#define SONICTEAM__PLAYER__ISTEPABLEPOST




namespace Sonicteam{
	namespace Player{

		class IStepablePost
		{
		public:
			IStepablePost(void);
			~IStepablePost(void);


			MARATHON_DESTRUCTION_H(IStepablePost);;
			virtual void OnStepablePost(float) = 0;

		};

	}
}


#endif