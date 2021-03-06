/**********************   PREPROCESSOR DIRECTIVES   **************************/
#pragma once
#include <cstddef>

/*---------------------------------------------------------------------------*/

namespace ilrd
{
namespace rd90
{

/************************   Classes declarations    **************************/

template <typename T>
class SharedPtr
{
public:
    explicit SharedPtr(T *ptr = NULL);
    ~SharedPtr();
    SharedPtr(const SharedPtr& other);

    SharedPtr& operator=(const SharedPtr& other);

    template <typename R>

    #ifdef assignment_overload
    explicit
    #endif
    SharedPtr(const SharedPtr<R>& other);

    #ifdef assignment_overload

    template <typename R>
    SharedPtr<T>& operator=(const SharedPtr<R>& other);

    #endif

    bool operator!();
    size_t UseCount() const;
    T* operator->() const;
    T& operator*() const;
    
private:
    T* m_ptr; 
    size_t *m_counter;
    void Detach();
    void Attach(const SharedPtr& other);

    template<typename>
    friend class SharedPtr;

};

/*---------------------------------------------------------------------------*/

template <typename T>
SharedPtr<T>::SharedPtr(T *ptr) : m_ptr(ptr), m_counter(new size_t(1))
{
}

/*---------------------------------------------------------------------------*/
#ifdef assignment_overload

template <typename T>
template <typename R>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<R>& other)
{
    return (*this = SharedPtr(other));
}
#endif

/*---------------------------------------------------------------------------*/

template <typename T>
SharedPtr<T>::~SharedPtr()
{
    Detach();
}

/*---------------------------------------------------------------------------*/

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) :
    m_ptr(other.m_ptr), m_counter(other.m_counter)
{
    ++(*m_counter);
}

/*---------------------------------------------------------------------------*/

template <typename T>
void SharedPtr<T>::Detach()
{
    if (--(*m_counter) == 0)
    {
        delete m_counter;
        delete m_ptr;
    }
}

/*---------------------------------------------------------------------------*/

template <typename T>
void SharedPtr<T>::Attach(const SharedPtr& other)
{
    m_counter = other.m_counter;
    m_ptr = other.m_ptr;
    ++(*m_counter);
}

/*---------------------------------------------------------------------------*/

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr& other)
{
    if (this != &other)
    {
        Detach();
        Attach(other);
    }
}

/*---------------------------------------------------------------------------*/

template <typename T>
bool SharedPtr<T>::operator!()
{
    return (!m_ptr);
}

/*---------------------------------------------------------------------------*/

template <typename T>
size_t SharedPtr<T>::UseCount() const
{
    return (*m_counter);
}

/*---------------------------------------------------------------------------*/

template <typename T>
T* SharedPtr<T>::operator->() const
{
    return (m_ptr);
}

/*---------------------------------------------------------------------------*/

template <typename T>
T& SharedPtr<T>::operator*() const
{
    return (*m_ptr);
}

/*****************************************************************************/

} //namespace rd90
} //namespace ilrd
