#ifndef SONICTEAM__PLAYER__OBJECTPLAYER
#define SONICTEAM__PLAYER__OBJECTPLAYER

#include "Defs.h"


#include <Actor.h>
#include <Sox/Engine/Task.h>
#include <Player/RootFrame.h>
#include <Player/IModel.h>
#include <Player/State/IMachine.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Player/Gauge/IGauge.h>
#include <Player/Score.h>
#include <Player/Load.h>
#include <Player/IVariable.h>
#include <Player/IDynamicLink.h>
#include <Player/IStepable.h>
#include <Player/IExportExternalFlag.h>
#include <Player/IExportPostureRequestFlag.h>
#include <Player/IExportWeaponRequestFlag.h>
#include <Player/IImportAnimation.h>
#include <Player/OtherParameter.h>
#include <Player/Camera/Cameraman.h>
#include <Sox/Input/Manager.h>

//#include <Player/IImportAnimation>
#include <Player/INotification.h>
#include <Player/AI/IBase.h>
#include <Combo/AttackManager.h>

#include <Player/IEventer.h>

#include <xtl.h>


namespace Sonicteam{
	
	class m_pCameraman; 
	class m_spPackageBinary; 


	namespace Player{

		class Gravity;
	

		struct ObjectPlayerCreationData{
			const char *m_Name;
			int m_Index;
			int m_PadID;
			int CameraActorID;
			int SetupModuleIndex;
			int Unk0x14;
			int Unk0x18;
			int Unk0x1C;
			XMVECTOR PlayerRotation;
			XMVECTOR PlayerPosition;
			int PlayerStartRingsCount;
			void *player_based_on;
			char CurrentIsAmigo;
		};
		struct ObjectPlayerUpgrade{
			unsigned int global_flag;
			unsigned int equip_flag;
		};

		#define RegisterPluginMatcher(Type) \
			bool operator()(const boost::shared_ptr<Type>& ptr) const { \
			Sonicteam::Player::IPlugIn* plugin = dynamic_cast<Sonicteam::Player::IPlugIn*>(ptr.get()); \
			return plugin != NULL && plugin->PluginName == PlugName; \
		}

		struct PluginMatcher {
			std::string PlugName;
			PluginMatcher(const std::string& plugName) : PlugName(plugName) {}	
			RegisterPluginMatcher(Sonicteam::Player::IPlugIn);
			RegisterPluginMatcher(Sonicteam::Player::IVariable);
			RegisterPluginMatcher(Sonicteam::Player::IDynamicLink);
			RegisterPluginMatcher(Sonicteam::Player::IFlagCommunicator);
			RegisterPluginMatcher(Sonicteam::Player::IStepable);
			RegisterPluginMatcher(Sonicteam::Player::IExportExternalFlag);
			RegisterPluginMatcher(Sonicteam::Player::IExportPostureRequestFlag);
			RegisterPluginMatcher(Sonicteam::Player::IExportWeaponRequestFlag);
			RegisterPluginMatcher(Sonicteam::Player::IImportAnimation);
			RegisterPluginMatcher(Sonicteam::Player::INotification);
			RegisterPluginMatcher(Sonicteam::Player::IEventerListener);

		

			//		ModelMatcherOP(int);
		};

		

		class ObjectPlayer:public Sonicteam::Actor
		{


		public:
			ObjectPlayer(Sonicteam::SoX::Engine::Task* NamedTask,ObjectPlayerCreationData* CreationData);

			virtual int OnMessageRecieved(SoXMessageType) override;

			virtual char* GetType() override;

			virtual void OnStep(float) override;

			std::string m_LuaFile; // 0x58;
			std::string m_PackageFile; // 0x58;

			unsigned int m_TargetCameraActorID; //0x90
			Sonicteam::SoX::MessageReceiver* m_pCameraman; //0x94
			unsigned m_Index; //0x98 (not sure )
			unsigned m_PadID; //0x9C

			XMVECTOR m_SpawnRotation; //0xA0
			XMVECTOR m_SpawnPosition; //0xB0

			unsigned int m_SpawnRingCount; //0xC0
			REF_TYPE(Sonicteam::SoX::RefCountObject) m_spSpawnSource; //0xC4
			char CurrentIsAmigo; //0xC8
			char m_IsPosture; //0xC9
			char m_IsAmigo; //0xCA //82196F78
			char IsFlag0xCB; //0xCB //82196F78
			REF_TYPE(Sonicteam::Player::RootFrame) m_spRootFrame; //0xCC
			REF_TYPE(Sonicteam::m_spPackageBinary) m_spPackageBinary; //0xD0

			boost::shared_ptr<Sonicteam::Player::IModel> m_spModel; //0xD4, either just Model, Replace Later 
			boost::shared_ptr<Sonicteam::Player::IPostureControl> m_spPostureControl; // 0xDC
			boost::shared_ptr<Sonicteam::Player::State::IMachine> m_spStateMachine; // 0xE4
			boost::shared_ptr<Sonicteam::Player::Gravity> m_spGravity; // 0xEC
			boost::shared_ptr<Sonicteam::Player::IImpulseManager> m_spImpulse; // 0xF4
			unsigned int m_SetupModuleIndexPrefix; //0xFC
			unsigned int m_SetupModuleIndexPostfix; //0x100
			boost::shared_ptr<Sonicteam::Player::IGauge> m_spGauge; //0x104
			unsigned int unk0x10C; //0x10C
			unsigned int unk0x110; //0x110
			std::vector<boost::shared_ptr<Sonicteam::Player::IPlugIn>> m_vspPlugins; //0x114
			boost::shared_ptr<Sonicteam::Player::Path> PlayerPath; //0x124
			//boost::shared_ptr<Sonicteam::Player::PathGuide> PlayerPathGuide; //0x12C
			boost::shared_ptr<Sonicteam::Player::PathCollision> PlayerPathCollision; //0x12C
			boost::shared_ptr<Sonicteam::Player::PathLightDash> PlayerPathLightDash; //0x134
			boost::shared_ptr<Sonicteam::Player::Score> PlayerScore; //0x13C , WeakPTR? (82195BC8 . lock called why?)
			boost::shared_ptr<Sonicteam::Player::GameMasterCommunicator> PlayerGameMaster; //0x144
			boost::shared_ptr<Sonicteam::Player::Load> PlayerLoad; //0x14C
			boost::shared_ptr<Sonicteam::Player::IEventer> PlayerEventer; //0x154
			boost::shared_ptr<Sonicteam::Player::IEventer> PlayerPostureRodeo; //0x15C
			unsigned int unk0x164;
			unsigned int unk0x168;
			boost::shared_ptr<Sonicteam::Player::PathWaterSlider> PlayerPathWaterSlider; //0x16C
			float m_DeltaTime; //0x174
			MARATHON_INSERT_PADDING(0x30);
			std::vector<Sonicteam::Player::ObjectPlayerUpgrade> m_vEquipFlags; //0x1A8
			std::string m_Name; //0x1B8

			unsigned int unk0x1F4; //0x1F4
			unsigned int unk0x1F8; //0x1F4 == -1
			unsigned int unk0x1FC; //0x1F4 == -1
			unsigned long long PlayerExternalFlag; //0x200, removes after init i think
			unsigned int PlayerICommonContextIFFlag; //0x208
			unsigned int IPosturePlugInFlag; //0x20C
			unsigned int PlayerUpgradeFlag; //0x210
			unsigned int PlayerExportFlag2; //0x214
			unsigned int PlayerCurrentGem; //0x218

			std::vector<boost::shared_ptr<Sonicteam::Player::IVariable>> PlayerIVarible; //0x21C
			std::vector<boost::shared_ptr<Sonicteam::Player::IDynamicLink>> PlayerIDynamicLink; //0x22C
			std::vector<boost::shared_ptr<Sonicteam::Player::IFlagCommunicator>> PlayerIFlagCommunicator; //0x23C

			std::vector<boost::shared_ptr<Sonicteam::Player::IStepable>> PlayerIStepable1; //0x24C
			std::vector<boost::shared_ptr<Sonicteam::Player::IStepable>> PlayerIStepable2; //0x25C
			std::vector<boost::shared_ptr<Sonicteam::Player::IStepable>> PlayerIStepable3; //0x26C
			std::vector<boost::shared_ptr<Sonicteam::Player::IStepable>> PlayerIStepable4; //0x27C
		
			std::vector<boost::shared_ptr<Sonicteam::Player::IExportExternalFlag>> PlayerIExportExternalFlag; //0x28C;
			std::vector<boost::shared_ptr<void*>> PlayerIExportVehicleFlag; //0x29C (not used) // IExportVehicleFla

			std::vector<boost::shared_ptr<Sonicteam::Player::IExportPostureRequestFlag>> PlayerIExportPostureRequestFlag; //0x2AC
			std::vector<boost::shared_ptr<Sonicteam::Player::IExportWeaponRequestFlag>> PlayerIExportWeaponRequestFlag; //0x2BC
			std::vector<boost::shared_ptr<Sonicteam::Player::IImportAnimation>> PlayerIImportAnimation; //0x2CC
			std::vector<boost::shared_ptr<Sonicteam::Player::INotification>> PlayerINotification; //0x2DC


			boost::shared_ptr<Sonicteam::Player::AI::IBase> PlayerAIAmigo; //0x2EC
			unsigned int unk0x2F4;
			unsigned int unk0x2F8; //ref count objs (82195CFC )
			REF_TYPE(Sonicteam::Combo::AttackManager) PlayerComboAttackManager; //0x2FC
			unsigned int unk0x300;
			unsigned int unk0x304;
			unsigned int unk0x308;
			unsigned int unk0x30C;

			template <typename T>
			T* GetStateMachine()
			{
				return static_cast<T*>(m_spStateMachine.get());
			}

			template <typename T>
			T* GetCameraMan()
			{
				return static_cast<T*>(m_pCameraman);
			}

			template <typename T>
			T* GetPlugin(const char* plugin)
			{
				for (std::vector<boost::shared_ptr<Sonicteam::Player::IPlugIn>>::iterator it = this->m_vspPlugins.begin(); it != this->m_vspPlugins.end(); ++it) 
				{
					if (it->get()->PluginName == plugin)
						return static_cast<T*>(it->get());
				}
				return 0;
			}

			SoX::Input::Manager* GetInputManager();

			void RemovePlugin(std::string plugin);

			template <typename T>
			void RemovePlugin(boost::shared_ptr<T> plugin);

			boost::shared_ptr<Sonicteam::Player::IPlugIn> FindPluginLast(std::string plugin);
	

		};

		#define RemovePluginDef(Vector, PlugType, OtherPlug, OtherPlugType) \
		for (typename std::vector<boost::shared_ptr<PlugType>>::iterator it = Vector.begin(); \
		it != Vector.end(); ) { \
		if (boost::dynamic_pointer_cast<OtherPlugType>(*it).get() == OtherPlug.get()) { \
		it = Vector.erase(it); \
		} else { \
		it++; \
		} \
		}


		template <typename T>
		void Sonicteam::Player::ObjectPlayer::RemovePlugin(boost::shared_ptr<T> plugin)
		{
			RemovePluginDef(this->m_vspPlugins,Sonicteam::Player::IPlugIn,plugin,T);
			RemovePluginDef(this->PlayerIVarible,Sonicteam::Player::IVariable,plugin,T);
			RemovePluginDef(this->PlayerIDynamicLink,Sonicteam::Player::IDynamicLink,plugin,T);
			RemovePluginDef(this->PlayerIFlagCommunicator,Sonicteam::Player::IFlagCommunicator,plugin,T);
			RemovePluginDef(this->PlayerIStepable1,Sonicteam::Player::IStepable,plugin,T);
			RemovePluginDef(this->PlayerIStepable2,Sonicteam::Player::IStepable,plugin,T);
			RemovePluginDef(this->PlayerIStepable3,Sonicteam::Player::IStepable,plugin,T);
			RemovePluginDef(this->PlayerIStepable4,Sonicteam::Player::IStepable,plugin,T);
			RemovePluginDef(this->PlayerIExportExternalFlag,Sonicteam::Player::IExportExternalFlag,plugin,T);
			RemovePluginDef(this->PlayerIExportPostureRequestFlag,Sonicteam::Player::IExportPostureRequestFlag,plugin,T);
			RemovePluginDef(this->PlayerIExportWeaponRequestFlag,Sonicteam::Player::IExportWeaponRequestFlag,plugin,T);
			RemovePluginDef(this->PlayerIImportAnimation,Sonicteam::Player::IImportAnimation,plugin,T);
			RemovePluginDef(this->PlayerINotification,Sonicteam::Player::INotification,plugin,T);
			RemovePluginDef(this->PlayerEventer->EventerListener,Sonicteam::Player::IEventerListener,plugin,T);

		}

	}
}




#endif
