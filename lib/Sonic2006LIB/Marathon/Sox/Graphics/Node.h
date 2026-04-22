#pragma once

#include <Marathon.inl>
#include <SoX/Graphics/Frame.h>
#include <SoX/RefSharedPointer.h>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Graphics
        {
            class SharedObject;

            class Node : public Frame
            {
            public:
                MARATHON_INSERT_PADDING(8);
                xpointer<const char> m_pField54; // name, not sure
                SoX::RefSharedPointer<SharedObject> m_spSharedObject;
                MARATHON_INSERT_PADDING(4); // align
                MARATHON_INSERT_PADDING(0x1B0);
            };

            MARATHON_ASSERT_SIZEOF(Node, 0x210);
            MARATHON_ASSERT_OFFSETOF(Node, m_pField54, 0x54);
            MARATHON_ASSERT_OFFSETOF(Node, m_spSharedObject, 0x58);
        }
    }
}
