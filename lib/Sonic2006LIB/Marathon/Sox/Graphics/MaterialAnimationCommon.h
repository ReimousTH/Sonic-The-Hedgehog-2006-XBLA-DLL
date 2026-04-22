#pragma once

#include <Marathon.inl>
#include <SoX/Graphics/AnimationCore/CoreTemplate.h>
#include <SoX/Graphics/MaterialAnimation.h>
#include <Boost/shared_ptr.hpp>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Graphics
        {
            class MaterialAnimationObject;

            class MaterialAnimationCommon : public MaterialAnimation
            {
            public:
                boost::shared_ptr<AnimationCore::CoreTemplate<MaterialAnimationObject>> m_spMaterialAnimationObject;
            };


            MARATHON_ASSERT_OFFSETOF(MaterialAnimationCommon, m_spMaterialAnimationObject, 0x70);
            MARATHON_ASSERT_SIZEOF(MaterialAnimationCommon, 0x78);
        }
    }
}