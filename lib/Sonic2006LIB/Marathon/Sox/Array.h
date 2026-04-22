#pragma once

#include <Marathon.inl>
#include <algorithm> 

namespace Sonicteam
{
    namespace SoX
    {
        // Dynamic array with manual memory management (VS2005 style)
        template <typename T>
        class ArrayOld
        {
        private:
            struct Header
            {
                be<uint32_t> m_count;
                T m_elements[1];  // Flexible array member alternative
            };

            T* m_elements;

            // Disable copy for now (VS2005 doesn't have = delete)
        private:
            ArrayOld(const ArrayOld&);
            ArrayOld& operator=(const ArrayOld&);

        public:
            // Iterator types
            typedef T* iterator;
            typedef const T* const_iterator;
            typedef std::reverse_iterator<iterator> reverse_iterator;
            typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

            ArrayOld() : m_elements(NULL)
            {
            }

            explicit ArrayOld(uint32_t count)
            {
                if (count == 0) {
                    m_elements.reset(NULL);
                    return;
                }

                // Calculate total size including header
                size_t headerSize = sizeof(Header) - sizeof(T); // Subtract the m_elements[1]
                size_t totalSize = headerSize + sizeof(T) * count;

                // Allocate raw memory
                void* memory = g_userHeap.Alloc(totalSize);
                if (!memory) {
                    // Handle allocation failure (VS2005 style)
                    throw std::bad_alloc();
                }

                Header* header = static_cast<Header*>(memory);
                header->m_count = count;

                m_elements.reset(reinterpret_cast<T*>(header + 1));

                // Default construct elements
                for (uint32_t i = 0; i < count; ++i) {
                    new (&m_elements[i]) T();  // Placement new
                }
            }

            // Manual cleanup method (VS2005 style)
            void Clear()
            {
                if (!m_elements) return;

                Header* header = reinterpret_cast<Header*>(
                    reinterpret_cast<char*>(m_elements) - (sizeof(Header) - sizeof(T)));
                uint32_t count = header->m_count;

                // Destroy elements in reverse order
                for (int32_t i = static_cast<int32_t>(count) - 1; i >= 0; --i) {
                    m_elements[i].~T();
                }

                free(header);
            }

            // Element access
            T* data()
            {
                return m_elements.get();
            }

            const T* data() const
            {
                return m_elements.get();
            }

            T& operator[](size_t index)
            {
                return m_elements[index];
            }

            const T& operator[](size_t index) const
            {
                return m_elements[index];
            }

            uint32_t size() const
            {
                if (!m_elements) return 0;

                Header* header = reinterpret_cast<Header*>(
                    reinterpret_cast<char*>(m_elements.get()) - (sizeof(Header) - sizeof(T)));
                return header->m_count;
            }

            bool empty() const
            {
                return size() == 0;
            }

            // Iterators
            iterator begin()
            {
                return m_elements;
            }

            iterator end()
            {
                return m_elements + size();
            }

            const_iterator begin() const
            {
                return m_elements;
            }

            const_iterator end() const
            {
                return m_elements + size();
            }

            const_iterator cbegin() const
            {
                return m_elements;
            }

            const_iterator cend() const
            {
                return m_elements + size();
            }

            // Reverse iterators
            reverse_iterator rbegin()
            {
                return reverse_iterator(end());
            }

            reverse_iterator rend()
            {
                return reverse_iterator(begin());
            }

            const_reverse_iterator rbegin() const
            {
                return const_reverse_iterator(end());
            }

            const_reverse_iterator rend() const
            {
                return const_reverse_iterator(begin());
            }

            const_reverse_iterator crbegin() const
            {
                return const_reverse_iterator(end());
            }

            const_reverse_iterator crend() const
            {
                return const_reverse_iterator(begin());
            }

            ~ArrayOld()
            {
                Clear();
            }
        };

        // Fixed-size array (compile-time size)
        template <typename T, uint32_t count>
        class Array
        {
        private:
            T m_container[count];

            // Disable copy
            Array(const Array&);
            Array& operator=(const Array&);

        public:
            Array()
            {
                // Default construct all elements
                for (uint32_t i = 0; i < count; ++i) {
                    new (&m_container[i]) T();
                }
            }

            ~Array()
            {
                // Destroy all elements in reverse order
                for (int32_t i = static_cast<int32_t>(count) - 1; i >= 0; --i) {
                    m_container[i].~T();
                }
            }

            T& operator[](uint32_t index)
            {
                return m_container[index];
            }

            const T& operator[](uint32_t index) const
            {
                return m_container[index];
            }

            uint32_t size() const
            {
                return count;
            }

            T* data()
            {
                return m_container;
            }

            const T* data() const
            {
                return m_container;
            }
        };

        // Simple array pointer wrapper
        template <typename T>
        struct ArrayPtr {
            xpointer<T> pitems;
            uint32_t count;

            ArrayPtr() : pitems(NULL), count(0)
            {
            }

            explicit ArrayPtr(uint32_t size) :
                pitems(g_userHeap.Alloc(size * sizeof(T))),
                count(size)
            {
                if (pitems) {
                    // Default construct elements
                    for (uint32_t i = 0; i < count; ++i) {
                        new (&pitems[i]) T();
                    }
                }
            }

            ArrayPtr(T* ptr, uint32_t size)
                : pitems(ptr), count(size)
            {
            }

            ~ArrayPtr()
            {
                Clear();
            }

            void Clear()
            {
                if (!pitems) return;

                // Destroy elements
                for (int32_t i = static_cast<int32_t>(count) - 1; i >= 0; --i) {
                    pitems[i].~T();
                }

                g_userHeap.Free(pitems.get());
                pitems.reset(NULL);
                count = 0;
            }

            T& operator[](size_t index)
            {
                return pitems[index];
            }

            const T& operator[](size_t index) const
            {
                return pitems[index];
            }

        private:
            // Disable copy (VS2005 style)
            ArrayPtr(const ArrayPtr&);
            ArrayPtr& operator=(const ArrayPtr&);
        };

        // AIMArray for VS2005
        template <typename T>
        struct AIMArray
        {
            xpointer<T> pitems;
            uint32_t LastItemID;
            uint32_t EndItemID;

            explicit AIMArray(uint32_t size)
                : pitems(g_userHeap.Alloc(size * sizeof(T))),
                LastItemID(size > 0 ? size - 1 : 0xFFFFFFFF),
                EndItemID(size > 0 ? size - 1 : 0xFFFFFFFF)
            {
                if (pitems) {
                    // Default construct elements
                    for (uint32_t i = 0; i <= EndItemID; ++i) {
                        new (&pitems[i]) T();
                    }
                }
            }

            AIMArray(T* existingItems, uint32_t size)
                : pitems(existingItems),
                LastItemID(size > 0 ? size - 1 : 0xFFFFFFFF),
                EndItemID(size > 0 ? size - 1 : 0xFFFFFFFF)
            {
            }

            ~AIMArray()
            {
                if (pitems) {
                    // Destroy elements
                    for (int32_t i = static_cast<int32_t>(EndItemID); i >= 0; --i) {
                        pitems[i].~T();
                    }

                    g_userHeap.Free(pitems.get());
                    pitems.reset(NULL);
                }
            }

            // Safe element access
            T& at(size_t index) {
                if (index > EndItemID) {
                    // VS2005 doesn't have std::out_of_range in all cases
                    static T dummy;
                    return dummy;  // Better: throw or assert
                }
                return pitems[index];
            }

            const T& at(size_t index) const {
                if (index > EndItemID) {
                    static T dummy;
                    return dummy;
                }
                return pitems[index];
            }

            T& operator[](size_t index) {
                return at(index);
            }

            const T& operator[](size_t index) const {
                return at(index);
            }

        private:
            // Disable copy
            AIMArray(const AIMArray&);
            AIMArray& operator=(const AIMArray&);
        };

    }
}