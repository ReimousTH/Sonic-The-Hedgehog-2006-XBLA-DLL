#ifndef SONICTEAM__SOX__ENGINE__GTASK
#define SONICTEAM__SOX__ENGINE__GTASK

#include <Marathon.inl>
#include <Sox/Engine/Defs.h>
#include "../MessageReceiver.h"
#include <Sox/Component.h>

namespace Sonicteam 
{
	namespace SoX 
	{
		namespace Engine 
		{
			class GTask : SoX::Component
			{
			public:

				GTask(GTask*);
				~GTask(void);
                MARATHON_DESTRUCTION_H(GTask);
                virtual char* GetType();

				be<uint32_t> m_Timestamp;
				xpointer<GTask> m_pPrevSibling;
				xpointer<GTask> m_pNextSibling;
				xpointer<GTask> m_pDependency;
				xpointer<GTask> m_pDependencies;

                class iterator
                {
                    GTask* m_pCurrent;

                public:
                    iterator(GTask* pCurrent = nullptr) : m_pCurrent(pCurrent) {}

                    GTask& operator*() const
                    {
                        return *m_pCurrent;
                    }

                    GTask* operator->() const
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

                GTask* GetFirstDependency() const
                {
                    if (!m_pDependencies)
                        return nullptr;

                    GTask* pCurrent = m_pDependencies.get();

                    while (pCurrent->m_pPrevSibling && pCurrent->m_pPrevSibling != m_pDependencies.get())
                        pCurrent = pCurrent->m_pPrevSibling.get();

                    return pCurrent;
                }

                GTask* GetLastDependency() const
                {
                    return m_pDependencies.get();
                }

                void AddChild(GTask* pChild)
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
                }

                void RemoveDependencies()
                {
                    if (m_pDependencies)
                    {
                        for (GTask::iterator it = m_pDependencies->begin(); it != m_pDependencies->end();)
                        {
                            GTask* pTask = &*it;
                            it = ++it;
                            pTask->m_pDependency = 0;
                            pTask->m_pPrevSibling = 0;
                            pTask->m_pNextSibling = 0;
                        }
                    }
                }

                void RemoveFromParent()
                {
                    GTask* pParent = m_pDependency.get();

                    if (pParent)
                    {
                        GTask* pNextSibling = m_pNextSibling.get();

                        if (pNextSibling != this)
                        {
                            if (pParent->m_pDependencies.get() == this)
                            {
                                pParent->m_pDependencies = m_pPrevSibling;

                                if (m_pNextSibling)
                                    m_pNextSibling->m_pPrevSibling = nullptr;

                                GTask* pPrevSibling = m_pPrevSibling.get();
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

                                GTask* pPrevSibling = m_pPrevSibling.get();
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

			};
		};
	}
}
#endif
