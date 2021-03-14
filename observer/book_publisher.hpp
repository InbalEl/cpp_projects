/***********************   PREPROCESSOR DIRECTIVES   *************************/
#pragma once
#include "observer.hpp"

/******************************   ilrd::rd90   *******************************/
namespace ilrd {
namespace rd90 {
/*****************************  Class Publisher  *****************************/

class Publisher
{
public:
    void subscribe(CallBack* subscriber);
    void ReceiveBook();

private:
    void NewBookRelease();
    Dispatcher m_disp;
};

/*******************************  Class Reader  ******************************/

class Geek
{
public:
    explicit Geek();
    void subscribe(Publisher& publisher);
    void unsubscribe();
    void OnNotification(int i);

private:
    CallBack m_callback;
};

/******************************   ilrd::rd90   *******************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/