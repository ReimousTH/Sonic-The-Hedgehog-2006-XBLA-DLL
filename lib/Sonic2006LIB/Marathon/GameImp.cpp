#include "GameImp.h"

#include <NamedActor.h>

using namespace Sonicteam;


DESTRUCTION_CPP(GameImp);

int Sonicteam::GameImp::OnMessageRecieved(SoXMessageType)
{
	throw std::logic_error("The method or operation is not implemented.");
}

Sonicteam::GameImp::~GameImp()
{
	

}

Sonicteam::GameImp::GameImp()
{

}

bool Sonicteam::GameImp::IsPrevDestroyAll()
{
	return this->m_PostState == 9;
}

Sonicteam::DocMarathonImp* Sonicteam::GameImp::GetDocMarathon()
{
	return this->m_pDoc;
}

boost::shared_ptr<Sonicteam::ActorManager> Sonicteam::GameImp::GetActorManager()
{
	return boost::shared_ptr<Sonicteam::ActorManager>(this->m_spActorManager);
}

bool Sonicteam::GameImp::IsGameKyWorldDef()
{
	return this->GameKyWorldDef;
}

Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Physics::World> Sonicteam::GameImp::GetWorldHavok()
{
	return this->m_spPhysicsWorld;
}

unsigned int Sonicteam::GameImp::CollisionFilterFunc01(unsigned int)
{
	return 0;
	//no idea
}

Sonicteam::ObjInfoMgr* Sonicteam::GameImp::GetGameObjInfoMgr()
{
	return this->m_pObjInfoMgr;
}

void Sonicteam::GameImp::GameActivateProp(Sonicteam::SoX::RefCountObject**)
{

}

void Sonicteam::GameImp::GameImpUnk01()
{

}

boost::shared_ptr<Sonicteam::Prop::Manager> Sonicteam::GameImp::GameGetPropManager()
{
	return this->m_spPropManager;
}

Sonicteam::NamedActor* Sonicteam::GameImp::GameGetPropTask(unsigned int index)
{
	Sonicteam::NamedActor *result; // r3
	switch ( index )
	{
	case 1:
		result = this->m_pRootTask;
		break;
	case 2:
		result = this->m_pPlayers;
		break;
	case 3:
		result = this->m_pCameras;
		break;
	case 4:
		result = this->m_pDependCameras;
		break;
	case 5:
		result = this->m_pSystemDependCameras;
		break;
	case 6:
		result = this->m_pEnemyThread;
		break;
	default:
		result = this->m_pEntities;
		break;
	}
	return result;

}

boost::shared_ptr<Sonicteam::Player::Load> Sonicteam::GameImp::GameGetPlayerLoad()
{
	return this->m_spPlayerLoad;
}

boost::shared_ptr<Sonicteam::KeyframedPathScene>* Sonicteam::GameImp::GameGetKeyFramedScene(unsigned int index)
{
	return this->m_aStageKeyFramePaths[index];

}

std::string* Sonicteam::GameImp::GameGetCameraParamLua()
{
	return &this->m_StageEnvironment.LuaEnvironment.CameraParam;

}

bool Sonicteam::GameImp::GameOLDGetActivePlayerA0Pos_1(XMVECTOR* vec,double delta) //i guess?
{
	return false; // easy to implement i wait until Actor class
}

bool Sonicteam::GameImp::GameOLDGetActivePlayerA0Pos_2(XMVECTOR* vec,double delta)
{
	return GameOLDGetActivePlayerA0Pos_1(vec,delta);
}

bool Sonicteam::GameImp::GameNEWGetActivePlayerA0Pos(XMVECTOR*,double)
{
	return false; // ESH too much vector operations
}

Sonicteam::ImpactManager* Sonicteam::GameImp::GameGetImpactManager()
{
	return this->m_pImpactManager;
}

void Sonicteam::GameImp::GameProcessGlobalEventActions(double a2)
{

}

int Sonicteam::GameImp::GameGetSoundData()
{
	return 0;
}

int Sonicteam::GameImp::GameParticleContainerFirstData()
{
	return 0;
}

int Sonicteam::GameImp::GameGetGameRuleContext()
{
	return this->GetDocMarathon()->DocGetGameRuleContext();
}

Sonicteam::PhysicsMaterialMgr* Sonicteam::GameImp::GameGetPhysicsMaterialMgr()
{
	return this->m_pPhysicsMaterialMgr;
}
