#ifndef SONICTEAM__APPMARATHON
#define SONICTEAM__APPMARATHON
#include "Defs.h"
#include <Sox/Engine/Application.h>



namespace Sonicteam{


	class AppMarathon:Sonicteam::SoX::Engine::Application
	{

		AppMarathon();
		~AppMarathon();


		DocMarathonImp* m_pDoc;



		MARATHON_DESTRUCTION_H(AppMarathon);;

	};
};

#endif