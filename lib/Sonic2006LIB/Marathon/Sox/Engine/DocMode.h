#pragma once

#include <Marathon.inl>
#include <SoX/Engine/Task.h>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Engine
        {
            class DocMode : public Task
            {
            public:
                MARATHON_INSERT_PADDING(4);
            };

            MARATHON_ASSERT_SIZEOF(DocMode, 0x50);
        }
    }
}