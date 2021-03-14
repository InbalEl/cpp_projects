#include <sys/timerfd.h> /* timer_fd */
#include <unistd.h>  // read
#include <stdint.h> // uint_64
#include <iostream>  // cout
#include "../reactor/monitors.hpp"
#include "../reactor/reactor.hpp"
#include "../function/function.hpp"
#include "timer_wheel.hpp"
#include "timers.hpp"

/**********************  using namespace ilrd::rd90  *************************/

using namespace ilrd::rd90;
#if(0)
void setTimer(int timerfd);
void HandleTimerEvent(int timerfd);
#endif

std::shared_ptr<TimerWheel<TimerFd>> timer_wheel;
void Pong(int i);
void Ping(int i)
{
    std::cout << "Ping " << i << std::endl;
    NanoSecs timeout(1000000000);
    timer_wheel->SetAlarm(timeout, Bind(&Pong));
}


void Pong(int i)
{
    std::cout << "Pong " << i << std::endl;
    NanoSecs timeout(1000000000);
    timer_wheel->SetAlarm(timeout, Bind(&Ping));
}

/*********************************  MAIN  ************************************/
int main()
{
    Reactor<Epoll> react(new Epoll(3));
    std::shared_ptr<TimerFd> timer(new TimerFd(react));
    timer_wheel.reset(new TimerWheel<TimerFd>(timer));

    NanoSecs timeout1(2000000000);

    /* NanoSecs timeout2(6000000000);
    NanoSecs timeout3(3000000000);
    NanoSecs timeout4(4000000000);
    NanoSecs timeout5(8000000000);
    NanoSecs timeout6(10000000000); */
    
    timer_wheel->SetAlarm(timeout1, Bind(&Ping));

    /* timer_wheel->SetAlarm(timeout2, Bind(&Ping));
    timer_wheel->SetAlarm(timeout3, Bind(&Ping));
    timer_wheel->SetAlarm(timeout4, Bind(&Ping));
    TimerWheel<TimerFd>::AlarmId alarm_id5 = timer_wheel->SetAlarm(timeout5, Bind(&Ping));
    timer_wheel->SetAlarm(timeout6, Bind(&Ping));
    timer_wheel->CancelAlarm(alarm_id5); */

    react.Run();


    #if(0)
    int timerfd = -1;
    itimerspec new_val = {0};
    uint64_t nof_exp_occ = 0;

    timerfd = timerfd_create(CLOCK_REALTIME, 0);

    /* set timer */
    new_val.it_interval.tv_sec = 2;
    new_val.it_interval.tv_nsec = 0;
    new_val.it_value.tv_nsec = 0;
    new_val.it_value.tv_sec = 2; /* reccuring */

    timerfd_settime(timerfd, 0, &new_val, NULL);

    while (1)
    {
        read(timerfd, &nof_exp_occ, sizeof(nof_exp_occ));
        std::cout << "number of expirations read = " << nof_exp_occ << std::endl;   
    }

    /* --------------------------- */

    std::cout << "Creating reactor" << std::endl;
    Reactor<Select> react(new Select);
    int timerfd = timerfd_create(CLOCK_REALTIME, 0);
    std::cout << "opening timerfd" << std::endl;
    react.AddReadHandler(timerfd, Function<void(int)>(HandleTimerEvent));
    std::cout << "setting timer" << std::endl;
    setTimer(timerfd);
    
    std::cout << "running reactor" << std::endl;
    react.Run();

    /* --------------------------- */
    
    std::cout << "Creating reactor" << std::endl;
    Reactor<Epoll> react(new Epoll(3));
    int timerfd = timerfd_create(CLOCK_REALTIME, 0);
    std::cout << "opening timerfd" << std::endl;
    react.AddReadHandler(timerfd, Function<void(int)>(HandleTimerEvent));
    std::cout << "setting timer" << std::endl;
    setTimer(timerfd);
    
    std::cout << "running reactor" << std::endl;
    react.Run();
    #endif

    return (0);
}

#if(0)
void setTimer(int timerfd)
{
    itimerspec new_val;
    new_val.it_interval.tv_sec = 2;
    new_val.it_interval.tv_nsec = 0;
    new_val.it_value.tv_nsec = 0;
    new_val.it_value.tv_sec = 5; /* reccuring */

    timerfd_settime(timerfd, 0, &new_val, NULL);
}

void HandleTimerEvent(int timerfd)
{
    uint64_t nof_exp_occ = 0;
    read(timerfd, &nof_exp_occ, sizeof(nof_exp_occ));
    std::cout << "number of expirations read = " << nof_exp_occ << std::endl;
}
#endif
/*****************************************************************************/

/*****************************************************************************/