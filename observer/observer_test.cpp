/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <iostream>
#include "observer.hpp"
#include "book_publisher.hpp"

/*************************  namespace ilrd::rd90  ****************************/
using namespace ilrd::rd90;
/****************************   DECLARATIONS   *******************************/
/**********************************  MAIN  ***********************************/

int main()
{
    Publisher geekbooks;
    Geek geek1;
    Geek geek2;
    Geek geek3;

    geek1.subscribe(geekbooks);
    geek2.subscribe(geekbooks);
    geek3.subscribe(geekbooks);

    geekbooks.ReceiveBook();

    geek2.unsubscribe();

    geekbooks.ReceiveBook();

    return(0);
}

/*****************************************************************************/

/*****************************************************************************/
