#pragma once

#include <Marathon.inl>
#include <boost/shared_ptr.hpp>
#include <Actor.h>

namespace Sonicteam
{
    namespace Camera
    {
        class CameraModeManager;

        class m_pCameraman : public Actor
        {
        public:
            MARATHON_INSERT_PADDING(0x24);
            boost::shared_ptr<CameraModeManager> m_spCameraModeManager;
            MARATHON_INSERT_PADDING(0x2C);
            float m_FOV;
            MARATHON_INSERT_PADDING(0x0C);
        };

        MARATHON_ASSERT_OFFSETOF(m_pCameraman, m_spCameraModeManager, 0x7C);
        MARATHON_ASSERT_OFFSETOF(m_pCameraman, m_FOV, 0xB0);
        MARATHON_ASSERT_SIZEOF(m_pCameraman, 0xC0);
    };
}