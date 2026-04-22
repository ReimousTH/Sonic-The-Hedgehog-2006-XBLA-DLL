#pragma once

#include <Marathon.inl>
#include <SoX/RefCountObject.h>

namespace Sonicteam
{
    namespace SoX
    {
        template <typename T>
        class RefSharedPointer
        {
        private:
            T* m_ptr;

        public:
            RefSharedPointer() : m_ptr(0)
            {
            }

            explicit RefSharedPointer(T* value) : m_ptr(value)
            {
                if (m_ptr)
                    m_ptr->AddReference();
            }

            RefSharedPointer(const RefSharedPointer& other) : m_ptr(other.m_ptr)
            {
                if (m_ptr)
                    m_ptr->AddReference();
            }

            ~RefSharedPointer()
            {
                if (m_ptr)
                    m_ptr->Release();
            }

            RefSharedPointer& operator=(const RefSharedPointer& other)
            {
                if (this && this != &other && (&other))
                {
                    reset();

                    m_ptr = other.m_ptr;

                    if (m_ptr)
                        m_ptr->AddReference();
                }

                return *this;
            }

            // Removed move constructor (C++11 feature not in VS2005)
            // Use std::swap idiom instead for VS2005

            void swap(RefSharedPointer& other)
            {
                T* temp = m_ptr;
                m_ptr = other.m_ptr;
                other.m_ptr = temp;
            }

            void reset()
            {
                if (m_ptr)
                    m_ptr->Release();

                m_ptr = 0;
            }

            T* get() const
            {
                return m_ptr;
            }

            T* operator->() const
            {
                return m_ptr;
            }

            T& operator*() const
            {
                return *m_ptr;
            }

            // operator bool() for VS2005 (not explicit in C++03)
            typedef T* RefSharedPointer::* unspecified_bool_type;

            operator unspecified_bool_type() const
            {
                return m_ptr != 0 ? &RefSharedPointer::m_ptr : 0;
            }

            // Safe bool idiom helper
            bool operator!() const
            {
                return m_ptr == 0;
            }

            bool operator==(const RefSharedPointer& other) const
            {
                return m_ptr == other.m_ptr;
            }

            bool operator!=(const RefSharedPointer& other) const
            {
                return m_ptr != other.m_ptr;
            }

            // Comparison with raw pointer
            bool operator==(const T* ptr) const
            {
                return m_ptr == ptr;
            }

            bool operator!=(const T* ptr) const
            {
                return m_ptr != ptr;
            }

            // Comparison with 0
            bool operator==(int null) const
            {
                return null == 0 && m_ptr == 0;
            }

            bool operator!=(int null) const
            {
                return !(*this == null);
            }
        };

        // Global swap function
        template<typename T>
        void swap(RefSharedPointer<T>& a, RefSharedPointer<T>& b)
        {
            a.swap(b);
        }
    }
}