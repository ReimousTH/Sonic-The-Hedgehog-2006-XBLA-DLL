#pragma once

#include <xtl.h>

#define nullptr 0

#define BranchTo(offset,rtype,...) \
	((rtype (__fastcall *)(...))offset)(__VA_ARGS__)

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned long long uint64_t;
typedef unsigned char uint8_t;

template<typename T>
struct be
{
    T value;

    be() : value(0)
    {
    }

    be(const T v)
    {
        set(v);
    }

    void set(const T v)
    {
        value = v;
    }

    T get() const
    {
        return value;
    }

    be& operator| (T value)
    {
        set(get() | value);
        return *this;
    }

    be& operator& (T value)
    {
        set(get() & value);
        return *this;
    }

    operator T() const
    {
        return get();
    }

    be& operator=(T v)
    {
        set(v);
        return *this;
    }
};

template<typename T>
class xpointer {
private:
    T* m_ptr;

public:
    // Constructors
    xpointer() : m_ptr(NULL) {}
    xpointer(T* ptr) : m_ptr(ptr) {}
    xpointer(const xpointer& other) : m_ptr(other.m_ptr) {}

    // Templated constructor for derived types
    template<typename U>
    xpointer(const xpointer<U>& other) : m_ptr(other.get()) {}

    // Assignment operators
    xpointer& operator=(T* ptr) {
        m_ptr = ptr;
        return *this;
    }

    xpointer& operator=(const xpointer& other) {
        m_ptr = other.m_ptr;
        return *this;
    }

    template<typename U>
    xpointer& operator=(const xpointer<U>& other) {
        m_ptr = other.get();
        return *this;
    }

    // Get the raw pointer
    T* get() const { return m_ptr; }

    // Implicit conversion to raw pointer
    operator T* () const { return m_ptr; }

    // Arrow operator
    T* operator->() const { return m_ptr; }

    // Dereference
   // T& operator*() const { return *m_ptr; }

    // Array access
//    T& operator[](size_t index) const { return m_ptr[index]; }

    // Boolean conversion
    operator bool() const { return m_ptr != NULL; }
    bool operator!() const { return m_ptr == NULL; }

    // Reset
    void reset(T* ptr = NULL) { m_ptr = ptr; }

    // Address-of operator (returns pointer to raw pointer)
    T** operator&() { return &m_ptr; }
    const T** operator&() const { return &m_ptr; }
};

#define MARATHON_XBLA_CLASS

#define MARATHON_ASSERT_OFFSETOF(type, field, offset) ;

#define MARATHON_ASSERT_SIZEOF(type, size) ;

#define MARATHON_ALIGNAS(aligment) __declspec(align(aligment))


#define MARATHON_CONCAT2(x, y) x##y
#define MARATHON_CONCAT(x, y) MARATHON_CONCAT2(x, y)

#define MARATHON_INSERT_PADDING(length) \
uint8_t MARATHON_CONCAT(pad, __LINE__)[length]

#define MARATHON_CALL_VIRTUAL_FUNCTION(base, returnType, func, ...) \
      ((returnType (*)(...))\
        ((this->m_pVftable.get()->*##func)).get())\
        (__VA_ARGS__)

#define MARATHON_DESTRUCTION_H(className) \
    virtual void* DestroyObject(unsigned int flag) { \
        this->~className(); \
        if ((flag & 1) != 0) { \
            ::operator delete(this); \
        } \
        return this; \
    }
