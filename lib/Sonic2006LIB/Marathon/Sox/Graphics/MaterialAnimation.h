#pragma once

#include <Marathon.inl>
#include <SoX/IResource.h>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Graphics
        {
            class MaterialAnimation : public IResource
            {
            public:
                be<float> m_Field64;
                be<float> m_Field68;
                be<float> m_Field6C;
            };

            MARATHON_ASSERT_OFFSETOF(MaterialAnimation, m_Field64, 0x64);
            MARATHON_ASSERT_OFFSETOF(MaterialAnimation, m_Field68, 0x68);
            MARATHON_ASSERT_OFFSETOF(MaterialAnimation, m_Field6C, 0x6C);
        }
    }
}