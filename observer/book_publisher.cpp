/***********************   PREPROCESSOR DIRECTIVES   *************************/
#include <iostream>
#include "observer.hpp"
#include "function.hpp"
#include "book_publisher.hpp"

/******************************   ilrd::rd90   *******************************/
namespace ilrd {
namespace rd90 {
/*****************************  Class Publisher  *****************************/

void Publisher::subscribe(CallBack* subscriber)
{
    m_disp.subscribe(subscriber);
}

/*----------------------------------------------------------------------------*/

void Publisher::NewBookRelease()
{
    m_disp.notify();
}

/*----------------------------------------------------------------------------*/

void Publisher::ReceiveBook()
{
    std::cout << "GeekBooks received a new book!" << std::endl;
    m_disp.notify();
}

/*******************************  Class Reader  ******************************/

Geek::Geek()
 : m_callback(Bind(&Geek::OnNotification, this, 0))
{}

/*----------------------------------------------------------------------------*/

void Geek::subscribe(Publisher& publisher)
{
    publisher.subscribe(&m_callback);
}

/*----------------------------------------------------------------------------*/

void Geek::OnNotification(int i)
{
    std::cout << "Gotta run to the store! " << i << std::endl;
}

/*----------------------------------------------------------------------------*/

void Geek::unsubscribe()
{
    m_callback.unsubscribe();
}

/******************************   ilrd::rd90   *******************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/