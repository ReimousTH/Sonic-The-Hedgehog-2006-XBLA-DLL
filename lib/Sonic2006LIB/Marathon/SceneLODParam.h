#ifndef SONICTEAM__SCENELODPARAM
#define SONICTEAM__SCENELODPARAM

#include <ISceneParam.h>
#include <xtl.h>
#include <Marathon.inl>

namespace Sonicteam
{
	class SceneLODParam : public Sonicteam::ISceneParam
	{
	public:

        struct LODParam
        {
            uint8_t m_IsSet;
            MARATHON_INSERT_PADDING(3);
            be<float> m_ClipDistance;
            be<float> m_FarDistance;  
        };

        /*
        enum LODEnumRetail
        {
            Main,
            MainXp,
            MainSky,
            MainPsi,
            MainOC,
            MainGlare,
            MainAfterPP,
            Light0,
            Light1,
            Light2,
            Light3,
            Reflection0,
            ReflectionSky,
            MainUser0,
            MainUser1,
            User0,
            User1,
            MainParticle,
            RaderMap
        };
        */

        enum LODEnum
        {
            Main,
            MainXp,
            MainSky,
            MainPsi,
            MainOC,
            MainAfterPP,
            Light0,
            Light1,
            Light2,
            Light3,
            Reflection0,
            ReflectionSky,
            MainUser0,
            MainUser1,
            User0,
            User1,
            MainParticle,
            RaderMap,
            Unknown, //MainGlare
        };

        SceneLODParam();
		~SceneLODParam();

        LODParam m_LODParam[19];


		MARATHON_DESTRUCTION_H(SceneLODParam);;
		virtual const char* GetParamName();

	};

};





#endif