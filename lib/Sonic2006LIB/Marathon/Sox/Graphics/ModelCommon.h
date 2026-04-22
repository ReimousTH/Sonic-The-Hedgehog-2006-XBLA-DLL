#pragma once

#include <Marathon.inl>
#include <SoX/Graphics/Model.h>
#include <map>
#include <string>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Graphics
        {
            class ModelCommon : public Model
            {
            public:
                xpointer<void> m_Model; // (raw data), used in real-time
                std::map<std::string, uint32_t> m_BoneIndices; // ??
                std::map<std::string, uint32_t> m_MaterialIndices;
            };

            MARATHON_ASSERT_OFFSETOF(ModelCommon, m_Model, 0x7C);
            MARATHON_ASSERT_OFFSETOF(ModelCommon, m_BoneIndices, 0x80);
            MARATHON_ASSERT_OFFSETOF(ModelCommon, m_MaterialIndices, 0x8C);
            MARATHON_ASSERT_SIZEOF(ModelCommon, 0x98);
        }
    }
}