#pragma once

#include <Marathon.inl>
#include <SoX/Graphics/Frame.h>
#include <string>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Graphics
        {
            class FrameGP : public Frame
            {
            public:
                MARATHON_INSERT_PADDING(0x84);
                std::string m_AttachPoint;
                MARATHON_INSERT_PADDING(4);
            };

            MARATHON_ASSERT_OFFSETOF(FrameGP, m_AttachPoint, 0xD0);
            MARATHON_ASSERT_SIZEOF(FrameGP, 0xF0);
        }
    }
}