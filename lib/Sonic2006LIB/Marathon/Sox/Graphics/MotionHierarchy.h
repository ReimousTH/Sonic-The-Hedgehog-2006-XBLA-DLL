#pragma once

#include <Marathon.inl>
#include <Sox/Graphics/IAnimation.h>
#include <Sox/Graphics/IAnimationBlend.h>
#include <Sox/RefCountObject.h>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Graphics
        {
            class MotionHierarchy : public IAnimation, public IAnimationBlend<MotionHierarchy>, public SoX::RefCountObject
            {
                MARATHON_INSERT_PADDING(0x14);
            };
        }
    }
}