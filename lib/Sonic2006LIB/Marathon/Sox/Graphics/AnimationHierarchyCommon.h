#pragma once

#include <Marathon.inl>
#include <Sox/Graphics/MotionHierarchy.h>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Graphics
        {
            class AnimationHierarchyCommon : public MotionHierarchy
            {
                MARATHON_INSERT_PADDING(0x48);
            };
        }
    }
}