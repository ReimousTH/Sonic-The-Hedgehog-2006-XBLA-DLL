#pragma once

#include <Marathon.inl>
#include <Player/IPlugIn.h>
#include <Player/IDynamicLink.h>
#include <Player/IStepable.h>
#include <Player/IFlagCommunicator.h>
#include <Player/IVariable.h>
#include <SoX/RefSharedPointer.h>
#include <SoX/Graphics/InstanceModelCommon.h>
#include <SoX/Scenery/Clump.h>
#include <string>
#include <vector>

namespace Sonicteam
{
    namespace Player
    {
        struct ItemPoint
        {
            be<uint32_t> m_Count; // ?
            std::string m_AttachPoint; // Left_ItemPoint
            MARATHON_INSERT_PADDING(4);
            SoX::RefSharedPointer<SoX::Graphics::InstanceModelCommon> m_spInstanceModel; //player/sonic_new/so_itm_bbungle_L.xno
            SoX::RefSharedPointer<SoX::Scenery::Clump> m_spClump;    //player/sonic_new/so_itm_bbungle_L.xno
            SoX::RefSharedPointer<SoX::Scenery::Clump> m_spClump2;   //player/sonic_new/so_itm_bbungle_L.xno
        };

        MARATHON_ASSERT_SIZEOF(ItemPoint, 0x30);

        class Load;
        class RootFrame;

        class Item : public IPlugIn, public IDynamicLink, public IStepable, public IFlagCommunicator, public IVariable
        {
        public:
            xpointer<Load> m_pLoad;
            SoX::RefSharedPointer<RootFrame> m_spParentFrame;
            std::vector<ItemPoint> m_pvItemPoints;
            be<uint32_t> m_Flag; // 00 1F FF FF (not attached) 80 1F FF FF (attached)
        };

        MARATHON_ASSERT_OFFSETOF(Item, m_pLoad, 0x30);
        MARATHON_ASSERT_OFFSETOF(Item, m_spParentFrame, 0x34);
        MARATHON_ASSERT_OFFSETOF(Item, m_pvItemPoints, 0x38);
        MARATHON_ASSERT_OFFSETOF(Item, m_Flag, 0x48);
        MARATHON_ASSERT_SIZEOF(Item, 0x4C);
    }
}