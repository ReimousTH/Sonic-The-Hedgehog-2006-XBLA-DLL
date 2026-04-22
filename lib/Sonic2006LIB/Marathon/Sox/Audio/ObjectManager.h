#pragma once

#include <Marathon.inl>
#include <Sox/RefSharedPointer.h>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Audio
        {
            class MyCue;
            class Emitter;

            class ObjectManager
            {
                MARATHON_DESTRUCTION_H(ObjectManager);
                virtual SoX::RefSharedPointer<MyCue> GetCue(uint32_t) = 0;
                virtual SoX::RefSharedPointer<Emitter> GetEmitter(uint32_t) = 0;
                virtual void Initialize() = 0;
                virtual void SetFlag(uint32_t) = 0;
            };
        }
    }
}