
#ifndef Sonicteam_GameImp
#define Sonicteam_GameImp

#include "Defs.h"
#include <Marathon.inl>

#include <vector>
#include <xmath.h>
#include <string>


#include <Stage/Object.h>
#include <Sox/Engine/Task.h>
#include <Sox/MessageReceiver.h>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <DocMarathonImp.h>
#include <CsdObject.h>
#include <Sox/IResource.h>
#include <ActorManager.h>
#include <Player/Load.h>
#include <ActorManager.h>
#include <NamedActor.h>



#include <Prop/ActivationMgr.h>
#include <Prop/PropClass.h>
#include <Prop/Manager.h>
#include <Prop/Scene.h>
#include <Prop/GenerateArea.h>
#include <Prop/ActiveArea.h>
#include <Prop/Manager.h>
#include <Prop/IActorCreator.h>
#include <Prop/ActorCreators.h>
#include <Prop/Library.h>
#include <Sox/Scenery/World.h>
#include <Sox/Audio/Cue.h>
#include <Sox/Audio/SoundBank.h>
#include <SplinePath/Scene.h>
#include <Mission/Core.h>
#include <Particles/ParticleContainer.h>
#include <Sox/Physics/World.h>
#include <TextBook.h>
#include <EventPlaybook.h>
#include <ObjInfo.h>
#include <Sox/Physics/CollisionFilter.h>

#define  GAMEIMP_START_LEVEL 1
#define  GAMEIMP_START_CUTSCENE 2
#define  GAMEIMP_START_CGI_CUTSCENE 3
#define  GAMEIMP_STOP_LEVEL 4
#define  GAMEIMP_STOP_CUTSCENE 5
#define  GAMEIMP_SAVEDATA 7
#define  GAMEIMP_DESTROY_ALL 9


namespace Sonicteam
{

	class TaskAdapter;
	class PauseAdapter;
	class ImpactManager;
	class GameScript;
	class MainDisplayTask;
	class EnemyLifeTask;
	class ObjInfoMgr;
	class ObjInfo;
	class KeyframedPathScene;
	class MeshPathScene;
	class MeshPathManager;
	class KeyframedPathManager;
	class HavokRunnerThread;
	class TextBook;
	class EventPlaybook;
	class PhysicsMaterialMgr;
	class RaderMapManager;
	class RaderMapIcon;
	class PopupScreenTask;
	class MyCollisionFilter;

	struct SetType{
		enum ST_ENUM{
			Default,
			Design,
			Particle,
			Test,
			Area,
			PathObj
		};
	};


	struct GameImpStruct3200{
		char buffer[0x3200];
	};

	class GameImp : Sonicteam::SoX::MessageReceiver
	{
	public:

		enum GameState : uint32_t
		{
			GameState_MainMenu,
			GameState_Stage,
			GameState_Event,
			GameState_Movie,
			GameState_Result,
			GameState_Message,
			GameState_6,
			GameState_Save,
			GameState_ReturnToMainMenu,
			GameState_Destroy,
		};

		struct Character 
		{
			std::string PostLuaFile;
			XMVECTOR PostRotation; // 0x20
			XMVECTOR PostPosition; // 0x30
			bool PostIsAmigo;
			std::string CurrentLuaFile; // choose this lua after restart?:/death
			XMVECTOR CurrentRotation; // 0x60
			XMVECTOR CurrentPosition; // 0x70
			XMMATRIX Transform; // Not Used
			bool CurrentIsAmigo; // 0xC0
	
		};
		
		// sizeof(48)
		struct GCharacter
		{
			uint32_t ActorID;
			uint32_t RingCount;
			MARATHON_INSERT_PADDING(4);
			uint32_t LifeCount;
			uint32_t ScoreCount;
			float AliveTime;
			float Time;
			MARATHON_INSERT_PADDING(4);
			float SectionTime;
			float GaugeValue;
			uint32_t MaturityLevel;
			float MaturityValue;
			MARATHON_INSERT_PADDING(4);
			uint32_t ExtendRingCount;
			uint32_t GemIndex;
			MARATHON_INSERT_PADDING(0x8);
		};

		~GameImp();
		GameImp();



		uint32_t				m_PostState;
		uint32_t				m_CurrentState;

		Sonicteam::DocMarathonImp*	m_pDoc;
		uint32_t				m_Flags; 
		uint32_t				m_Field14;
		GameImpStruct3200*		m_pField18;

		uint32_t				m_Field1C;
		uint32_t				m_Field20;
		uint32_t				m_Field24;
		uint32_t				m_Field28;
		bool					m_Field2C;
		Character m_CharacterData[0xF]; //0x30
		GCharacter m_PlayerData[0x4]; //0xC60
		GCharacter m_PlayerData2[0x4]; //0xD70


		Sonicteam::TaskAdapter* m_pTaskAdapter; // 0xE80
		uint32_t m_aCameraActorIds[4]; // 0xE84
		Sonicteam::PauseAdapter* m_pPauseAdapter; //0xE94
		Sonicteam::ImpactManager* m_pImpactManager; //0xE98
		std::string m_Area; // 0xE9C , wap/shadow/a


		//0xEB8
		struct
		{
			union
			{
				Sonicteam::CsdObject* value[3];
				struct
				{
					Sonicteam::CsdObject* CSDPausemenu;
					Sonicteam::CsdObject* CSDEnemyPowergauge;
					Sonicteam::CsdObject* CSDTalkwindow;
				};

			};
			Sonicteam::CsdObject* operator[](int index) {
				if (index > 3) return 0;
				return (value)[index];
			}
				
		} m_BaseHUD;


		boost::shared_ptr<GameImp> m_spGame; //0xEC4
		std::string m_MissionFile; // 0xECC Lua file, disposed after load
		MARATHON_INSERT_PADDING(0x40); // EE8
		bool m_FieldF28; // F28
		bool m_IsStageMission; // F29
		bool m_IsStageLoaded; // F2A
		bool m_FieldF2B; // F2B
		uint32_t m_FieldF2C; // F2C
		Sonicteam::GameScript* m_pGameScript; // F30
		uint32_t m_aCharacterActorIds[0xF]; //F34
		boost::shared_ptr<Sonicteam::ActorManager> m_spActorManager; //0xF70
		std::vector<std::vector<boost::shared_ptr<SoX::Scenery::Camera>>> m_vvspCameras; // 0xF78
		std::vector<boost::shared_ptr<Sonicteam::SoX::Scenery::World>> m_vspWorlds; // 0xF88
		Sonicteam::Stage::Object* m_pStageObject; //0xF98
		Sonicteam::SoX::LinkRef<Sonicteam::MainDisplayTask> m_alrMainDisplayTasks[4]; // 0xF9C

		boost::shared_ptr<Sonicteam::Prop::ActivationMgr> m_spActivationMgr; // 0xFDC
		boost::shared_ptr<Sonicteam::Prop::Manager> m_spPropManager; // 0xFE4


		struct KhronosLibrary
		{
			enum
			{
				Camera,
				ObjCommon,     
				Kingdomvalley, 
				Crisiscity,  
				Whiteacropolis,
				Dustydesert, 
				Tropicaljungle,
				Radicaltrain, 
				Flamecore,   
				Town,      
				Waveocean,     
				Aquaticbase,   
				Endoftheworld
			};
			Sonicteam::Prop::Library* values[0xD];
			Sonicteam::Prop::Library* operator[](uint32_t index)
			{
				return values[index];
			}
		} m_PropLibrary; //0xFEC


		struct KhronosSets
		{
			enum
			{
				Default,
				Design,
				Particle,
				Test,
				Area,
				PathObj
			};

			std::string values[6]; //0x12B
			std::string* operator[](uint32_t index)
			{
				return &values[index];
			}
		} m_PropSets; //0x1020

	
		boost::shared_ptr<Sonicteam::Prop::Scene> m_spPropScene[6];	// 0x10C8 						
		boost::shared_ptr<uint32_t> GameKynapseSystem; //Sonicteam::KynapseSystem  //0x10F8
		Sonicteam::SoX::IResource* GameKyWorldDef; //Sonicteam::KyWorldDef //0x1100
		MARATHON_INSERT_PADDING(4);
		Sonicteam::SoX::Audio::Cue* m_pMyCueAdx; //8204D19C  //0x1108
		Sonicteam::SoX::Audio::SoundBank* m_pSystemSounds; //0x8204D120  //0x110C
		Sonicteam::SoX::Audio::SoundBank* m_pBGMSounds; //0x8204D120  //0x1110
		Sonicteam::SoX::Audio::SoundBank* m_pObjCommonSounds; //0x8204D120  //0x1114
		Sonicteam::SoX::Audio::SoundBank* m_pStageSounds; //0x8204D120  //0x1118 //sound/stage_twn

		// Missing for XBLA
		//Sonicteam::SoX::Audio::SoundBank* m_pVoiceAllSounds; //0x8204D120  //0x1118
		//Sonicteam::SoX::Audio::SoundBank* m_pStageVoiceSounds; //0x8204D120  //0x1120 //sound/voice_twn_e

		Sonicteam::LuaSystem* m_pCameraLua; // 0x111C
		Sonicteam::SoX::LinkRef<Sonicteam::EnemyLifeTask> m_lrEnemyLifeTask; //0x1120
		Sonicteam::ObjInfoMgr* m_pObjInfoMgr; //0x1130
		Sonicteam::ObjInfo* m_pObjEnemyShotInfo; //0x1134
		Sonicteam::ObjInfo* m_pObjExplosionInfo; //0x1138
		Sonicteam::ObjInfo* m_pObjChaosDrieInfo; //0x113C
		Sonicteam::ObjInfo* m_pObjRingInfo; //0x1140

		std::vector<Sonicteam::ObjInfo> m_vObjInfo; //0x1144

		Sonicteam::NamedActor* m_pRootTask; //0x1154
		Sonicteam::NamedActor* m_pDependGame; //0x1158
		Sonicteam::NamedActor* m_pEntities; //0x115C
		Sonicteam::NamedActor* m_pEnemyThread; //0x1160
		Sonicteam::NamedActor* m_pPlayers; //0x1164
		Sonicteam::NamedActor* m_pCameras; //0x1168
		Sonicteam::NamedActor* m_pSystemDependCameras; //0x116C
		Sonicteam::NamedActor* m_pDependCameras; //0x1170

		// 0x1174
		struct KhronosPath
		{
			enum 
			{
				Default,
				Grind,
				Guide,
				PathObj,
				PhysicsPath
			};
			std::string values[5];  
			std::string* operator[](uint32_t index) 
			{
				return &values[index];
			}
		} m_aStagePaths;

		// 0x1200
		struct KhronosPathResource
		{
			enum 
			{
				Default,
				Grind,
				Guide,
				PathObj,
				PhysicsPath
			};
			
			Sonicteam::SoX::RefSharedPointer<Sonicteam::SplinePath::Scene> values[5];
			Sonicteam::SoX::RefSharedPointer<Sonicteam::SplinePath::Scene> operator[](int index) 
			{
				return values[index];
			}
		} m_aStageSplinePaths;

		// 0x1214
		struct KhronosKeyFrame
		{
			enum 
			{
				Default,
				Grind,
				Guide,
				PathObj,
				PhysicsPath
			};
			boost::shared_ptr<Sonicteam::KeyframedPathScene> values[5]; 
			boost::shared_ptr<Sonicteam::KeyframedPathScene>* operator[](int index) 
			{
				return &values[index];
			}
		} m_aStageKeyFramePaths;


		boost::shared_ptr<Sonicteam::MeshPathScene> m_spGrindMeshPathScene; // 0x123C
		boost::shared_ptr<Sonicteam::MeshPathScene> m_spGuideMeshPathScene; // 0x1244

		boost::shared_ptr<Sonicteam::MeshPathManager> m_spMeshPathManager; // 0x124C
		boost::shared_ptr<Sonicteam::KeyframedPathManager> m_spKeyframedPathManager; // 0x1254

		boost::shared_ptr<Sonicteam::Player::Load> m_spPlayerLoad; // 0x125C

		// 0x1264
		struct KhronosEnvironment
		{
		public:
			Sonicteam::HavokRunnerThread* pHavokRunnerThread;
			struct
			{
			public:
				std::string CameraParam; //placement/kdv/cameraparam_sonic_c.lua  //0x152C //0x0
				std::string RenderScript; //scripts/render/stage/render_kdv_c.lua //  0x1548 - 0x1C, 
				std::string SceneParams; //scripts/stage/kdv/scene_kdv_c.lua , get empty after load i guess :) //0x1564 - 0x38
				std::string Light;  //  ,  //0x1580 - 0x54
				std::string EnvMap;  //  ,  //0x159C - 0x70
				std::string Terrain;  //  stage/kdv/c/ ,  //0x15B8 - 0x8C
				std::string Sky;  //  ,  //0x15D4 - 0xA8
				std::string AreaName;  //  ,  //0x15F0 - 0xC4
				std::string unk0xE0;  //  ,  //0x160C - 0xE0 //Old Default Path
				std::string Kynapse;  //  ,  //0x1628 - 0xFC
				std::string TownsManInfo; // , 	 //0x1644 - 0x118
				std::string Radarmap; // , 	 //0x1660 - 0x134 (Not Sure)
				// Not Sure Which of them missing //
				//std::string BGM; // , stg_kdv_c 	 //0x167C - 0x150  (BGM)
				//std::string ObjectPhysics; // , 	 //0x1698 - 0x16C
				//std::string Explosion; // , 	 //0x16B4 - 0x188
				//std::string Enemy; // , 	 //0x16D0 - 0x1A4
				//std::string Shot; // , 	 //0x16EC - 0x1C0
			} LuaEnvironment;

		} m_StageEnvironment;

		//0x1708
		struct
		{
			enum
			{
				msg_hint,
				msg_hint_xenon
			};
			Sonicteam::SoX::RefSharedPointer<Sonicteam::TextBook> values[2]; 	

			Sonicteam::SoX::RefSharedPointer<Sonicteam::TextBook> operator[](uint32_t index)
			{
				return values[index];
			}
		} m_aStageTextBooks;

		uint32_t m_Field13C0;
		Sonicteam::Mission::Core* m_pMissionCore; // 0x13C4

		std::string m_Field13C8; // 0x13C8
		std::string m_Field13E4; // 0x13E4

		Sonicteam::SoX::LinkRef<uint32_t> m_lrField1400;
		std::string m_Field1410; // 0x1410 
		std::string m_Field142C; // 0x142C	 
		std::string m_Field1448; // 0x1448	 
		MARATHON_INSERT_PADDING(0x34);
	

		Sonicteam::SoX::LinkRef<uint32_t> m_lrField1498; // 0x1498

		// 0x14A8
		struct KhronosResult
		{
			std::string ID; 
			MARATHON_INSERT_PADDING(0x34);
			std::string ResultID; // 0x54
		} m_StageResult;

		MARATHON_INSERT_PADDING(0x10); // 0x1514
		Sonicteam::SoX::RefSharedPointer<Sonicteam::Particles::ParticleContainer> m_spStageParticleContainer; // 0x1524
		std::vector<Sonicteam::SoX::RefSharedPointer<Sonicteam::Particles::ParticleContainer>> m_vspParticleContainers; // 0x1528
		Sonicteam::SoX::RefSharedPointer<Sonicteam::EventPlaybook> m_spStageEventPlayBook; // 0x1538
		MARATHON_INSERT_PADDING(0x14); // 0x153C
		std::string m_Field1550; // 0x1550
		MARATHON_INSERT_PADDING(0xC); // 0x156C (Either at 0x8, Prop:: ??? no named class)
		Sonicteam::PhysicsMaterialMgr* m_pPhysicsMaterialMgr; // 0x1578
		Sonicteam::RaderMapManager* m_pRaderMapManager; // 0x157C
		Sonicteam::RaderMapIcon* m_pRaderMapIcon; // 0x1580

		Sonicteam::SoX::LinkRef<Sonicteam::PopupScreenTask> m_pPopupScreenTask; //1584

		Sonicteam::SoX::LinkRef<uint32_t> m_lrField1594; //0x1594
		Sonicteam::SoX::LinkRef<uint32_t> m_lrField15A4; //0x15A4
		HANDLE m_Handle15B4; // 0x15B4


		Sonicteam::SoX::RefSharedPointer<Sonicteam::Prop::ActiveArea> m_pStagePropActiveArea[4]; // 0x15B4
		Sonicteam::SoX::RefSharedPointer<Sonicteam::Prop::GenerateArea> m_pStagePropGenerateArea[4]; // 0x15C8
		boost::shared_ptr<Sonicteam::Prop::ActorCreators> m_spActorCreators; //0x15D8
		Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Physics::World> m_spPhysicsWorld; // 0x15E0
		Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Physics::CollisionFilter> m_spCollisionFilter; // 0x15E4
		MARATHON_INSERT_PADDING(8);

		MARATHON_DESTRUCTION_H(GameImp);;
		virtual int OnMessageRecieved(SoXMessageType);
		virtual bool IsPrevDestroyAll();
		virtual Sonicteam::DocMarathonImp* GetDocMarathon();
		virtual boost::shared_ptr<Sonicteam::ActorManager> GetActorManager();
		virtual bool IsGameKyWorldDef();
		virtual Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Physics::World> GetWorldHavok();
		virtual uint32_t CollisionFilterFunc01(uint32_t);
		virtual Sonicteam::ObjInfoMgr* GetGameObjInfoMgr();
		virtual void GameActivateProp(Sonicteam::SoX::RefCountObject**);
		virtual void GameImpUnk01();
		virtual boost::shared_ptr<Sonicteam::Prop::Manager> GameGetPropManager();
		virtual Sonicteam::NamedActor* GameGetPropTask(uint32_t);
		virtual boost::shared_ptr<Sonicteam::Player::Load> GameGetPlayerLoad();
		virtual boost::shared_ptr<Sonicteam::KeyframedPathScene>* GameGetKeyFramedScene(uint32_t);
		virtual std::string* GameGetCameraParamLua();
		virtual bool GameOLDGetActivePlayerA0Pos_1(XMVECTOR*,double); //not used ??????
		virtual bool GameOLDGetActivePlayerA0Pos_2(XMVECTOR*,double); //not used ??????
		virtual bool GameNEWGetActivePlayerA0Pos(XMVECTOR*,double);
		virtual Sonicteam::ImpactManager* GameGetImpactManager();
		virtual void GameProcessGlobalEventActions(double a2);
		virtual int GameGetSoundData();
		virtual int GameParticleContainerFirstData();
		virtual int GameGetGameRuleContext();
		virtual Sonicteam::PhysicsMaterialMgr* GameGetPhysicsMaterialMgr();

		int PlayerActorIDToIndex(int32_t actorId)
		{
			for (int i = 0; i < 4; i++)
			{
				if (m_PlayerData[i].ActorID == actorId)
					return i;
			}

			return -1;
		}
	};
};
#endif


