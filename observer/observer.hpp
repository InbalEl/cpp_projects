/***********************   PREPROCESSOR DIRECTIVES   *************************/
#include <set> // std::set 
#pragma once
#include "function.hpp" // Function

/******************************   ilrd::rd90   *******************************/
namespace ilrd {
namespace rd90 {
/***************************  Class Dispatcher  ******************************/
class CallBack;
class Dispatcher
{
public:
    void subscribe(CallBack* subscriber);
    void unsubscribe(CallBack* subscriber);
    void notify();

private:
    std::set<CallBack*> m_callbacks;
};

/*************************** Class CallBack ***********************************/

class CallBack
{
public:
    explicit CallBack(const Function<void(void)>& on_update_func);
    ~CallBack();
    void operator()();
    void setDispatcher(Dispatcher* disp);
    void unsubscribe();

private:
    Function<void(void)> m_on_update_func;
    Dispatcher* m_disp;
};

/******************************   ilrd::rd90   *******************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/