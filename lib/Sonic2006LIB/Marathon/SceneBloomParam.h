#ifndef SONICTEAM__SCENEBLOOMPARAM
#define SONICTEAM__SCENEBLOOMPARAM


#include <ISceneParam.h>

namespace Sonicteam{




	class SceneBloomParam:public Sonicteam::ISceneParam{
	public:
		SceneBloomParam();
		~SceneBloomParam();

		float MinThreshold; //14
		float MaxThreshold; //18
		float Scale; //1C
	


		MARATHON_DESTRUCTION_H(SceneBloomParam);;
		virtual const char* GetParamName();

	};

};





#endif