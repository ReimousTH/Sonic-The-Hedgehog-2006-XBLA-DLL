#ifndef SONICTEAM__DOCMARATHONIMP
#define SONICTEAM__DOCMARATHONIMP
#include "Defs.h"

#include <Sox/Scenery/World.h>
#include <vector>
#include <Sox/Engine/Doc.h>
//#include <Sox/Graphics/FrameSynchronizer.h>
#include <Sox/Graphics/Frame.h>
#include <Sox/Engine/View.h>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <Sox/IResource.h>
#include <SceneCollection.h>
#include <Sox/Thread.h>
#include <boost/function.hpp>
#include <Sox/Input/Defs.h>
#include <SoX/Audio/Player.h>


namespace Sonicteam
{
	struct DocMarathonImpVecFlags{
		unsigned int Flag0;
		unsigned int Flag1;
		unsigned int Flag2;
		unsigned int Flag3;

	};
	//not full
	struct DocMarathonPlayerInput{
		unsigned int Index;
		unsigned int PInput;
	};


	//not complete :(

	class DocMarathonImp : public Sonicteam::SoX::Engine::Doc
		{
		public:

		
			DocMarathonImp(unsigned int); //ptr SonicTeamEngineApplication
			~DocMarathonImp(void);
		

			MARATHON_DESTRUCTION_H(DocMarathonImp);
			virtual void Update(double);
			virtual void UpdateRender();
			virtual unsigned int DocGetMyGraphicDevice();
			virtual Sonicteam::SoX::Graphics::FrameSynchronizer* DocGetFrameSynchronizer();
			virtual boost::shared_ptr<void*> DocDoUnkAction01(unsigned int,unsigned int); //8204ACD8
			virtual boost::shared_ptr<Sonicteam::SoX::Scenery::World> DocGetWorld(unsigned int); //8204ACDC Doculist
			virtual std::vector<unsigned int>* DocGetDoculist(); //8204ACE0 Doculist
			virtual unsigned int DocDoUnkAction02(unsigned int); //8204ACD8 (Get Boot)
			virtual unsigned int DocDoUnkModules(); //8204ACD8 (Get Boot)
			virtual unsigned int GetPlayerInput(unsigned int); //need complete struct ,index
			virtual DocMarathonImpVecFlags* GetDocImpFlag(); //need complete struct ,index
			virtual unsigned int GSDocFrameSynchronizer(unsigned int); //need complete struct ,index
			virtual void QueueControllerInputByIndex(Sonicteam::SoX::Input::Listener* listener,size_t index); //need complete struct ,index
			virtual unsigned int DocGetCurrentCase(); //need complete struct ,index
			virtual unsigned int DocGetPreviousCase(); //need complete struct ,index
			virtual unsigned int DocGetSFXAgent(); //need complete struct ,index
			virtual unsigned int DocGetImageFilterCollection(); //need complete struct ,index
			virtual unsigned int DocGetPostprocessController(); //need complete struct ,index
			virtual unsigned int DocGetCurrentMessage(); //need complete struct ,index
			virtual boost::shared_ptr<SoX::Audio::Player> DocGetAudioPlayerImp(); //need complete struct ,index
			virtual unsigned int DocSetCurrentMode(unsigned int); //need complete struct ,index
			virtual unsigned int DocGetDebug3DFont(); //need complete struct ,index
			virtual Sonicteam::SceneParamManager* DocGetSceneParamManager(); //need complete struct ,index
			virtual unsigned int DocGetUnkManagers(); //need complete struct ,index
			virtual unsigned int DocGetUnkGameResources(); //need complete struct ,index
			virtual void DocNull01() {};
			virtual void DocNull02() {};
			virtual unsigned int DocGetUnkFloatParam01(); //need complete struct ,index
			virtual unsigned int DocResetUnkLoadingFlag01(); //need complete struct ,index
			virtual unsigned int DocGetUnkLoadingFlag02(); //need complete struct ,index
			virtual unsigned int DocResetUnkLoadingFlag02(); //need complete struct ,index
			virtual unsigned int DocGetUnkLoadingFlag01(); //need complete struct ,index
			virtual void DocNull03() {};
			virtual void DocNull04() {};
			virtual void DocNull05() {};
			virtual unsigned int DocResetUnkFlag01(); //need complete struct ,index
			virtual unsigned int DocPauseGame(unsigned char); //need complete struct ,index
			virtual unsigned int DocLoadScene(const char*); //need complete struct ,index
			virtual unsigned int DocGetuintgap0x114(); //need complete struct ,index
			virtual unsigned int DocGetuintgap0x234(); //need complete struct ,index
			virtual unsigned int DocChangeGameSpeed(unsigned char,double); //need complete struct ,index
			virtual unsigned int DocIsVFrameEnabled(); //need complete struct ,index
			virtual unsigned int DocChangeVFrameState(unsigned char,const char*); //need complete struct ,index
			virtual unsigned int DocCopy0x360(unsigned int); // copy element ptr
			virtual unsigned int DocGetGameRuleContext(); //need complete struct ,index
			virtual unsigned int DocSetGameRuleContext(unsigned int); //need complete struct ,index
			virtual Sonicteam::SoX::Engine::Task* DocGetTask(size_t);
			virtual unsigned int DoGetcuint0xFC(); //need complete struct ,index
			virtual unsigned int DoGetcuint0xF8(); //need complete struct ,index
			virtual unsigned int DocChangeSpeedState(unsigned char); //need complete struct ,index
			virtual unsigned int DocResetUnkFlag02(); //need complete struct ,index
			virtual unsigned int DocGetUnkFlag02(); //need complete struct ,index
			virtual unsigned int DocResetOnUnkFlag02(); //need complete struct ,index
			virtual void DocNull06() {};
			virtual void DocNull07() {};
			virtual void DocNull08() {};
			virtual void DocNull09() {};
			virtual void DocNull10() {};
			virtual void DocNull11() {};
			virtual unsigned int DocGetUnkStaticFlag01(); //need complete struct ,index
			virtual unsigned int DocSetStringFlags01(std::string*); //need complete struct ,index
			virtual unsigned int DocSetStringFlags02(std::string*); //need complete struct ,index
			virtual void DocNull12() {};
			virtual unsigned int DocGetUnkStaticFlag02(); //need complete struct ,index
			virtual unsigned int DocGetUnkStaticFlag02_1(); //need complete struct ,index
			virtual unsigned int DocProcessSubtitleFlag(char*,unsigned int); //compare to
			virtual unsigned int DocProcessCheckSubtitleFlag(char*); //compare to
			virtual const char* DocGetLanguage(); //need complete struct ,index
			virtual unsigned int DocGetuint0x3BC(); //need complete struct ,index
			virtual unsigned int DocGetuint0x3BC_1(); //need complete struct ,index
			virtual unsigned int DocGetuint0x2AFCC(); //need complete struct ,index
			virtual unsigned int DocGetuint0x2AFCC_1(); //need complete struct ,index
			virtual unsigned int DocGetuint0x55BDC(); //need complete struct ,index
			virtual unsigned int DocGetuint0x55BDC_1(); //need complete struct ,index
			virtual unsigned int DocGetuint0x55C24(); //need complete struct ,index
			virtual unsigned int DocGetuint0x55C24_1(); //need complete struct ,index
			virtual unsigned int SetP1RealControllerID(unsigned int); //need complete struct ,index
			virtual unsigned int GetP1RealControllerID(); //need complete struct ,index
			virtual unsigned int SetRealControllerID(unsigned int,unsigned int); //need complete struct ,index
			virtual unsigned int GetRealControllerID(unsigned int); //need complete struct ,index

			virtual unsigned int DocGet0x55C54();
			virtual unsigned int DocSet0x55C54(int value);
			virtual unsigned int DocSet0x55C58(char flag);
			virtual unsigned int DocGet0x5C5C();
			virtual unsigned int DocSet0x5C5C(int v);
			virtual unsigned int DocProcessShaderMode(int mode);



			//field
			Sonicteam::SoX::Engine::Task* DocCurrentDocMode; //MainMode (0x5C) But ... StateGamemode ..... (DocMode)
			unsigned int DocMyGraphicDevice; //0x60
			Sonicteam::SceneParamManager* DocSceneParamManager; //0x64
			//either vector ptr or lost structure ? (max size = 0x20)

			boost::shared_ptr<SceneCollection<Sonicteam::SoX::Scenery::World>> WorldCollection; //0x68-0x6C

			unsigned int Docuint0x70;
			unsigned int Docuint0x74;
			unsigned int Docuint0x78;
			unsigned int Docuint0x7C;
			//and soo onnn later
			Sonicteam::SoX::Graphics::FrameSynchronizer DocFrameSynchronizer; //0x80



			//Sonicteam::SoX::Input::Manager(no vft, finally usefull) 825863D8 (constructor) 

			std::vector<boost::shared_ptr<Sonicteam::SoX::Input::Manager>> m_vspInputManager; //0x9C


			Sonicteam::SoX::Engine::View DocEngineView; //AC
			DocMarathonImpVecFlags DocImpFlag; //0xB8
			boost::shared_ptr<SoX::Audio::Player> DocAudioPlayerImp; //PTR
			unsigned char IsVFrameEnabled; //Frame skip
			unsigned char Docuint0xD1; //unknown param (Freeze game?????)
			unsigned char NotUseParticle; //Frame skip  D2
			unsigned char EnableGe1Particle; //Frame skip D3
			unsigned int CurrentCase; //maybe
			unsigned int PreviousCase; //naybe
			unsigned int DocSFXAgent; // (82635BC8 later)
			unsigned int DocImageFilterCollection; //82633088
			unsigned int DocPostprocessController; //8262A140 E4
			unsigned int m_DocSpeed;
			unsigned int DocUnkFlag01;
			unsigned int Docuint0xF0;
			unsigned int DocCurrentMessage; //maybe (F4)

			LinkSoxNode<Sonicteam::SoX::Thread>* LinkThreadRoot; //0xF8 :MyStepableThread, HavokRunnerThread YES LINKSOXNODE pointer manuall relese (full)

			unsigned int Docuint0xFC;
			unsigned int Docuint0x100;
			unsigned int Docuint0x104;
			unsigned int DocDebug3DFont; //0x108
			boost::shared_ptr<unsigned int> MyGraphicsCommonFactory; //0x10C
			int field_114;
			int field_118;
			int field_11C;
			int field_120;
			int field_124;
			int field_128;
			int field_12C;
			int field_130;
			int field_134;
			int field_138;
			int field_13C;
			int field_140;
			int field_144;
			int field_148;
			int field_14C;
			int field_150;
			int field_154;
			unsigned int field_158;
			int field_15C;
			int field_160;
			int field_164;
			int field_168;
			int field_16C;
			int field_170;
			int field_174;
			int field_178;
			int field_17C;
			int field_180;
			int field_184;
			int field_188;
			int field_18C;
			MARATHON_INSERT_PADDING(0x58);
	};
};

#endif