#pragma once

#include <Marathon.inl>
#include <Player/IExportExternalFlag.h>
#include <Player/IPlugIn.h>
#include <SoX/Graphics/Frame.h>

namespace Sonicteam
{
    namespace Player
    {
        class RootFrame : public SoX::Graphics::Frame, public IPlugIn, public IExportExternalFlag
        {
        public:
            XMMATRIX m_Field70;
            XMMATRIX m_FieldB0;
            XMVECTOR m_PositionF0;
            XMVECTOR m_Field100;
            XMVECTOR m_Impulse;
            MARATHON_INSERT_PADDING(0x30);
            be<uint64_t> m_ExternalFlag;
        };

        MARATHON_ASSERT_OFFSETOF(RootFrame, m_Field70, 0x70);
        MARATHON_ASSERT_OFFSETOF(RootFrame, m_FieldB0, 0xB0);
        MARATHON_ASSERT_OFFSETOF(RootFrame, m_PositionF0, 0xF0);
        MARATHON_ASSERT_OFFSETOF(RootFrame, m_Field100, 0x100);
        MARATHON_ASSERT_OFFSETOF(RootFrame, m_Impulse, 0x110);
        MARATHON_ASSERT_OFFSETOF(RootFrame, m_ExternalFlag, 0x150);
        MARATHON_ASSERT_SIZEOF(RootFrame, 0x160);
    }
}