#pragma once

#include <Marathon.inl>
#include <Sox/IResource2.h>

namespace Sonicteam
{
    class ParticleContainerMgr;

    namespace Particles
    {
        class ParticleContainer : public Sonicteam::SoX::IResource2<ParticleContainer, ParticleContainerMgr>
        {
        };
    }
}