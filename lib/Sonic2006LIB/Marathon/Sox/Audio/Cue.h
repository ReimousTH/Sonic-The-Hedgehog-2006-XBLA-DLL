#pragma once

#include <Marathon.inl>
#include <Sox/LinkNode.h>
#include <Sox/RefCountObject.h>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Audio
        {
            class Player;

            class Cue : public SoX::RefCountObject
            {
            public:
                MARATHON_DESTRUCTION_H(Cue);
                Listener* m_pListener;
                SoX::LinkedList<Cue> m_llCues;
                MARATHON_INSERT_PADDING(8);
                SoX::LinkNode<Cue> m_lnCue;
                MARATHON_INSERT_PADDING(8);
                Player* m_pPlayer;
            };
        }
    }
}