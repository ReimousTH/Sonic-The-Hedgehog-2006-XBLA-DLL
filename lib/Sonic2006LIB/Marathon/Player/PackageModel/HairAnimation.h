#pragma once

#include <Marathon.inl>
#include <Sox/RefSharedPointer.h>

namespace Sonicteam
{
    namespace Player
    {
        namespace PackageModel
        {
            class HairHierarchy;

            class HairAnimation
            {
            public:
                struct Vftable
                {
                    be<uint32_t> fpDestroy;
                    //...
                };

                xpointer<Vftable> m_pVftable;
                SoX::RefSharedPointer<HairHierarchy> m_spHairHierarchy;
            };
        }
    }
}