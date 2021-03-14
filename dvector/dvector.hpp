

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <cstddef> // size_t
#include <assert.h> // assert
#pragma once

namespace ilrd {
namespace rd90 {
/****************************   Class DVector   ******************************/

const static size_t g_dvec_growth_factor = 2;
const static size_t g_dvec_shrink_factor = 4;

template <typename T>
class DVector
{
public:
    explicit DVector(); //to allow support for user class not having def. ctor
    explicit DVector(size_t init_size = 0, const T& t = T()); // throws
    ~DVector();

    void PushBack(const T& data);
    void PopBack();
    size_t Size() const;
    size_t Capacity() const;
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    void Resize(size_t new_size, T t = T());
    void Reserve(size_t new_cap);

/*------------------------  Nested Iterator Class  --------------------------*/

    class Iterator
    {
    public:
        typedef std::input_iterator_tag iterator_category;
        typedef  T value_type;
        typedef size_t difference_type;
        typedef T* pointer;
        typedef T& reference;

        explicit Iterator(T* data = NULL);
        bool operator!=(const Iterator& other);
        T& operator*();
        Iterator& operator++(); // prefix

    private:
        T* m_elem_ptr;
    };

/*---------------------------------------------------------------------------*/

    Iterator Begin();
    Iterator End();

private:
    DVector(const DVector& other);
    DVector& operator=(const DVector& other);
    static T* AllocateArray(size_t size);
    static void DestroyArr(T* t_arr, size_t from_i, size_t to_i);
    static void InitArr(T* t_arr, size_t from_i, size_t to_i, T& t);
    void Realloc(size_t new_cap);
    void CleanUp();

    size_t m_size;
    size_t m_curr_cap;
    T *m_data;    // array of T members
};

/*---------------------------------------------------------------------------*/

template <typename T>
DVector<T>::DVector() :
    m_size(0), m_curr_cap(m_size + 1), m_data(AllocateArray(m_curr_cap)) {}

/*---------------------------------------------------------------------------*/

template <typename T>
DVector<T>::DVector(size_t init_size, const T& t) :
    m_size(init_size), m_curr_cap(m_size + 1) ,
    m_data(AllocateArray(m_curr_cap))
    {
        InitArr(m_data, 0, init_size, const_cast<T&>(t));
    }

/*---------------------------------------------------------------------------*/

template <typename T>
DVector<T>::~DVector()
{
    CleanUp();
}

/*---------------------------------------------------------------------------*/

template <typename T>
void DVector<T>::CleanUp()
{
    DestroyArr(m_data, 0, m_size);
    ::operator delete(m_data);
}

/*---------------------------------------------------------------------------*/

template <typename T>
void DVector<T>::PushBack(const T& data)
{
    if (!(m_size < m_curr_cap))
    {
        Reserve(m_curr_cap * g_dvec_growth_factor);
    }

    new (m_data + m_size++) T(data); // calling cctor
}

/*---------------------------------------------------------------------------*/

template <typename T>
void DVector<T>::PopBack()
{
    m_data[--m_size].~T();

    if ((m_curr_cap / m_size) >= g_dvec_shrink_factor)
    {
        Realloc(m_curr_cap / 2);
    }
}

/*---------------------------------------------------------------------------*/

template <typename T>
size_t DVector<T>::Size() const
{
    return (m_size);
}

/*---------------------------------------------------------------------------*/

template <typename T>
size_t DVector<T>::Capacity() const
{
    return(m_curr_cap);
}

/*---------------------------------------------------------------------------*/

template <typename T>
T& DVector<T>::operator[](size_t index)
{
    return(const_cast<T&>(const_cast<const DVector&>(*this)[index]));
}

/*---------------------------------------------------------------------------*/

template <typename T>
const T& DVector<T>::operator[](size_t index) const
{
    assert(index < m_size);
    return (m_data[index]); // const
}

/*---------------------------------------------------------------------------*/

template <typename T>
void DVector<T>::Resize(size_t new_size, T t)
{
    Reserve(new_size);

    while (new_size > Size())
    {
        PushBack(t);
    }
    
    while (new_size < Size())
    {
        PopBack();
    }
}

/*---------------------------------------------------------------------------*/

template <typename T>
void DVector<T>::Reserve(size_t new_cap)
{
    if (new_cap > m_curr_cap)
    {
        Realloc(new_cap);
    }
}

/*---------------------------------------------------------------------------*/

template <typename T>
void DVector<T>::Realloc(size_t new_cap)
{
    T *new_data = AllocateArray(m_curr_cap = new_cap);
    
    for (size_t i = 0; i < m_size; ++i)
    {
        InitArr(new_data, i, i + 1, m_data[i]);
    }

    CleanUp();
    m_data = new_data;
}

/*---------------------------------------------------------------------------*/

template <typename T>
T* DVector<T>::AllocateArray(size_t size)
{
    return (static_cast<T *>(::operator new (sizeof(T) * size)));
}

/*---------------------------------------------------------------------------*/

template <typename T>
void DVector<T>::InitArr(T* t_arr, size_t from_i, size_t to_i, T& t)
{   // placement new loop
    for (size_t i = from_i; i < to_i; ++i)
    {
        ::new (reinterpret_cast<void *>(t_arr + i)) T(t);
    }
}

/*---------------------------------------------------------------------------*/

template <typename T>
void DVector<T>::DestroyArr(T* t_arr, size_t from_i, size_t to_i)
{
    for (size_t i = from_i; i < to_i; ++i) // i <= to_i ??? 
    {
        t_arr[i].~T();
    }
}

/*---------------------------------------------------------------------------*/

template <typename T>
typename DVector<T>::Iterator DVector<T>::Begin()
{
    return (DVector<T>::Iterator(&m_data[0]));
}

/*---------------------------------------------------------------------------*/

template <typename T>
typename DVector<T>::Iterator DVector<T>::End()
{
    return (DVector<T>::Iterator(&m_data[m_size]));
}

/********************  Class Iterator Implementation  ************************/

template <typename T>
DVector<T>::Iterator::Iterator(T* data) : m_elem_ptr(data) {}

template <typename T>
T& DVector<T>::Iterator::operator*()
{
    return (*m_elem_ptr);
}

/*---------------------------------------------------------------------------*/

template <typename T>
bool DVector<T>::Iterator::operator!=(const Iterator& other)
{
    return (m_elem_ptr != other.m_elem_ptr);
}

/*---------------------------------------------------------------------------*/

template <typename T>
typename DVector<T>::Iterator& DVector<T>::Iterator::operator++()
{
    ++m_elem_ptr;
    return (*this);
}

/*---------------------------------------------------------------------------*/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/

