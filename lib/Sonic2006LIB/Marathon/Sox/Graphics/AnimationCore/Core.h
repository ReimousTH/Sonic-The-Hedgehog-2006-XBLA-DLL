#pragma once

#include <Marathon.inl>
#include <SoX/Graphics/Device.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include <map>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Graphics
        {
            namespace AnimationCore
            {
                class SubAnimation;
                class ConstRepeat; // IRepeat

                class Core
                {
                public:
                    xpointer<void> m_pVftable;
                    std::map<be<uint32_t>, boost::shared_ptr<SubAnimation>> m_mspSubAnimations;
                    std::map<std::string, boost::shared_ptr<void*>> m_mspField10; //stdx::string, (not sure next), but 8 bytes left
                    boost::shared_ptr<ConstRepeat> m_spCRepeat;
                };

                MARATHON_ASSERT_OFFSETOF(Core, m_pVftable, 0);
                MARATHON_ASSERT_OFFSETOF(Core, m_mspSubAnimations, 4);
                MARATHON_ASSERT_OFFSETOF(Core, m_mspField10, 0x10);
                MARATHON_ASSERT_OFFSETOF(Core, m_spCRepeat, 0x1C);
                MARATHON_ASSERT_SIZEOF(Core, 0x24);
            }
        }
    }
}