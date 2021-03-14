/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <iostream>
#include <unistd.h>
#include "function.hpp"
#include "threadpool.hpp"

/**************************  using ilrd::ol90  *******************************/
using namespace ilrd::rd90;

/*******************************   MAIN   ************************************/
#if(1)
int task1_function(int arg)
{
    static int num = 0;
    
    num += arg;

    for (size_t i = 0; i < 5; i++)
    {
        sleep(1);
        std::cout << "Task 1 : The number is - " << num++ << std::endl;
    }

    return(0);
}

/*---------------------------------------------------------------------------*/

int task2_function(int arg)
{
    static int num = 0;

    num += arg;

    for (size_t i = 0; i < 5; i++)
    {
        sleep(1);
        std::cout << "Task 2 : The number is - " << num << std::endl;
        num *= 10;
    }

    return(0);
}

/*---------------------------------------------------------------------------*/

int main()
{
    Threadpool tp(4);

    std::cout << "Reducing concurrency level to 0" << std::endl;
    tp.fineTune(-4);
    
    std::cout << "Entering 2 tasks" << std::endl;

    Threadpool::Future f1 = tp.Async(Bind(&task1_function, 1),
                                     Threadpool::Priority::Low());
    Threadpool::Future f2 = tp.Async(Bind(&task2_function, 2),
                                     Threadpool::Priority::Mid());

    std::cout<< "Tasks should not be performed..." << std::endl;
    sleep(5);

    std::cout<< "Raising concurrency level to 2" << std::endl;
    sleep(1);
    tp.fineTune(2);

    std::cout << "status of f1 " <<  f1.GetTaskResult() << std::endl;
    std::cout << "status of f2 " <<  f2.GetTaskResult() << std::endl;

    return (0);
}
#endif


/*****************************************************************************/
#if(0)
int action(int arg)
{

    std::cout << arg << std::endl;
    return 0;
}

int action1(int arg)
{

    std::cout << "suppose to print only after sleep:" << arg << std::endl;
    return 0;
}


int main()
{
    Threadpool t1(16);
    t1.fineTune(-8);
    Threadpool::Future f1[100];
    

    //f1[1].GetStatus();

    for(int i = 0; i < 100; ++i)
    {
        f1[i] = t1.Async(Bind(action,i), Threadpool::Priority::Low());
    }

    f1[20].GetTaskResult();

/*     std::cout << "task 20 status after wait: " << f1[20].GetStatus() << std::endl;
    std::cout << "task 5 status after exec: " << f1[5].GetStatus() << std::endl; */

    t1.fineTune(-8);
    sleep(1);
    std::cout << "after tune to zero " << std::endl;

    Threadpool::Future f2[2];
    f2[0] = t1.Async(Bind(action1,5), Threadpool::Priority::Low());

    std::cout << "sleep for 4 sec and then tune up " << std::endl;
    sleep(4);
    t1.fineTune(8);

    std::cout << "--------------wait test---------------- "<< std::endl;

    //Threadpool t2(16);
    for(int j = 0; j < 100; ++j)
    {
        (t1.Async(Bind(action,j), Threadpool::Priority::Low())).GetTaskResult();
    }

    return 0;
}
#endif