/***********************   PREPROCESSOR DIRECTIVES   *************************/
#include <list> // std::vector 
#include "observer.hpp" // Dispatcher, Callback
#include "function.hpp" // Function

/******************************   ilrd::rd90   *******************************/
namespace ilrd {
namespace rd90 {
/****************************  Class Dispatcher  *****************************/

void Dispatcher::subscribe(CallBack* subscriber)
{
    m_callbacks.insert(subscriber);
    subscriber->setDispatcher(this);
}

/*----------------------------------------------------------------------------*/

void Dispatcher::unsubscribe(CallBack* subscriber)
{
    m_callbacks.erase(subscriber);
}

/*----------------------------------------------------------------------------*/

void Dispatcher::notify()
{
    std::set<CallBack*>::iterator curr = m_callbacks.begin();
    CallBack* cb;

    while (curr != m_callbacks.end())
    {
        cb = const_cast<CallBack*>(*curr);
        (*cb)();
        curr++;
    }
}

/*****************************  Class CallBack  ******************************/

CallBack::CallBack(const Function<void(void)>& on_update_func)
 : m_on_update_func(on_update_func) {}

/*----------------------------------------------------------------------------*/

void CallBack::setDispatcher(Dispatcher* disp)
{
    m_disp = disp;
}

/*----------------------------------------------------------------------------*/

CallBack::~CallBack()
{
    m_disp->unsubscribe(this);
}

/*----------------------------------------------------------------------------*/

void CallBack::operator()()
{
    m_on_update_func();
}

/*----------------------------------------------------------------------------*/

void CallBack::unsubscribe()
{
    m_disp->unsubscribe(this);
}

/******************************   ilrd::rd90   *******************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/