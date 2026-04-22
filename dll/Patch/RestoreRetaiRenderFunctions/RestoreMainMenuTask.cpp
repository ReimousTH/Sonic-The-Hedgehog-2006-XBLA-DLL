#include "RestoreMainMenuTask.h"

#include <Core2/Debug.h>
#include <Core2/Hook.h>
#include <stdx/RTTI.h>
#include <MainMenuTask.h>
#include <AudioRoomTask.h>
#include <SoX/Audio/AudioPlayerXenon.h>
#include <SoX/Audio/PlayerImp.h>
#include <SoX/Audio/Cue.h>
#include <SoX/Engine/Task.h>
#include <SoX/Input/Manager.h>


using namespace Patch::RestoreMainMenuTask;

HOOKV3(0x82486A28, Sonicteam::MainMenuTask*, MainMenuTask__CTOR,
    (Sonicteam::MainMenuTask*, int), (pMenuTask, a2),
    Sonicteam::MainMenuTask* pMenuTask, int a2)
{
    pMenuTask->MMTSoundFlags = 0;
    pMenuTask->MMMyAudioADXCue = 0;
    return pMenuTask;
}

void MainMenuTask__StopSound(Sonicteam::MainMenuTask* pMainMenuTask)
{
    Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Audio::Cue>* pspCue = (Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Audio::Cue>*) & pMainMenuTask->MMMyAudioADXCue;
    pspCue->reset();
}


uint32_t MainMenuTask__DTOR(Sonicteam::MainMenuTask* pMenuTask, uint32_t flag)
{
    MainMenuTask__StopSound(pMenuTask);
    return BranchTo(0x82485C90, int, pMenuTask, flag);
}

void MainMenuTask__PlaySound(Sonicteam::MainMenuTask* pMainMenuTask, const char* bank, const char* cue)
{
    Sonicteam::SoX::Audio::Player* pAudioPlayer = pMainMenuTask->GetCurrentDoc()->DocGetAudioPlayerImp().get();
    uint32_t bankID = pAudioPlayer->GetSoundBankID(bank);
    uint32_t cueID = pAudioPlayer->GetSoundCueID(bankID, cue);

    Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Audio::Cue>* pspCue = (Sonicteam::SoX::RefSharedPointer<Sonicteam::SoX::Audio::Cue>*) & pMainMenuTask->MMMyAudioADXCue;

    uint32_t prevBankID = -1;
    uint32_t prevCueID = -1;

    bool isNew = false;
    if (Sonicteam::SoX::Audio::Cue* cue = pspCue->get())
    {
        if (pMainMenuTask->MMTSoundFlags != cueID)
        {
            isNew = true;
            *pspCue = pAudioPlayer->Play(cueID);
        }

    }
    else
    {
        isNew = true;
        *pspCue = pAudioPlayer->Play(cueID);
    }
    pMainMenuTask->MMTSoundFlags = cueID;


    AddMessage2(isNew, "%s - %d MainMenuTask__PlaySound bank : [%s - %x], cue [%s - %x], previous bank [%x], cue [%x]",
        __FILE__, __LINE__, bank, bankID, cue, cueID, prevBankID, prevCueID);
}

uint32_t MainMenuTask__Update(Sonicteam::MainMenuTask* pMenuTask, double deltaTime)
{
    bool isStopSound = false;
    if (pMenuTask->m_pCurrentTask && stdx::ClassToName(pMenuTask->m_pCurrentTask) == "class Sonicteam::AudioRoomTask")
    {
        Sonicteam::AudioRoomTask* pAudioRoomTask = (Sonicteam::AudioRoomTask*)pMenuTask->m_pCurrentTask;
        if (pAudioRoomTask->m_State == 3)
        {
            isStopSound = true;
            MainMenuTask__StopSound(pMenuTask);
        }
        if (pAudioRoomTask->m_State == 2)
        {
            MainMenuTask__PlaySound(pMenuTask, "bgm", "mainmenu");
        }
    }


    switch (pMenuTask->MMCurrentCase)
    {
        //MainMenu-Init
    case 1:
        if (!isStopSound) MainMenuTask__PlaySound(pMenuTask, "bgm", "mainmenu");
    case 0x17:
        if (!isStopSound) MainMenuTask__PlaySound(pMenuTask, "bgm", "mainmenu");
        break;

        // Tag Story/ Trial, Constructor
    case 0x22:
        if (!isStopSound) MainMenuTask__PlaySound(pMenuTask, "bgm", "select");
        break;

        // Tag Story/ tRIAL, Exit
    case 0x1E:
        if (!isStopSound) MainMenuTask__PlaySound(pMenuTask, "bgm", "mainmenu");
        break;

    case 0x2D:
        if (!isStopSound) MainMenuTask__PlaySound(pMenuTask, "bgm", "select");
        break;
    }

    if (pMenuTask->GetCurrentDoc()->m_vspInputManager[0]->m_PadState.IsPressed(Sonicteam::SoX::Input::KeyState_Y))
    {
        char* pHudBackground = *(char**)((char*)pMenuTask + 0x70);
        Sonicteam::SoX::MessageReceiver* pReceiver = (Sonicteam::SoX::MessageReceiver*)(pHudBackground + 0x28);

        struct message_1b00c : Sonicteam::SoX::Message<0x1B00C>
        {
            uint32_t id2;
        };
        message_1b00c msg;
        msg.id2 = 0;
        pReceiver->OnMessageRecieved(&msg);
    }

    static uint32_t LastCase = pMenuTask->MMCurrentCase;
    if (pMenuTask->MMCurrentCase != LastCase)
    {
        LastCase = pMenuTask->MMCurrentCase;
        AddMessage2(false, "%s - %d MainMenuTask__Update MMCurrentCase %x", __FILE__, __LINE__, pMenuTask->MMCurrentCase);

        if (pMenuTask->MMCurrentCase == 1)
        {
            HardwareBreakpoint(__FILE__, __LINE__, "MainMenuTask::Update()");
        }
    }

    return BranchTo(0x8248CD20, uint32_t, pMenuTask, deltaTime);
}

uint32_t MainMenuTask__ProcessMessage(Sonicteam::SoX::MessageReceiver* pTask, Sonicteam::SoX::IMessage* pMessage)
{

    Sonicteam::MainMenuTask* pMainMenuTask = static_cast<Sonicteam::MainMenuTask*>(pTask);

    Sonicteam::SoX::Audio::Player* pAudioPlayer = pMainMenuTask->GetCurrentDoc()->DocGetAudioPlayerImp().get();

    uint32_t systemBankID = pAudioPlayer->GetSoundBankID("system");
    uint32_t systemBankBGM = pAudioPlayer->GetSoundBankID("bgm");

    uint32_t mainDesideCueID = pAudioPlayer->GetSoundCueID(systemBankID, "main_deside");
    uint32_t cannotDesideCueID = pAudioPlayer->GetSoundCueID(systemBankID, "cannot_deside");
    uint32_t WindowCloseCueID = pAudioPlayer->GetSoundCueID(systemBankID, "window_close");
    uint32_t moveCueID = pAudioPlayer->GetSoundCueID(systemBankID, "move");

    uint32_t selectCueID = pAudioPlayer->GetSoundCueID(systemBankBGM, "select");



    // Message ID definitions based on decompiled code analysis
    struct Message0x10001 : public Sonicteam::SoX::Message<0x10001>
    {
        uint32_t buttonID;
    };
    struct Message0x10002 : Sonicteam::SoX::Message<0x10002>
    {
        uint32_t buttonID;
        bool Action;
    };
    struct Message0x1C002 : Sonicteam::SoX::Message<0x1C002> {};
    struct Message0x1C003 : Sonicteam::SoX::Message<0x1C003> {};
    struct Message0x1C005 : Sonicteam::SoX::Message<0x1C005> {};

    if (Message0x10001* msg = (Message0x10001*)Message0x10001::AsMessage(pMessage))
    {
        pAudioPlayer->Play(moveCueID);

    }

    if (Message0x10002* msg = (Message0x10002*)Message0x10002::AsMessage(pMessage))
    {
        if (msg->buttonID == 0x5A || msg->Action)
        {
            pAudioPlayer->Play(mainDesideCueID);
        }
        else
        {
            pAudioPlayer->Play(WindowCloseCueID);
        }
    }

    return (BranchTo(0x82483A48, int, pTask, pMessage));
}


void Patch::RestoreMainMenuTask::INSTALL()
{
    HookNew::WRITE_VALUE(0x8202D4C8, MainMenuTask__ProcessMessage);
    HookNew::WRITE_VALUE(0x8202D4D4, MainMenuTask__DTOR);
    HookNew::WRITE_VALUE(0x8202D4D8, MainMenuTask__Update);
    INSTALL_HOOKV3EX(MainMenuTask__CTOR, 1, true, 9);
}