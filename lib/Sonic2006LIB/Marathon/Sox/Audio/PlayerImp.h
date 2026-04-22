#pragma once

#include <Marathon.inl>
#include <Sox/Audio/Player.h>
#include <Sox/LinkNode.h>
#include <Boost/shared_ptr.hpp>
#include <vector>
#include <map>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Audio
        {
            class Listener;
            class Cue;
            class AudioPlayerXenon;
            class ObjectManager;
            class SoundBank;

            class PlayerImp : Player
            {
            public:
                MARATHON_DESTRUCTION_H(PlayerImp);
                Listener* m_pListener;
                SoX::LinkedList<Cue> m_llCues;
                MARATHON_INSERT_PADDING(8);
                boost::shared_ptr<AudioPlayerXenon> m_spAudioPlayerXenon;
                MARATHON_INSERT_PADDING(4);
                std::vector<SoundBank*> m_vSoundBanks; // not sure about std::vector<>
                SoX::LinkNode<SoundBank> m_lnSoundBank;
                std::map<std::string, uint32_t> m_mBankIndicies;
                MARATHON_INSERT_PADDING(4);
            };
        }
    }
}