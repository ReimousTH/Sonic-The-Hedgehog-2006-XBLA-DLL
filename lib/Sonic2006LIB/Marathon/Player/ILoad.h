#pragma once
#include <Sox/Memory/IDestructible.h>
#include <Player/IPlugIn.h>
#include <DocMarathonImp.h>

#include <sox/RefCountObject.h>
#include <Player/Input/IListener.h>	
#include <Sox/Graphics/Frame.h>
#include <sox/StepableThread.h>

namespace Sonicteam{
	namespace Player{


		
		class ILoad:public Sonicteam::Player::IPlugIn
		{
		public:
			ILoad(void);
			~ILoad(void);

			MARATHON_DESTRUCTION_H(ILoad);;
			//r3 = RefCountObj r4 = this, r5 = std::string (player/select_sonic.pkg)

			virtual Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::RefCountObject> LoadPackage(std::string& package_path);
			//create link with out_input more like, put it to LinkNode list
			virtual Sonicteam::DocMarathonPlayerInput* ConnectListenerToInput(Sonicteam::DocMarathonPlayerInput* out_input,int index) = 0;
			virtual void AddFrame(Sonicteam::SoX::Graphics::Frame** PlayerFrame) = 0;

			//Sonicteam::SoX::Scenery::Clump (DRAWABLE models some stuff)
			virtual void ProcessClump01(Sonicteam::SoX::RefCountObject** obj) = 0; //no idea
			virtual void AddClump(Sonicteam::SoX::RefCountObject** obj) = 0; //no idea
			virtual void ProcessClump02_01(Sonicteam::SoX::RefCountObject** obj) = 0; //different clump type of 02 i guess
			virtual void AddShadowClump(Sonicteam::SoX::RefCountObject** obj) = 0; //different clump type of 02 i guess
			virtual void ILoadUnknown() = 0;
			virtual void ProcessClump02_03(Sonicteam::SoX::RefCountObject** obj) = 0; //different clump type of 02 i guess
			virtual void* GetParticleManager() = 0;
			virtual void* GetGraphicDevice() = 0; 

			//r4 (IDEK i say it like it)	
			virtual void SendMessageToSFXAgent(Sonicteam::SoX::RefCountObject**) = 0; 

			//r4 (put model threead to update)
			virtual void AddThread(Sonicteam::SoX::StepableThread*) = 0; 



		};

	}
}

