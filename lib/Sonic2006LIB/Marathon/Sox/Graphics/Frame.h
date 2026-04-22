#pragma once

#include <Marathon.inl>
#include <SoX/RefCountObject.h>
#include <SoX/LinkNode.h>
//#include <SoX/Math/Math.h>
#include <SoX/Array.h>
#include <SoX/Graphics/Frame.h>

#include <SoX/Graphics/FrameObserver.h>

namespace Sonicteam
{
    namespace SoX
    {

        namespace Graphics
        {

            class Frame;

            class FrameSynchronizer
            {
            public:
                void* m_pVftable;
                SoX::LinkedList<Frame> m_llFrames;
                SoX::ArrayOld<SoX::LinkedList<Frame>> m_aollFrame; // Index = FrameSyncronizerID
                uint32_t* m_aPriorityCounts;
                uint32_t m_Count;

                FrameSynchronizer(uint32_t count)
                {
                    m_pVftable = nullptr;
                    m_llFrames.m_pPrev = &m_llFrames;
                    m_llFrames.m_pNext = &m_llFrames;
                    m_Count = count;
                }
                FrameSynchronizer()
                {

                }

                void SynchronizeFrames();
                void RemoveFromPriorityQueue(Frame* pFrame);
            };

            MARATHON_ASSERT_OFFSETOF(FrameSynchronizer, m_llFrames, 4);
            MARATHON_ASSERT_OFFSETOF(FrameSynchronizer, m_aollFrame, 0x10);
            MARATHON_ASSERT_OFFSETOF(FrameSynchronizer, m_aPriorityCounts, 0x14);
            MARATHON_ASSERT_OFFSETOF(FrameSynchronizer, m_Count, 0x18);

            class Frame : public RefCountObject
            {
            public:
                enum FrameFlags
                {
                    FLAG_ACTIVE = 0x1,
                    FLAG_VISIBLE = 0x2,
                    FLAG_SYNC_ENABLED = 0x10,
                    FLAG_DIRTY = 0x20,
                    FLAG_IN_SYNC_LIST = 0x40
                };

                uint32_t m_Flag;
                xpointer<Frame> m_pOwner;
                xpointer<Frame> m_pParent;
                xpointer<Frame> m_pNextSibling;
                xpointer<Frame> m_pPrevSibling;
                xpointer<Frame> m_pFirstChild;
                LinkNode<FrameObserver> m_llFrameObserver; // For Particles
                LinkNode<Frame>* m_plnSyncronizerParent;
                FrameSynchronizer* m_pSynchronizer;
                LinkNode<Frame> m_lnFrameSync;
                LinkNode<Frame> m_lnFrame2;

                MARATHON_DESTRUCTION_H(Frame);

                virtual void Func4()
                {
                }

                virtual const char* GetName();

                virtual XMMATRIX FuncC()
                {
                    return XMMatrixIdentity();
                }

                virtual void Func10()
                {

                }

                virtual void Func14()
                {

                }

                virtual void Func18(XMMATRIX* transform)
                {

                }

                // Core hierarchy methods
                void AddChild(Frame* pChild);
                void RemoveChild(Frame* pChild);
                void RemoveFromParent();
                void DestroyFrame();
                bool Func8();

                // Transform methods
                void SetTransform(XMMATRIX* matrix);
                void SetTransform(XMMATRIX& matrix);
                void UpdateTransform(XMMATRIX& matrix);

                // Utility methods
                bool IsAttached() const { return m_pParent != 0; }
                bool HasChildren() const { return m_pFirstChild != 0; }
                xpointer<Frame> GetRoot();
                void SetSynchronizer(FrameSynchronizer* pSync);

                void UpdateOwnerRecursive(Frame* pNewOwner);
                void DestroyChildren();

                void DetachFromParent();
                void DestroyFrameHierarchy();

                void ForEach(void (*function)(Frame*));

                template<typename Functor>
                void ForEach(Functor function);

                Frame* FindNode(const char* name);

                Frame* FindChild(const char* name);
                Frame* FindNode(Frame* pFrame);
                bool ContainsFrame(Frame* pFrame);
                bool HasDirectChild(Frame* pFrame);
                void RemoveChildByName(const char* name);
                void RemoveNodeByName(const char* name);
            };

            MARATHON_ASSERT_SIZEOF(Frame, 0x4C);
            MARATHON_ASSERT_OFFSETOF(Frame, m_Flag, 8);
            MARATHON_ASSERT_OFFSETOF(Frame, m_pOwner, 0xC);
            MARATHON_ASSERT_OFFSETOF(Frame, m_pParent, 0x10);
            MARATHON_ASSERT_OFFSETOF(Frame, m_pNextSibling, 0x14);
            MARATHON_ASSERT_OFFSETOF(Frame, m_pPrevSibling, 0x18);
            MARATHON_ASSERT_OFFSETOF(Frame, m_pFirstChild, 0x1C);
            MARATHON_ASSERT_OFFSETOF(Frame, m_llFrameObserver, 0x20);
            MARATHON_ASSERT_OFFSETOF(Frame, m_plnSyncronizerParent, 0x2C);
            MARATHON_ASSERT_OFFSETOF(Frame, m_pSynchronizer, 0x30);
            MARATHON_ASSERT_OFFSETOF(Frame, m_lnFrameSync, 0x34);
            MARATHON_ASSERT_OFFSETOF(Frame, m_lnFrame2, 0x40);
        }
    }
}

#include "Frame.inl"