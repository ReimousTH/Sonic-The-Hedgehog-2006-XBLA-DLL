#pragma once

#include <Marathon.inl>
#include <Sox/Audio/Cue.h>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Audio
        {
            class Emitter : public Cue
            {
            public:
                MARATHON_INSERT_PADDING(0x3C);
            };
        }
    }
}