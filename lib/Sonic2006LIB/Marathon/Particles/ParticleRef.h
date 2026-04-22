#pragma once

#include <Marathon.inl>

namespace Sonicteam
{
    namespace Particles
    {
        class ParticleRef
        {
        public:


            virtual void Destroy(uint8_t flags = 1)
            {
                
            }

            virtual void Release()
            {
              
            }
           uint32_t m_ReferenceCount;
        };
    }
}