#pragma once

#include <Marathon.inl>
#include <SoX/Component.h>
#include <SoX/LinkNode.h>
#include <SoX/MessageReceiver.h>

namespace Sonicteam
{
    namespace SoX
    {
        namespace Engine
        {

            class Doc;

            class Task : public Component, public MessageReceiver
            {
            public:
                
                Task();
                Task(Task*);
                ~Task();

                MARATHON_DESTRUCTION_H(Task);

                virtual int OnMessageRecieved(SoXMessageType);
                virtual void OnStep(float);

                char m_Flag1;
                MARATHON_INSERT_PADDING(3);
                be<uint32_t> m_Timestamp;
                xpointer<Task> m_pPrevSibling;
                xpointer<Task> m_pNextSibling;
                xpointer<Task> m_pDependency;
                xpointer<Task> m_pDependencies;
                xpointer<Doc> m_pDoc;
                LinkNode<Task> m_llTask;

                class iterator
                {
                    Task* m_pCurrent;

                public:
                    iterator(Task* pCurrent = nullptr) : m_pCurrent(pCurrent) {}

                    Task& operator*() const
                    {
                        return *m_pCurrent;
                    }

                    Task* operator->() const
                    {
                        return m_pCurrent;
                    }

                    iterator& operator++()
                    {
                        if (m_pCurrent)
                            m_pCurrent = m_pCurrent->m_pPrevSibling.get();

                        return *this;
                    }

                    bool operator!=(const iterator& other) const
                    {
                        return m_pCurrent != other.m_pCurrent;
                    }
                };

                iterator begin()
                {
                    return iterator(this);
                }

                iterator end()
                {
                    return iterator(nullptr);
                }

                Task* GetFirstDependency() const
                {
                    if (!m_pDependencies)
                        return nullptr;

                    Task* pCurrent = m_pDependencies.get();

                    while (pCurrent->m_pPrevSibling && pCurrent->m_pPrevSibling != m_pDependencies.get())
                        pCurrent = pCurrent->m_pPrevSibling.get();

                    return pCurrent;
                }

                Task* GetLastDependency() const
                {
                    return m_pDependencies.get();
                }

                void AddChild(Task* pChild)
                {
                    if (!pChild)
                        return;

                    pChild->m_pDependency = this;

                    if (m_pDependencies)
                    {
                        pChild->m_pNextSibling = m_pDependencies->m_pNextSibling;
                        pChild->m_pPrevSibling = m_pDependencies.get();

                        if (m_pDependencies->m_pNextSibling)
                        {
                            m_pDependencies->m_pNextSibling->m_pPrevSibling = pChild;
                        }

                        m_pDependencies->m_pNextSibling = pChild;
                    }
                    else
                    {
                        pChild->m_pNextSibling = pChild;
                        pChild->m_pPrevSibling = pChild;
                        m_pDependencies = pChild;  
                    }
                    pChild->m_pDoc = this->m_pDoc;
                }

                void RemoveDependencies()
                {
                    if (m_pDependencies)
                    {
                        for (Task::iterator it = m_pDependencies->begin(); it != m_pDependencies->end();)
                        {
                            Task* pTask = &*it;
                            it = ++it;
                            pTask->m_pDependency = 0;
                            pTask->m_pPrevSibling = 0;
                            pTask->m_pNextSibling = 0;
                        }
                    }
                }

                void RemoveFromParent()
                {
                    Task* pParent = m_pDependency.get();

                    if (pParent)
                    {
                        Task* pNextSibling = m_pNextSibling.get();

                        if (pNextSibling != this)
                        {
                            if (pParent->m_pDependencies.get() == this)
                            {
                                pParent->m_pDependencies = m_pPrevSibling;

                                if (m_pNextSibling)
                                    m_pNextSibling->m_pPrevSibling = nullptr;

                                Task* pPrevSibling = m_pPrevSibling.get();
                                if (!pPrevSibling)
                                {
                                    m_pPrevSibling = nullptr;
                                    m_pNextSibling = nullptr;
                                    m_pDependency = nullptr;
                                    return;
                                }
                            }
                            else
                            {
                                if (m_pNextSibling)
                                    m_pNextSibling->m_pPrevSibling = m_pPrevSibling;

                                Task* pPrevSibling = m_pPrevSibling.get();
                                if (!pPrevSibling)
                                {
                                    pPrevSibling = pParent->m_pDependencies.get();
                                }
                            }

                            if (m_pPrevSibling)
                                m_pPrevSibling->m_pNextSibling = m_pNextSibling;
                        }
                        else
                        {
                            pParent->m_pDependencies = nullptr;
                        }
                    }

                    m_pPrevSibling = nullptr;
                    m_pNextSibling = nullptr;
                    m_pDependency = nullptr;
                }

                Task* Find(const char* name)
                {
                    if (!m_pDependencies)
                        return 0;

                    for (iterator it = m_pDependencies->begin(); it != m_pDependencies->end(); ++it)
                    {
                        //HardwareBreakpoint(__FILE__, __LINE__, "%s", (const char*)it->GetType());
                        if (strcmp(it->GetType(), name) == 0)
                        {
                            return &*it;
                        }
                      
                    }
                    return 0;
                }

                template <typename T>
                T* GetDoc()
                {
                    return (T*)m_pDoc.get();
                }
            };

            MARATHON_ASSERT_OFFSETOF(Task, m_Flags, 0x24);
            MARATHON_ASSERT_OFFSETOF(Task, m_Timestamp, 0x28);
            MARATHON_ASSERT_OFFSETOF(Task, m_pPrevSibling, 0x2C);
            MARATHON_ASSERT_OFFSETOF(Task, m_pNextSibling, 0x30);
            MARATHON_ASSERT_OFFSETOF(Task, m_pDependency, 0x34);
            MARATHON_ASSERT_OFFSETOF(Task, m_pDependencies, 0x38);
            MARATHON_ASSERT_OFFSETOF(Task, m_pDoc, 0x3C);
            MARATHON_ASSERT_OFFSETOF(Task, m_llTask, 0x40);
            MARATHON_ASSERT_SIZEOF(Task, 0x4C);
        }
    }
}