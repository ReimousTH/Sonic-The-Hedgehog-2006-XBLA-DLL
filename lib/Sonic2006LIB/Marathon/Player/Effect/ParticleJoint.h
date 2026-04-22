#pragma once

#include <Marathon.inl>
#include <xtl.h>
#include <SoX/Graphics/FrameObserver.h>
#include <Particles/Particle.h>
#include <SoX/LinkNode.h>

namespace Sonicteam
{
    namespace Player
    {
        namespace Effect
        {

            class ParticleJoint : public SoX::Graphics::FrameObserver
            {
            public:
                xpointer<Particles::Particle> m_pParticle;
                bool m_IsAcroarts;
                bool m_IsReverse;
                uint32_t m_Direction;
                const char* m_pNodeName;
                XMVECTOR m_Offset;
                XMVECTOR m_Position;
                XMVECTOR m_Rotation;
            };

            MARATHON_ASSERT_OFFSETOF(ParticleJoint, m_pParticle, 0x14);
            MARATHON_ASSERT_OFFSETOF(ParticleJoint, m_IsAcroarts, 0x18);
            MARATHON_ASSERT_OFFSETOF(ParticleJoint, m_IsReverse, 0x19);
            MARATHON_ASSERT_OFFSETOF(ParticleJoint, m_Direction, 0x1C);
            MARATHON_ASSERT_OFFSETOF(ParticleJoint, m_pNodeName, 0x20);
            MARATHON_ASSERT_OFFSETOF(ParticleJoint, m_Offset, 0x30);
            MARATHON_ASSERT_OFFSETOF(ParticleJoint, m_Position, 0x40);
            MARATHON_ASSERT_OFFSETOF(ParticleJoint, m_Rotation, 0x50);
            MARATHON_ASSERT_SIZEOF(ParticleJoint, 0x60);
        };
    }
}