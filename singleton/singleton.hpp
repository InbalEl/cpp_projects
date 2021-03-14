/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <cstdlib>
#include <new>

/*****************************************************************************/

namespace ilrd
{
namespace rd90
{

/**************************   Class Uncopyable   *****************************/

class Uncopyable
{
protected:
    Uncopyable();
    ~Uncopyable();

private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

/**************************   Class Singleton   ******************************/

template <typename T>
class Singleton : private Uncopyable //copying disabled
{
public:
    explicit Singleton();
    T* operator->() const;

private:
    static T* m_pInstance;
    static void Cleanup();
};

// this is a weak symbol - prevents several copies as we are in the H file
template <typename T>
T* Singleton<T>::m_pInstance = NULL;

/**********************  Class Singleton Functions  **************************/

template <typename T>
Singleton<T>::Singleton()
{
    if (m_pInstance == NULL)
    {
        static bool initStarted = false;

        // for debugging - sleep(1) so that 2 threads will get here at once
        if (!__atomic_test_and_set(&initStarted, __ATOMIC_SEQ_CST))
        {
            __atomic_store_n(&m_pInstance, new T, __ATOMIC_SEQ_CST);
            // if new throws, we're all in trouble anyway
            atexit(Cleanup);
        }
        
        else
        {
            while (__atomic_load_n(&m_pInstance, __ATOMIC_SEQ_CST) == 0) {} 
        }
    }
}

/*---------------------------------------------------------------------------*/

template <typename T>
T* Singleton<T>::operator->() const
{
    return m_pInstance;
}

/*---------------------------------------------------------------------------*/

template <typename T>
void Singleton<T>::Cleanup()
{
    delete m_pInstance;
    m_pInstance = (T*)0xDEADBEEF;
}

/*---------------------------------------------------------------------------*/
} // namespace ilrd
} // namespace rd90

/*****************************************************************************/