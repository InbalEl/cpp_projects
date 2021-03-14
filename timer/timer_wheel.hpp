/***********************   PREPROCESSOR DIRECTIVES   *************************/
#include <memory> /* shared_ptr */
#include <queue> /* priority_queue */
#include <chrono> /* duration, time_point */
#include <unordered_map> /* unordered_map */
#pragma once
#include "../function/function.hpp" /* Function */
#include "timers.hpp" /* TimerFd */

/******************************   ilrd::rd90   *******************************/
namespace ilrd {
namespace rd90 {
/******************************   Class Timer   ******************************/

template <typename T>
class TimerWheel
{
public:
    using AlarmId = uint32_t;

    explicit TimerWheel(std::shared_ptr<T>& timer);
    
    TimerWheel(const TimerWheel& other) = delete; /* UNCOPYABLE */
    TimerWheel& operator=(const TimerWheel& other) = delete;

    AlarmId SetAlarm(const NanoSecs& timeout,
                     const Function<void(int)>& func);
    void CancelAlarm(AlarmId uid);

private:
    class Task
    {
    public:
        Task(const Function<void(int)>& func,
             const TimePoint& exp_time, AlarmId uid);
        struct CompareTasks
        {
            bool operator()(const std::shared_ptr<Task>& task1,
                            const std::shared_ptr<Task>& task2);
            // compare expiration times of tasks
        };

        void PerformTask();
        void CancelTask();
        bool IsTaskCancelled();
        AlarmId GetAlarmId();
        TimePoint GetExpTime();
        Function<void(int)> GetTaskFunc();

    private:
        AlarmId m_alarm_id;
        TimePoint m_exp_time;
        Function<void(int)> m_alarm_func;
        bool m_is_task_cancelled;
    };

    uint32_t m_uid_counter;
    using TaskPtr = std::shared_ptr<Task>;
    void OnTimerEvent(int i);
    void RemoveTask(const TaskPtr& task);
    Duration GetDelta(const TimePoint& timeout);
    void ProcessNextTask();
    void AdvanceQueue();

    std::shared_ptr<T> m_timer;
    std::priority_queue<TaskPtr, std::vector<TaskPtr>,
                        typename Task::CompareTasks> m_task_queue;
    std::unordered_map<AlarmId, std::shared_ptr<Task>> m_tasks_map;

};

/************************  Timer Implementation   ****************************/

template <typename T>
TimerWheel<T>::TimerWheel(std::shared_ptr<T>& timer)
: m_timer(timer), m_task_queue(), m_tasks_map()
{
    // T:: saves us the vtable path and gives a direct function call
    m_timer->T::RegisterOnEvent(Bind(&TimerWheel::OnTimerEvent, this));
}

/*---------------------------------------------------------------------------*/

template <typename T>
typename TimerWheel<T>::AlarmId TimerWheel<T>::SetAlarm
            (const NanoSecs& timeout, const Function<void(int)>& func)
{
    // auto nanoSec = std::chrono::duration_cast<std::chrono::nanoseconds>(interval).count(); 
    AlarmId new_task_id = m_uid_counter++;
    TimePoint exp_time = SysClock::now() + timeout;
    
    TaskPtr new_task(new TimerWheel<T>::Task(func, exp_time, new_task_id));

    /* Add to map and queue */
    m_tasks_map[new_task_id] = new_task;
    m_task_queue.push(new_task);

    if (exp_time == m_task_queue.top()->GetExpTime())
    {
        m_timer->T::Arm(GetDelta((m_task_queue.top())->GetExpTime()));
    }
    
    return (new_task_id);
}

/*---------------------------------------------------------------------------*/

template <typename T>
void TimerWheel<T>::OnTimerEvent(int i)
{
    (void)i;
    std::cout << "OnTimerEvent" << std::endl;
    ProcessNextTask();
    AdvanceQueue();
}

/*---------------------------------------------------------------------------*/

template <typename T>
void TimerWheel<T>::ProcessNextTask()
{
    std::cout << "ProcessNextTask" << std::endl;
    TaskPtr curr_task = m_task_queue.top();
    if (!curr_task->IsTaskCancelled())
    {
        curr_task->PerformTask();
    }

    RemoveTask(curr_task);
}

/*---------------------------------------------------------------------------*/

template <typename T>
void TimerWheel<T>::AdvanceQueue()
{
    TaskPtr curr_task;

    while (!m_task_queue.empty())
    {
        std::cout << "AdvanceQueue" << std::endl;
        // curr_task.reset(&*m_task_queue.top()); => SEGFAULT!
        curr_task = m_task_queue.top();

        if (curr_task->IsTaskCancelled())
        {
            std::cout << "Removing next task since it's cancelled" << std::endl;
            RemoveTask(curr_task);
        }        

        // Duration delta = GetDelta(curr_task->GetExpTime());
        // uint64_t count = std::chrono::duration_cast<NanoSecs>(delta).count();
        // std::cout << "count = " << count << std::endl;

        else if (curr_task->GetExpTime() <= SysClock::now())
        {
            std::cout << "AdvanceQueue perform another task" << std::endl;
            ProcessNextTask();
        }

        else
        {
            std::cout << "Arming timer with future task" << std::endl;
            m_timer->T::Arm(GetDelta(curr_task->GetExpTime()));
            break;
        }
    }
}

/*---------------------------------------------------------------------------*/

template <typename T>
Duration TimerWheel<T>::GetDelta(const TimePoint& timeout)
{
    return (timeout - SysClock::now());
}

/*---------------------------------------------------------------------------*/

template <typename T>
void TimerWheel<T>::RemoveTask(const TaskPtr& task)
{
    m_tasks_map.erase(task->GetAlarmId());
    m_task_queue.pop();  
}

/*---------------------------------------------------------------------------*/

template <typename T>
void TimerWheel<T>::CancelAlarm(TimerWheel<T>::AlarmId uid)
{
    std::cout << "CancelAlarm" << std::endl;
    (m_tasks_map[uid])->CancelTask();
}

/***********************  Timer::Task Implementation   ***********************/
template <typename T>
TimerWheel<T>::Task::Task(const Function<void(int)>& func,
                          const TimePoint& exp_time, AlarmId uid)
 : m_alarm_id(uid), m_exp_time(exp_time), m_alarm_func(func),
   m_is_task_cancelled(0) {}

/*---------------------------------------------------------------------------*/

template <typename T>
void TimerWheel<T>::Task::PerformTask()
{
    std::cout << "PerformTask" << std::endl;
    m_alarm_func(0);
}

/*---------------------------------------------------------------------------*/

template <typename T>
void TimerWheel<T>::Task::CancelTask()
{
    std::cout << "CancelTask" << std::endl;
    m_is_task_cancelled = 1;
}

/*---------------------------------------------------------------------------*/

template <typename T>
typename TimerWheel<T>::AlarmId TimerWheel<T>::Task::GetAlarmId()
{
    return (m_alarm_id);
}

/*---------------------------------------------------------------------------*/

template <typename T>
TimePoint TimerWheel<T>::Task::GetExpTime()
{
    return (m_exp_time);
}

/*---------------------------------------------------------------------------*/

template <typename T>
Function<void(int)> TimerWheel<T>::Task::GetTaskFunc()
{
    return (m_alarm_func);
}

/*---------------------------------------------------------------------------*/

template <typename T>
bool TimerWheel<T>::Task::IsTaskCancelled()
{
    return (m_is_task_cancelled);
}

/*---------------------------------------------------------------------------*/

template <typename T>
bool TimerWheel<T>::Task::CompareTasks::operator()
    (const std::shared_ptr<Task>& task1, const std::shared_ptr<Task>& task2)
{
    return (task1->m_exp_time > task2->m_exp_time);
}

/******************************   ilrd::rd90   *******************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/