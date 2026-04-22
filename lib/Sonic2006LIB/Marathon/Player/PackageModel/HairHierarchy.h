#pragma once

#include <Marathon.inl>
#include <Sox/Graphics/AnimationHierarchyCommon.h>
#include <Sox/RefSharedPointer.h>
#include <vector>
#include <Boost/shared_ptr.hpp>

namespace Sonicteam
{
    namespace Player
    {
        class HairSimulator;

        namespace PackageModel
        {
            class HairHierarchy : public SoX::Graphics::AnimationHierarchyCommon
            {
            public:
                struct Vftable
                {
                    be<uint32_t> fpDestroy;
                    //...
                };

                std::vector<boost::shared_ptr<HairSimulator>> m_vspHairSimulators;
            };
        }
    }
}