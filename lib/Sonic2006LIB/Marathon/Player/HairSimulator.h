#pragma once

#include <Marathon.inl>
#include <vector>
#include <SoX/Graphics/FrameObserver.h>

namespace Sonicteam
{
    namespace Player
    {
        class HairSimulator : public SoX::Graphics::FrameObserver
        {
        public:
            XMVECTOR m_vField20;
            XMVECTOR m_vField30;
            XMVECTOR m_vField40;
            XMVECTOR m_vField50;
            XMVECTOR m_vField60;
            XMVECTOR m_vField70;
            XMVECTOR m_vField80;
            XMVECTOR m_vField90;
            XMVECTOR m_vFieldA0;
            XMVECTOR m_vFieldB0;
            XMVECTOR m_vFieldC0;
            XMVECTOR m_vFieldD0;
            xpointer<void> m_pFieldE0;
            XMVECTOR m_vFieldF0;
            XMVECTOR m_vField100;
            XMVECTOR m_vField110;
            XMVECTOR m_vField120; //not sure here
        };

        MARATHON_ASSERT_SIZEOF(HairSimulator, 0x130);
    }
}