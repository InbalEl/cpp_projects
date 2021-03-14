/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <functional>
#include <iostream>
#include "function.hpp"

/*****************************************************************************/

class Widget
{
public:
    void Action1(int data) // test func
    {
        printf("Widget Action 1: %d\n", data);
    }
    
    void Action2(int data) // test func
    {
        printf("Widget Action 2: %d\n", data);
    }
};

class Gadget
{
public:
    void Action(int data) // test func
    {
        printf("Gadget Action: %d\n", data);
    }

    int Action2(int data) // test func
    {
        printf("Gadget Action2: %d\n", data);
        return (data);
    }
};

/*****************************************************************************/
using namespace ilrd::rd90;

void forEach1(int* vector, size_t size, Function<void(int)> action) // action will be by ref
{
    for (size_t i = 0; i < size; ++i)
    {
        action(vector[i]);
    }
}

/*---------------------------------------------------------------------------*/

void forEach2(int* vector, size_t size, const Function<void(int)>& action)
{
    for (size_t i = 0; i < size; ++i)
    {
        action(vector[i]);
    }
}

/*---------------------------------------------------------------------------*/

void foo(int data)
{
    printf("Free func action: %d\n", data);
}

/*---------------------------------------------------------------------------*/

char fifi(int num)
{
    return (num + 48);
}

int pepe(void)
{
    printf("Pepe int(void0): %d\n", 77);
    return (77);
}

/*****************************************************************************/

int main()
{
    int arr[10] = {1, 2, 3 ,4 ,5 ,6 ,7, 8 ,9, 10};    

    Widget w1;
    Gadget g1;

    Function<void(int)> f1;

    if(1)
    {
        f1 = Function<void(int)>(&Widget::Action1, &w1);
        f1(123);
    }
    else
    {
        f1 = Function<void(int)>(&Gadget::Action, &g1);
        f1(456);
    }

    /*------------ Testing forEach with member functions ------------*/

    forEach1(arr, 10, Function<void(int)>(&Widget::Action1, &w1));
    forEach1(arr, 10, Function<void(int)>(&Widget::Action2, &w1));
    forEach1(arr, 10, Function<void(int)>(&Gadget::Action, &g1));


    forEach2(arr, 10, Function<void(int)>(&Widget::Action1, &w1));
    forEach2(arr, 10, Function<void(int)>(&Widget::Action2, &w1));
    forEach2(arr, 10, Function<void(int)>(&Gadget::Action, &g1));

    /*------------ Testing forEach with free functions ------------*/

    forEach1(arr, 10, Function<void(int)>(&foo));
    forEach2(arr, 10, Function<void(int)>(&foo));

    /*------------ Testing template function ------------*/

    Function<char(int)> f2(&fifi); // char*(int) is a signature
    char ch = f2(3);

    std::cout << "ch = " << ch << std::endl; 

    Function<int(int)> f5;
    f5 = Bind(&Gadget::Action2, &g1);
    f5(42);

    std::cout << "*** R(void) ***\n";
    Function<void(void)> f3(foo, 99);
    f3();

    // Function<int(void)> f3(&pepe); // this would require special treatment since void isn't a type
    //int num = f3();

    // std::cout << "num = " << num << std::endl;

    #if (I_AM_AWESOME)
    #endif

    return (0);
}

/*****************************************************************************/