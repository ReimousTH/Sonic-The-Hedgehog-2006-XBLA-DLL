#pragma once

#include <SoX/RefSharedPointer.h>
#include <cstring> 
#include "Frame.h"

namespace Sonicteam
{
    namespace SoX
    {
        namespace Graphics
        {
            inline void FrameSynchronizer::SynchronizeFrames()
            {
                for (int i = 0; i < m_Count && m_Count; i++)
                {
                    for (SoX::LinkedList<Frame>::iterator it = m_aollFrame[i].begin(); it != m_aollFrame[i].end();)
                    {
                        SoX::LinkedList<Frame>* pllFrames = (SoX::LinkedList<Frame>*)&it;
                        //Proces, Missing  sub_824FA068() XBLA
                        ++it;
                        pllFrames->ResetListFast();

                    }
                }
            }

            inline void FrameSynchronizer::RemoveFromPriorityQueue(Frame* pFrame)
            {
                // Exact sub_825942F0 implementation
                if (pFrame->m_lnFrameSync.m_pNext)
                {
                    pFrame->m_lnFrameSync.m_pPrev->m_pNext = pFrame->m_lnFrameSync.m_pNext;
                    pFrame->m_lnFrameSync.m_pNext->m_pPrev = pFrame->m_lnFrameSync.m_pPrev;
                    pFrame->m_lnFrameSync.m_pNext = nullptr;
                    pFrame->m_lnFrameSync.m_pPrev = nullptr;
                }

                if (pFrame->m_lnFrame2.m_pNext)
                {
                    pFrame->m_lnFrame2.m_pPrev->m_pNext = pFrame->m_lnFrame2.m_pNext;
                    pFrame->m_lnFrame2.m_pNext->m_pPrev = pFrame->m_lnFrame2.m_pPrev;
                    pFrame->m_lnFrame2.m_pNext = nullptr;
                    pFrame->m_lnFrame2.m_pPrev = nullptr;
                }

                uint32_t queueSize = m_Count;
                uint32_t foundIndex = queueSize;

                if (queueSize != 0)
                {
                    SoX::LinkedList<Frame>* currentList = m_aollFrame.begin();
                    for (uint32_t i = 0; i < queueSize; ++i, ++currentList)
                    {
                        if (currentList == pFrame->m_plnSyncronizerParent)
                        {
                            foundIndex = i;
                            break;
                        }
                    }
                }

                if (foundIndex < queueSize)
                {
                    m_aPriorityCounts[foundIndex] = m_aPriorityCounts[foundIndex] - 1;
                }

                pFrame->m_pSynchronizer = nullptr;
                pFrame->m_plnSyncronizerParent = nullptr;
            }

            // Recreation (sub_825949F0)
            inline void Frame::AddChild(Frame* pChild)
            {
                if (!pChild) return;

                Frame* child = pChild;

                // Link into child list
                if (m_pFirstChild)
                {
                    Frame* lastChild = m_pFirstChild->m_pPrevSibling;
                    lastChild->m_pNextSibling = child;
                    child->m_pPrevSibling = lastChild;
                }
                else
                {
                    m_pFirstChild = child;
                }

                m_pFirstChild->m_pPrevSibling = child;

                // Update ownership recursively
                child->UpdateOwnerRecursive(m_pOwner);

                // Handle synchronizer
                child->m_pParent = this;
                if (child->m_pSynchronizer)
                {
                    child->m_Flag = child->m_Flag | 0x10u;
                    child->m_pSynchronizer->RemoveFromPriorityQueue(child);
                }

                // Link into owner's frame list if needed
                if (child->m_pOwner && !child->m_pOwner->m_lnFrame2.m_pNext)
                {
                    LinkNode<Frame>* frameNode = child->m_pOwner->m_plnSyncronizerParent;
                    if (frameNode) {
                        LinkNode<Frame>& frame2 = child->m_pOwner->m_lnFrame2;
                        frame2.m_pPrev = frameNode;
                        frame2.m_pNext = frameNode->m_pNext;
                        frameNode->m_pNext->m_pPrev = &frame2;
                        frameNode->m_pNext = &frame2;
                    }
                }

                child->m_pOwner->m_Flag = child->m_pOwner->m_Flag | 1u;
                child->m_Flag = child->m_Flag | 2u;
                child->m_ReferenceCount = child->m_ReferenceCount + 1;
            }

            inline void Frame::RemoveChild(Frame* pChild)
            {
                if (!pChild || pChild->m_pParent != this) return;
                pChild->RemoveFromParent();
            }

            // sub_82594590 (Recreation)
            inline void Frame::RemoveFromParent()
            {
                if (!m_pParent) return;

                Frame* parent = m_pParent;

                FrameSynchronizer* pSynchronizer = 0;
                if (((m_Flag >> 4) & 1) != 0)
                {
                    Frame* pOwner = this->m_pOwner;
                    m_Flag = m_Flag & ~0x10;
                    pSynchronizer = pOwner->m_pSynchronizer;
                }

                xpointer<Frame> pNextSibling = this->m_pNextSibling;
                if (!pNextSibling)
                    pNextSibling = m_pParent->m_pFirstChild;
                pNextSibling->m_pPrevSibling = this->m_pPrevSibling;
                xpointer<Frame> v7 = this->m_pParent;
                xpointer<Frame> v8 = this->m_pNextSibling;
                if (v7->m_pFirstChild == this)
                    v7->m_pFirstChild = v8;
                else
                    this->m_pPrevSibling->m_pNextSibling = v8;
                this->m_pParent = 0;
                this->m_pNextSibling = 0;
                this->m_pPrevSibling = this;

                UpdateOwnerRecursive(this);

                if (pSynchronizer)
                {
                    SoX::RefSharedPointer<Frame> refPtr(this);
                    BranchTo(0x82507150,uint32_t, pSynchronizer, &refPtr);
                }

                Release();
            }

            inline void Frame::DestroyChildren()
            {
                Frame* child = m_pFirstChild;
                while (child)
                {
                    Frame* next = child->m_pNextSibling;
                    child->RemoveFromParent();
                    child = next;
                }
                m_pFirstChild = nullptr;
            }

            inline void Frame::DestroyFrame()
            {
                DestroyChildren();
                RemoveFromParent();

                // Clear remaining references
                m_pOwner = nullptr;
                m_pSynchronizer = nullptr;
                m_plnSyncronizerParent = nullptr;
            }

            inline void Frame::UpdateOwnerRecursive(Frame* pNewOwner)
            {
                m_pOwner = pNewOwner;
                Frame* child = m_pFirstChild;
                while (child)
                {
                    child->UpdateOwnerRecursive(pNewOwner);
                    child = child->m_pNextSibling;
                }
            }

            inline const char* Frame::GetName()
            {
                return "Frame";
            }

            inline xpointer<Frame> Frame::GetRoot()
            {
                Frame* root = this;
                while (root->m_pParent)
                {
                    root = root->m_pParent;
                }
                return root;
            }

            inline void Frame::SetSynchronizer(FrameSynchronizer* pSync)
            {
                if (m_pSynchronizer != pSync)
                {
                    m_pSynchronizer = pSync;
                }
            }

            inline void Frame::SetTransform(XMMATRIX* matrix)
            {
               
            }

            inline void Frame::SetTransform(XMMATRIX& matrix)
            {
                
            }

            inline void Frame::UpdateTransform(XMMATRIX& matrix)
            {
                SetTransform(matrix);
                Frame* owner = m_pOwner;
                if (owner && !owner->m_lnFrame2.m_pNext && owner->m_plnSyncronizerParent)
                {
                    owner->m_plnSyncronizerParent->InsertFast(&owner->m_lnFrame2);
                }
                if (owner)
                {
                    owner->m_Flag = owner->m_Flag | 1;
                }
                m_Flag = m_Flag | 2;
            }

            inline void Frame::DetachFromParent()
            {
                if (!m_pParent) return;

                Frame* parent = m_pParent;
                bool wasSynced = (m_Flag & FrameFlags::FLAG_SYNC_ENABLED) != 0;

                if (wasSynced)
                {
                    m_Flag = m_Flag & ~FrameFlags::FLAG_SYNC_ENABLED;
                }

                // Remove from circular sibling list
                if (m_pPrevSibling == this)
                {
                    // Only child case
                    parent->m_pFirstChild = nullptr;
                }
                else {
                    // Update sibling pointers
                    m_pPrevSibling->m_pNextSibling = m_pNextSibling;
                    m_pNextSibling->m_pPrevSibling = m_pPrevSibling;

                    // Update parent's first child if we were first
                    if (parent->m_pFirstChild == this) {
                        parent->m_pFirstChild = m_pNextSibling;
                    }
                }

                // Reset our sibling pointers to self
                m_pParent = nullptr;
                m_pNextSibling = nullptr;
                m_pPrevSibling = this;

                UpdateOwnerRecursive(this);

                if (wasSynced)
                {
                    SoX::RefSharedPointer<Frame> refPtr(this);
                    BranchTo(0x82507150, uint32_t, m_pOwner->m_pSynchronizer, refPtr.get());
                }

                Release();
            }

            inline void Frame::DestroyFrameHierarchy()
            {
                // Destroy all children recursively
                DestroyChildren();

                // Detach from parent
                DetachFromParent();

                // Clear synchronization links
                if (m_pSynchronizer)
                {
                    m_pSynchronizer->RemoveFromPriorityQueue(this);
                }

                // Reset all links
                m_lnFrameSync.Reset();
                m_lnFrame2.Reset();
                m_llFrameObserver.Reset();

                m_pOwner = nullptr;
                m_pSynchronizer = nullptr;
                m_plnSyncronizerParent = nullptr;
            }

            inline void Frame::ForEach(void (*function)(Frame*))
            {
              
                for (Frame* child = m_pFirstChild.get(); child; child = child->m_pNextSibling.get())
                {
                    function(child);
                    child->ForEach(function);
                }
            }

            template<typename Functor>
            void ForEach(Functor function)
            {

                for (Frame* child = m_pFirstChild.get(); child; child = child->m_pNextSibling.get())
                {
                    function(child);
                    child->ForEach(function);
                }
            }

            inline Frame* Frame::FindNode(const char* name)
            {
                // Check current node first
                if (const char* currentName = GetName())
                {
                    if (strcmp(currentName, name) == 0)
                        return this;
                }

                // Depth-first search through children
                for (Frame* child = m_pFirstChild.get(); child; child = child->m_pNextSibling.get())
                {
                    if (Frame* found = child->FindNode(name))
                        return found;
                }

                return nullptr;
            }

            inline Frame* Frame::FindChild(const char* name)
            {
                // Search only direct children (non-recursive)
                for (Frame* child = m_pFirstChild.get(); child; child = child->m_pNextSibling.get())
                {
                    if (const char* childName = child->GetName())
                    {
                        if (strcmp(childName, name) == 0)
                            return child;
                    }
                }
                return nullptr;
            }

            inline Frame* Frame::FindNode(Frame* pFrame)
            {
                if (!pFrame) return nullptr;

                // Check current node
                if (this == pFrame)
                    return this;

                // Recursively search children
                for (Frame* child = m_pFirstChild.get(); child; child = child->m_pNextSibling.get())
                {
                    if (Frame* found = child->FindNode(pFrame))
                        return found;
                }

                return nullptr;
            }

            inline bool Frame::ContainsFrame(Frame* pFrame)
            {
                if (!pFrame) return false;

                if (this == pFrame)
                    return true;

                for (Frame* child = m_pFirstChild.get(); child; child = child->m_pNextSibling.get())
                {
                    if (child->ContainsFrame(pFrame))
                        return true;
                }

                return false;
            }

            inline bool Frame::HasDirectChild(Frame* pFrame)
            {
                if (!pFrame) return false;

                for (Frame* child = m_pFirstChild.get(); child; child = child->m_pNextSibling.get())
                {
                    if (child == pFrame)
                        return true;
                }

                return false;
            }

            inline void Frame::RemoveChildByName(const char* name)
            {
                Frame* child = FindChild(name);
                if (child)
                {
                    RemoveChild(child);
                }
            }

            inline void Frame::RemoveNodeByName(const char* name)
            {
                Frame* node = FindNode(name);
                if (node && node != this)
                {
                    node->RemoveFromParent();
                }
            }
        }
    }
}