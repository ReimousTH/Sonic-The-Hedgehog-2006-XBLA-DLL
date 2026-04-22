#ifndef SONICTEAM__PLAYER__LOAD
#define SONICTEAM__PLAYER__LOAD


#include <Marathon.inl>
#include "Defs.h"
#include <Sox/Graphics/Frame.h>
#include <Sox/Memory/IDestructible.h>
#include <Player/IPlugIn.h>
#include <Player/IExportExternalFlag.h>
#include <Sox/Engine/Doc.h>
#include <Sox/Memory/DEFINITIONS.hpp>

#include <map>
#include <Player/ILoad.h>
#include <string>


#include <memory>
#include <iostream>
#include <deque>


#include <Sox/RefCountObject.h>
#include <boost/shared_ptr.hpp>
#include <PackageBinary.h>
#include <Player/AnimationScript.h>

namespace Sonicteam
{
	namespace Player
	{

		class Load:public ILoad
		{
		public:
			Load(Sonicteam::SoX::Engine::Doc* doc);
			~Load(void);

			MARATHON_DESTRUCTION_H(Load);

			virtual Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::RefCountObject> LoadPackage(std::string& package_path);
			//create link with out_input more like, put it to LinkNode list
			virtual Sonicteam::DocMarathonPlayerInput* ConnectListenerToInput(Sonicteam::DocMarathonPlayerInput* out_input, int index);
			virtual void AddFrame(Sonicteam::SoX::Graphics::Frame** PlayerFrame);

			//Sonicteam::SoX::Scenery::Clump (DRAWABLE models some stuff)
			virtual void ProcessClump01(Sonicteam::SoX::RefCountObject** obj);; //no idea
			virtual void AddClump(Sonicteam::SoX::RefCountObject** obj) = 0; //no idea
			virtual void ProcessClump02_01(Sonicteam::SoX::RefCountObject** obj); //different clump type of 02 i guess
			virtual void AddShadowClump(Sonicteam::SoX::RefCountObject** obj); //different clump type of 02 i guess
			virtual void ILoadUnknown() = 0;
			virtual void ProcessClump02_03(Sonicteam::SoX::RefCountObject** obj); //different clump type of 02 i guess
			virtual void* GetParticleManager();
			virtual void* GetGraphicDevice();

			//r4 (IDEK i say it like it)	
			virtual void SendMessageToSFXAgent(Sonicteam::SoX::RefCountObject**);

			//r4 (put model threead to update)
			virtual void AddThread(Sonicteam::SoX::StepableThread*);

		};

	}
}




#endif


