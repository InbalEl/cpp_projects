/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <unistd.h> /* sleep */
#include "singleton.hpp"

/***************************   Class Widget   ********************************/
namespace ilrd
{
namespace rd90
{

const static int nof_threads = 2;

class Widget
{
public:
    ~Widget();
    void fooMethod();
    int getNofInst() const;
    
    template <typename T>
    friend class Singleton;

private:
    explicit Widget();
    static int instCount;
};


Widget::Widget()
{
    ++instCount;
}

int Widget::getNofInst() const
{
    return (instCount);
}


int Widget::instCount = 0;

} // namespace ilrd
} // namespace rd90

using namespace ilrd::rd90;

void *threadFunc(void *thread_data);

/*******************************   MAIN   ************************************/

int main()
{
    pthread_t threads[nof_threads];
 
    Singleton<Widget> wi1;
    Singleton<Widget> wi2;
    Singleton<Widget> wi3;


    for (int i = 0 ; i < nof_threads ; ++i)
    {
        int res = pthread_create(&threads[i], NULL, threadFunc, NULL);
 
        if (res != 0)
        {
            std::cout << "Error in thread creation: " << res << std::endl;
        }
    }

    sleep(2);
    printf("in main number of instances of widget: %d\n", wi3->getNofInst());


    return (0);
}

void *threadFunc(void *thread_data)
{
    (void)thread_data;
    Singleton<Widget> wid1;
    Singleton<Widget> wid2;
    Singleton<Widget> wid3;

    sleep(3);

    printf("in thread number of instances of widget: %d\n", wid3->getNofInst());

    return (NULL);
}

/*****************************************************************************/
