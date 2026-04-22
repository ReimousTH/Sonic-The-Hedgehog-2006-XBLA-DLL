#pragma once

namespace Sonicteam
{
    namespace SoX
    {
        template <typename T>
        class ILinkNode
        {
        public:
            T* m_pPrev;
            T* m_pNext;

            void InsertFast(T* nextNode)
            {
                nextNode->m_pPrev = static_cast<T*>(this);
                nextNode->m_pNext = this->m_pNext;

                this->m_pNext->m_pPrev = nextNode;
                this->m_pNext = nextNode;
            }

            void Insert(T* nextNode)
            {
                if (!nextNode) return;

                nextNode->m_pPrev = static_cast<T*>(this);
                nextNode->m_pNext = this->m_pNext;

                if (this->m_pNext)
                    this->m_pNext->m_pPrev = nextNode;

                this->m_pNext = nextNode;
            }

            void ResetListFast()
            {
                T* current = this->m_pNext;

                while (current && current != this)
                {
                    T* next = current->m_pNext;
                    current->m_pPrev = 0;
                    current->m_pNext = 0;
                    current = next;
                }

                if (this->m_pPrev) this->m_pPrev->m_pNext = this->m_pNext;
                if (this->m_pNext) this->m_pNext->m_pPrev = this->m_pPrev;
            }

            void ResetQuick()
            {
                if (this->m_pPrev) this->m_pPrev->m_pNext = this->m_pNext;
                if (this->m_pNext) this->m_pNext->m_pPrev = this->m_pPrev;
            }

            void Reset()
            {
                if (this->m_pPrev) this->m_pPrev->m_pNext = this->m_pNext;
                if (this->m_pNext) this->m_pNext->m_pPrev = this->m_pPrev;
                this->m_pPrev = 0;
                this->m_pNext = 0;
            }

            void ResetFast()
            {
                this->m_pPrev->m_pNext = this->m_pNext;
                this->m_pNext->m_pPrev = this->m_pPrev;
                this->m_pPrev = 0;
                this->m_pNext = 0;
            }

            void InitializeLink()
            {
                this->m_pPrev = 0;
                this->m_pNext = 0;
            }

            void InitializeList()
            {
                this->m_pNext = static_cast<T*>(this);
                this->m_pPrev = static_cast<T*>(this);
            }
        };

        template <typename T>
        class LinkNode : public ILinkNode<LinkNode<T>>
        {
        public:
            T* m_pThis;

            // Simple iterator implementation for C++2003
            class iterator
            {
                friend class LinkNode<T>;
                LinkNode<T>* m_pCurrent;
                LinkNode<T>* m_pStart;

            public:
                iterator(LinkNode<T>* start = 0)
                    : m_pCurrent(0)
                    , m_pStart(start)
                {
                    if (m_pStart && m_pStart->m_pNext != m_pStart)
                    {
                        m_pCurrent = static_cast<LinkNode<T>*>(m_pStart->m_pNext);
                    }
                }

                iterator& operator++()
                {
                    if (m_pCurrent && m_pCurrent->m_pNext)
                    {
                        m_pCurrent = static_cast<LinkNode<T>*>(m_pCurrent->m_pNext);

                        // Stop if we reach the start node again
                        if (m_pCurrent == m_pStart)
                            m_pCurrent = 0;
                    }
                    else
                    {
                        m_pCurrent = 0;
                    }
                    return *this;
                }

                iterator operator++(int)
                {
                    iterator temp = *this;
                    ++(*this);
                    return temp;
                }

                T* operator*() const
                {
                    return m_pCurrent ? m_pCurrent->m_pThis : 0;
                }

                T* operator->() const
                {
                    return m_pCurrent ? m_pCurrent->m_pThis : 0;
                }

                bool operator==(const iterator& other) const
                {
                    return m_pCurrent == other.m_pCurrent;
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
                return iterator(0);
            }

            // Typedefs for backward compatibility
            typedef LinkNode<T> IType;
            typedef ILinkNode<LinkNode<T> > INodeType;
            typedef T pThisType;

            void InitializeLink(T* thisNode)
            {
                ILinkNode<LinkNode<T>>::InitializeLink();
                this->m_pThis = thisNode;
            }

            void ResetListFast()
            {
                for (iterator it = begin(); it != end(); )
                {
                    IType* node = it.m_pCurrent;
                    ++it;               
                    node->m_pPrev = 0;  
                    node->m_pNext = 0;  
                }
                ResetQuick();
            }

            void ResetList()
            {
                for (iterator it = begin(); it != end(); )
                {
                    IType* node = it.m_pCurrent;
                    ++it;               
                    node->m_pPrev = 0;  
                    node->m_pNext = 0;  
                    node->m_pThis = 0;
                }
                ResetQuick();
            }

            void ForEach(void (*function)(T*))
            {
                for (iterator it = begin(); it != end(); )
                {
                    T* pNode = *it;
                    ++it; 
                    if (pNode)
                        function(pNode);
                }
            }

            template<typename Functor>
            void ForEach(Functor function)
            {
                for (iterator it = begin(); it != end(); )
                {
                    T* pNode = *it;
                    ++it; 
                    if (pNode)
                        function(pNode);
                }
            }

        };

        template <typename T>
        class LinkedList : public LinkNode<T>
        {
        };

        template <typename T>
        class LinkedListExtended : public LinkNode<T>
        {
        public:
            uint32_t m_Flags;

            void InitializeList()
            {
                this->m_pNext = static_cast<LinkNode<T>*>(this);
                this->m_pPrev = static_cast<LinkNode<T>*>(this);
                this->m_Flags = 0;
            }
        };

        template <typename T, typename Y = T>
        class LinkRef
        {
        public:
            Y* m_pElement;  // Changed from xpointer<Y> to raw pointer
            LinkNode<LinkRef<T, Y> > m_lnElement;

            typedef LinkNode<LinkRef<T, Y> > lnElementType;
            typedef T pElementType;
        };
    }
}