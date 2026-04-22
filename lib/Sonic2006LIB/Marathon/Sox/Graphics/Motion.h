#pragma once

#include <Marathon.inl>
#include <SoX/IResource2.h>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Graphics
        {
            class MotionMgr;

            class Motion : public IResource2<Motion, MotionMgr>
            {
            };
        }
    }
}