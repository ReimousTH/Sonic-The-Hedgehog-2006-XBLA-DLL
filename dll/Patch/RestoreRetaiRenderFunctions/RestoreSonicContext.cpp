#include "RestoreSonicContext.h"

#include <Core2/Debug.h>
#include <Core2/Hook.h>
#include <stdx/RTTI.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <Player/State/SonicContext.h>
#include <Sox/Graphics/Frame.h>
#include <Player/HairSimulator.h>
#include <Player/PackageModel/HairAnimation.h>
#include <Player/PackageModel/HairHierarchy.h>
#include <Player/PackageModel/IAnimation.h>
#include <Player/Effect/ParticleLuaInfo.h>
#include <Player/State/Machine2.h>
#include <Player/ModelSonic.h>
#include <Player/State/SonicState.h>
#include <Player/Effect/ParticleJoint.h>

using namespace Patch::RestoreSonicContext;

class NewSonicContext : public Sonicteam::Player::State::SonicContext
{
public:
    boost::shared_ptr<Sonicteam::Player::Effect::ParticleJoint> ms_spSuperParticle;
    boost::shared_ptr<Sonicteam::Player::Effect::ParticleJoint> ms_spSuperParticle2;
};


// Helper function to process a frame and its children recursively
struct FrameProcessor {
    std::map<std::string, std::vector<Sonicteam::SoX::Graphics::FrameObserver*>>& ParticleJointSave;
    std::vector<Sonicteam::Player::HairSimulator*>& HairSimulatorSave;
    Sonicteam::Player::ModelSonic* pModel;

    FrameProcessor(
        std::map<std::string, std::vector<Sonicteam::SoX::Graphics::FrameObserver*> >& particleSave,
        std::vector<Sonicteam::Player::HairSimulator*>& hairSave,
        Sonicteam::Player::ModelSonic* model)
        : ParticleJointSave(particleSave)
        , HairSimulatorSave(hairSave)
        , pModel(model)
    {
    }

    void Process(Sonicteam::SoX::Graphics::Frame* frame) {
        // Get frame name
        std::string frameName;
        const char* name = frame->GetName();
        if (name == 0)
            frameName = "";
        else
            frameName = name;

        // Process observers using LinkNode iterator
        typedef Sonicteam::SoX::LinkNode<Sonicteam::SoX::Graphics::FrameObserver>::iterator ObserverIterator;
        for (ObserverIterator it = frame->m_llFrameObserver.begin();
            it != frame->m_llFrameObserver.end();
            it++)
        {
            // Get the actual pointer from iterator
            Sonicteam::SoX::Graphics::FrameObserver* observer = *it;
            if (!observer) continue;

            // Get class name
            std::string className = stdx::ClassToName(observer);

            AddMessage("%s - %d FrameProcessor::Process %s", __FILE__, __LINE__, className);

            if (className == "class Sonicteam::Player::Effect::ParticleJoint")
            {
                Sonicteam::Player::Effect::ParticleJoint* pJoint =
                    static_cast<Sonicteam::Player::Effect::ParticleJoint*>(observer);

                AddMessage("%s - %d FrameProcessor::Process, Save ParticleJoint %x, function %x", __FILE__, __LINE__, observer);

                // Call function

                pJoint->m_pParticle->Func1C(Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Graphics::Frame>(pModel->m_spRootFrame.get()));

                // Save to map
                ParticleJointSave[frameName].push_back(observer);
            }
        }

        Sonicteam::Player::PackageModel::HairAnimation* pHairAnimation = (Sonicteam::Player::PackageModel::HairAnimation*)pModel->m_spPackageModel->m_spHairAnimation.get();
        if (!pHairAnimation)
            return;

        Sonicteam::Player::PackageModel::HairHierarchy* pHairHierarchy = pHairAnimation->m_spHairHierarchy.get();
        for (std::vector<boost::shared_ptr<Sonicteam::Player::HairSimulator>>::iterator it = pHairHierarchy->m_vspHairSimulators.begin(); it != pHairHierarchy->m_vspHairSimulators.end(); it++)
        {
            HairSimulatorSave.push_back(it->get());
        }
    }

    void AttachParticles(Sonicteam::SoX::Graphics::Frame* frame)
    {
        // Try To Join Partiles to New Nodes
        for (std::map<std::string, std::vector<Sonicteam::SoX::Graphics::FrameObserver*>>::iterator& it = ParticleJointSave.begin(); it != ParticleJointSave.end(); it++)
        {
            Sonicteam::SoX::Graphics::Frame* NodeTo = pModel->m_spFrameGPStandard->FindNode(it->first.c_str());
            if (!NodeTo)
                NodeTo = pModel->m_spFrameGPStandard->m_pParent;

            for (std::vector<Sonicteam::SoX::Graphics::FrameObserver*>::iterator jt = it->second.begin(); jt != it->second.end(); jt++)
            {
                Sonicteam::Player::Effect::ParticleJoint* pJoint =
                    static_cast<Sonicteam::Player::Effect::ParticleJoint*>(*jt);
                pJoint->m_lnFrameObserver.Reset();
                pJoint->m_pFrameGPStandard = NodeTo;
                NodeTo->m_llFrameObserver.Insert(&pJoint->m_lnFrameObserver);
            }
        }
    }

    void ProcessHairAnimation(Sonicteam::SoX::Graphics::Frame* frame)
    {
        Sonicteam::Player::PackageModel::HairAnimation* pHairAnimation = (Sonicteam::Player::PackageModel::HairAnimation*)pModel->m_spPackageModel->m_spHairAnimation.get();
        if (!pHairAnimation)
            return;
        Sonicteam::Player::PackageModel::HairHierarchy* pHairHierarchy = pHairAnimation->m_spHairHierarchy.get();
        for (std::vector<boost::shared_ptr<Sonicteam::Player::HairSimulator>>::iterator it = pHairHierarchy->m_vspHairSimulators.begin(); it != pHairHierarchy->m_vspHairSimulators.end(); it++)
        {
            int index = (it - pHairHierarchy->m_vspHairSimulators.begin());
            Sonicteam::Player::HairSimulator* HP = HairSimulatorSave[index];
            Sonicteam::Player::HairSimulator* CP = it->get();
            CP->m_vField20 = HP->m_vField20;
            CP->m_vField30 = HP->m_vField30;
            CP->m_vField40 = HP->m_vField40;
            CP->m_vField50 = HP->m_vField50;
            CP->m_vField60 = HP->m_vField60;
            CP->m_vField70 = HP->m_vField70;
            CP->m_vField80 = HP->m_vField80;
            CP->m_vField90 = HP->m_vField90;
            CP->m_vFieldA0 = HP->m_vFieldA0;
            CP->m_vFieldB0 = HP->m_vFieldB0;
            CP->m_vFieldC0 = HP->m_vFieldC0;
            CP->m_vFieldD0 = HP->m_vFieldD0;
            CP->m_vFieldF0 = HP->m_vFieldF0;
            CP->m_vField100 = HP->m_vField100;
            CP->m_vField110 = HP->m_vField110;
            CP->m_vField120 = HP->m_vField120;
            AddMessage("%s - %d ProcessHairAnimation To %x From %x", __FILE__, __LINE__, CP, HP);
        }
    }
};

// NewSonicContext Constructor 
HOOKV3(0x821E1DA0, NewSonicContext*, SonicContext__CTOR, (NewSonicContext*), (a1), NewSonicContext* a1)
{
    memset(&a1->ms_spSuperParticle, 0, 8);
    memset(&a1->ms_spSuperParticle2, 0, 8);
    return (a1);
}
// NewSonicContext Destructor 
void* SonicContext__DTOR(NewSonicContext* context, char flag)
{
    context->ms_spSuperParticle.reset();
    context->ms_spSuperParticle2.reset();
    return BranchTo(0x821D0F98, void*, context, flag);
}

bool SonicContext__IsCanDrain(NewSonicContext* context, Sonicteam::Player::State::SonicContext::Gem gem)
{
    if (!context->ScorePlugin.get() || !context->ScorePlugin->PtrObjectPlayer || !context->ScorePlugin->PtrObjectPlayer->GetInputManager())
        return false;

    Sonicteam::SoX::Input::Manager* pInputManager = context->ScorePlugin->PtrObjectPlayer->GetInputManager();

    bool IsRTReleased = pInputManager->m_PadState.IsReleased(Sonicteam::SoX::Input::KeyState_RightTrigger);
    bool IsRTPress = pInputManager->m_PadState.IsPressed(Sonicteam::SoX::Input::KeyState_RightTrigger);
    bool IsRTHold = pInputManager->m_PadState.IsDown(Sonicteam::SoX::Input::KeyState_RightTrigger);

    if (context->m_CurrentGem != gem)
        return false;

    switch (context->m_CurrentGem)
    {
    case Sonicteam::Player::State::SonicContext::Gem_Blue:
    case Sonicteam::Player::State::SonicContext::Gem_Green:
    case Sonicteam::Player::State::SonicContext::Gem_Super:
    case Sonicteam::Player::State::SonicContext::Gem_White:
    case Sonicteam::Player::State::SonicContext::Gem_Yellow:
        return IsRTPress;

    case Sonicteam::Player::State::SonicContext::Gem_Red:
    case Sonicteam::Player::State::SonicContext::Gem_Purple:
        return IsRTHold;
    case Sonicteam::Player::State::SonicContext::Gem_Sky:
        return IsRTPress | IsRTReleased | IsRTHold;
    }
    return false;
}

void SonicContext_HalfSwap(Sonicteam::Player::ObjectPlayer* pPlayer, const char* PostLuaFile, const char* Name, const char* PackageFile)
{
    Sonicteam::Player::Load* pLoad = pPlayer->GetPlugin<Sonicteam::Player::Load>("load");
    Sonicteam::Player::State::ICommonContext* pContext = pPlayer->GetPlugin<Sonicteam::Player::State::ICommonContext>("state context");
    Sonicteam::Player::ModelSonic* pModelSonic = pPlayer->GetPlugin<Sonicteam::Player::ModelSonic>("model");


    pPlayer->m_PackageFile = PackageFile;
    pPlayer->m_LuaFile = PostLuaFile;
    pPlayer->m_Name = Name;

    pLoad->LoadPackage(pPlayer->m_LuaFile);

    pModelSonic->m_spPackageModel->m_pFrameGPObject->RemoveFromParent();
    pModelSonic->m_spPackageShoesModel->m_pFrameGPObject->RemoveFromParent();

    Sonicteam::SoX::RefSharedPointer<Sonicteam::LuaSystem> m_spLuaFile(0);

    // Save Particles From Previous Frame
    std::map<std::string, std::vector<Sonicteam::SoX::Graphics::FrameObserver*> > ParticleJointSave;
    std::vector<Sonicteam::Player::HairSimulator*> HairSimulatorSave;

    double currentKeyFrame = pModelSonic->m_spPackageModel->m_PackageAnimationRoot->GetKeyFrame();
    int curretAnimationID = pModelSonic->IModelGetAnimID1();

    FrameProcessor processor(ParticleJointSave, HairSimulatorSave, pModelSonic);
    processor.Process(pModelSonic->m_spRootFrame.get());

    pModelSonic->m_spPackageShoesModel->m_pFrameGPObject->RemoveFromParent();
    pModelSonic->m_spPackageModel->m_pFrameGPObject->RemoveFromParent();
    //pModelSonic->m_spFrameGPStandard->AddChild(pModelSonic->m_spPackageShoesModel->m_pFrameGPObject);

    pModelSonic->m_spFrameGPStandard->DestroyChildren();
    pModelSonic->m_pFrameGPShoes->DestroyChildren();

    ((int(__fastcall*)(Sonicteam::SoX::RefSharedPointer<Sonicteam::LuaSystem>&, std::string&, int, int))0x821B5E58)(m_spLuaFile, pPlayer->m_LuaFile, 0X82002BA8, 0x1D);

    pModelSonic->OnVarible(m_spLuaFile);
    pContext->OnVarible(m_spLuaFile); // update physics

    BranchTo(0x821F51F0, int, pModelSonic->m_spPackageModel.get(), curretAnimationID);
    pModelSonic->m_spPackageModel->m_PackageAnimationRoot->SetKeyFrame2(currentKeyFrame);

    processor.AttachParticles(pModelSonic->m_spRootFrame.get());

}


Sonicteam::Player::Effect::ParticleLuaInfo SonicContext_GetEffect(uint32_t pEffect, const char* particleName)
{
    Sonicteam::Player::Effect::ParticleLuaInfo particleData;
    HookBranchTo(0x821ECC80, int, &particleData, pEffect + 0x2C, &std::string(particleName));
    return particleData;
}

boost::shared_ptr<Sonicteam::Player::Effect::ParticleJoint> SonicContext_PlayParticle(uint32_t pEffect, Sonicteam::SoX::RefSharedPointer<Sonicteam::Player::RootFrame>& spRootFrame, Sonicteam::Player::Effect::ParticleLuaInfo particleLuaData)
{
    boost::shared_ptr<Sonicteam::Player::Effect::ParticleJoint> spParticleJoint;

    /*
    // Call Particle
    HookBranchTo(0x821ECE40, int,
        &spParticleJoint, // r3
        pEffect + 0x2C,  // r4
        *(uint64_t*)&particleData.BankID, //r5
        *(uint64_t*)&particleData.Direction, //r6
        *(uint64_t*)&particleData.OffsetX, //r7
        *(uint32_t*)&particleData.OffsetZ, //r8
        &spRootFrame //r9
    );
    */
    char* particleData = (char*)&particleLuaData;

    // Call Particle
    HookBranchTo(0x821ECE40, int,
        &spParticleJoint, // r3
        pEffect + 0x2C,  // r4
        *(uint64_t*)&particleData[0], //r5
        *(uint64_t*)&particleData[8], //r6
        *(uint64_t*)&particleData[0x10], //r7
        (uint64_t) * (uint32_t*)&particleData[0x18], //r8
        &spRootFrame //r9
    );



    return spParticleJoint;
}

// SonicContext Gems Restoration
void SonicContext__Update(NewSonicContext* context, double a2)
{
    if (!context->ScorePlugin.get() || !context->ScorePlugin->PtrObjectPlayer || !context->ScorePlugin->PtrObjectPlayer->GetInputManager())
    {
        return;
        HookBranchTo(0x821E0BC0, int, context, a2);
    }

    Sonicteam::Player::Score* pScore = context->ScorePlugin.get();
    Sonicteam::Player::ObjectPlayer* pPlayer = context->ScorePlugin->PtrObjectPlayer;
    Sonicteam::Player::State::Machine2* pStateMachine = pPlayer->GetStateMachine<Sonicteam::Player::State::Machine2>();
    Sonicteam::SoX::RefSharedPointer<Sonicteam::Player::RootFrame> spRootFrame(pPlayer->m_spRootFrame.get());
    uint32_t pEffect = (uint32_t)pPlayer->GetPlugin<Sonicteam::Player::IPlugIn>("effect");
    Sonicteam::Player::Load* pLoad = pPlayer->GetPlugin<Sonicteam::Player::Load>("load");
    Sonicteam::Player::ModelSonic* pModelSonic = pPlayer->GetPlugin<Sonicteam::Player::ModelSonic>("model");
    Sonicteam::SoX::Input::Manager* pInputManager = pPlayer->GetInputManager();



    context->m_Shrink = 0;

    bool BlockWhiteGem = false;
    bool BlockSkyGemSnipe = false;
    bool BlockSkyGemThrow = false;
    bool BlockTornadoGem = false;
    bool BlockSuperGem = false;
    bool BlockMachSpeed = false;
    bool IsSuper = pPlayer->m_Name == "supersonic";
    bool IsGrounded = (context->m_PostureFlags & PostureFlag_Grounded) != 0;

    if (Sonicteam::SoX::AI::State<Sonicteam::Player::State::IContext>* state = pStateMachine->CurrentState.get())
    {
        uint32_t state_vft = *(uint32_t*)(state);
        if (state_vft == 0x8200A248)
        {
            BlockWhiteGem = true;
        }
        if (state_vft == 0x82003204)
        {
            BlockSkyGemSnipe = true;
        }
        if (state_vft == 0x820031D4)
        {
            BlockSkyGemThrow = true;
        }
        if (state_vft == 0x820032FC || state_vft == 0x820032D0)
        {
            BlockTornadoGem = true;
        }
        if (state_vft == 0x82003330)
        {
            BlockSuperGem = true;
        }
        if (state_vft == 0x8200329C)
        {
            BlockMachSpeed = true;
        }
    }

    bool IsRTReleased = pInputManager->m_PadState.IsReleased(Sonicteam::SoX::Input::KeyState_RightTrigger);
    bool IsRTPress = pInputManager->m_PadState.IsPressed(Sonicteam::SoX::Input::KeyState_RightTrigger);
    bool IsRTHold = pInputManager->m_PadState.IsDown(Sonicteam::SoX::Input::KeyState_RightTrigger);


    if (IsRTHold)
    {
        context->float22C = clamp(context->float22C + a2, 0.0, 1.0);
    }
    else
    {
        context->float22C = 0.0;
    }

    if (IsRTReleased)
    {
        switch (context->m_CurrentGem)
        {
        case Sonicteam::Player::State::SonicContext::Gem_Sky:
            if (IsGrounded && !BlockSkyGemThrow && !BlockSkyGemSnipe)
                pStateMachine->ChangeState(Sonicteam::Player::State::SonicState_GemThrow);
            break;
        }
    }
    // Press (0x10000 input)
    if (IsRTPress)
    {
        switch (context->m_CurrentGem)
        {
        case Sonicteam::Player::State::SonicContext::GemSprite_Super:
            if (!BlockSuperGem && !IsSuper)
            {
                context->ms_spSuperParticle = SonicContext_PlayParticle(pEffect, spRootFrame, SonicContext_GetEffect(pEffect, "superchange"));
                pStateMachine->ChangeState(Sonicteam::Player::State::SonicState_SonicSuper);
                context->float228 = 1.25;
            }

            break;
        }
    }

    context->float228 = clamp(context->float228 - a2, 0.0, 1.25);
    if (context->float228 == 0.0 && BlockSuperGem && !IsSuper)
    {
        SonicContext_HalfSwap(pPlayer, "player/sonic_super.lua", "supersonic", "player/sonic_super");
        context->float228 = 1.0;
    }

    if (IsSuper)
    {
        if (!context->ms_spSuperParticle2.get())
            context->ms_spSuperParticle2 = SonicContext_PlayParticle(pEffect, spRootFrame, SonicContext_GetEffect(pEffect, "superchange_g"));

        context->IsInvulnerable2 = 1;
        if (pPlayer->PlayerScore->GetRings() <= 0)
        {
            context->ms_spSuperParticle2.reset();
            SonicContext_HalfSwap(pPlayer, "player/sonic_new.lua", "sonic", "player/sonic_new");
        }
        else if (context->float228 == 0.0)
        {
            pPlayer->PlayerScore->AddRings(-1);
            context->float228 = 1.0;
        }
    }


    else if (IsRTHold && !IsRTPress)
    {
        switch (context->m_CurrentGem)
        {
        case Sonicteam::Player::State::SonicContext::Gem_Sky:
            if (IsGrounded && !BlockSkyGemSnipe && !BlockSkyGemThrow && context->float22C > 0.2)
                pStateMachine->ChangeState(Sonicteam::Player::State::SonicState_GemSnipe);
            break;
        }
    }

    HookBranchTo(0x821E0BC0, int, context, a2);
}

void Patch::RestoreSonicContext::INSTALL()
{
    INSTALL_HOOKV3EX(SonicContext__CTOR, 1, false, 9);
    HookNew::WRITE_VALUE(0x8200A3C4, SonicContext__DTOR);
    HookNew::WRITE_VALUE(0x8200A3D0, SonicContext__IsCanDrain);
    HookNew::WRITE_VALUE(0x8200A3CC, SonicContext__Update);
    HookNew::WRITE_VALUE(0x82186A04, POWERPC_ADDIS(3, 0, sizeof(NewSonicContext)));
}