/***********************   PREPROCESSOR DIRECTIVES   *************************/
#include <memory> /* shared_ptr */
#include <chrono> /* duration, time_point */
#include <unistd.h>  // read
#include <sys/timerfd.h> /* timerfd */
#include <stdint.h> // uint_64
#pragma once
#include "../function/function.hpp" /* Function */
#include "../reactor/reactor.hpp" /* Reactor */

/******************************   ilrd::rd90   *******************************/
namespace ilrd {
namespace rd90 {
/*---------------------------------------------------------------------------*/

using SysClock = std::chrono::system_clock;
using TimePoint = std::chrono::time_point<SysClock>;
using Duration = std::chrono::duration<double>;
using NanoSecs = std::chrono::nanoseconds;

class ITimer
{
public:
    virtual ~ITimer() = default;

    virtual void RegisterOnEvent(const Function<void(int)>& func) = 0;
    virtual void Arm(const Duration& delta) = 0;
};

/*****************************************************************************/

class TimerFd : public ITimer
{
public:
    explicit TimerFd(Reactor<Epoll>& react); /* non-const since we want to manipulate it */
    ~TimerFd();

    TimerFd(const TimerFd&) = delete; /* UNCOPYABLE */
    TimerFd& operator=(const TimerFd&) = delete;

    virtual void Arm(const Duration& delta);
    virtual void RegisterOnEvent(const Function<void(int)>& func);

private:
    void OnTimerHandler(int i);

    int m_timerfd;
    Reactor<Epoll>& m_react;
    Function<void(int)> m_timer_func;
};

/*****************************************************************************/

TimerFd::TimerFd(Reactor<Epoll>& react)
 : m_timerfd(timerfd_create(CLOCK_REALTIME, 0)), m_react(react)
{
    react.AddReadHandler(m_timerfd, Bind(&TimerFd::OnTimerHandler, this));
}

/*---------------------------------------------------------------------------*/

TimerFd::~TimerFd()
{
    m_react.RemoveReadHandler(m_timerfd);
    close(m_timerfd);
}

/*---------------------------------------------------------------------------*/

void TimerFd::Arm(const Duration& delta)
{
    static uint32_t nanosecs_in_sec = 1000000000;
    itimerspec timer_val;
    uint64_t nanoSec = std::chrono::duration_cast<NanoSecs>(delta).count();

    timer_val.it_value.tv_sec = nanoSec / nanosecs_in_sec; 
    timer_val.it_value.tv_nsec = nanoSec % nanosecs_in_sec;
    timer_val.it_interval.tv_sec = 0;
    timer_val.it_interval.tv_nsec = 0;

    timerfd_settime(m_timerfd, 0, &timer_val, NULL);
}

/*---------------------------------------------------------------------------*/

void TimerFd::RegisterOnEvent(const Function<void(int)>& func)
{
    m_timer_func = func;
}

/*---------------------------------------------------------------------------*/

void TimerFd::OnTimerHandler(int i)
{
    (void)i;
    uint64_t nof_exp_occ = 0;
    read(m_timerfd, &nof_exp_occ, sizeof(nof_exp_occ));
    m_timer_func(0);
}

/*------------------------------  ilrd::rd90  -------------------------------*/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/