/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <memory>
#include <unistd.h>
#include "threadpool.hpp"

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {

/*****************************************************************************/
/*************************  Class  Threadpool  *******************************/
/*****************************************************************************/

Threadpool::Threadpool(size_t nof_threads) :
    m_max_threads(nof_threads),
    m_conc_level(m_max_threads),
    m_active_threads_sem(m_max_threads),
    /* m_pool(m_max_threads), */
    m_threadpool_state(new Open())
{
    m_pool.reserve(m_max_threads);
	for (size_t i = 0; i < m_conc_level; ++i)
    {
        m_pool.push_back(std::thread(&Threadpool::threadFunc, this));
        //m_pool[i] = std::thread(&Threadpool::threadFunc, this);
    }
}

/*---------------------------------------------------------------------------*/

Threadpool::~Threadpool()
{
    CloseQueueForNewTasks();
    fineTune(m_max_threads);

    // create and Enqueue Self Destruct task per thread
    for (size_t i = 0; i < m_max_threads; ++i)
    {
        Async(Function<int(void)>(&Threadpool::selfDestruct, this, 1), 
                                                        Priority::Min());
    }
    
    // join all threads
    for (size_t i = 0; i < m_max_threads; ++i)
    {
        m_pool[i].join();
    }
}

/*---------------------------------------------------------------------------*/

Threadpool::Future Threadpool::Async(Function<int(void)> func, Priority pri)
{
    return (m_threadpool_state->HandleTask(this, func, pri));
}

/*---------------------------------------------------------------------------*/

void Threadpool::threadFunc()
{
    try
    {
        std::shared_ptr<Task> next_task;
        while (1)
        {
            //std::cout << "concurrency level = " << m_conc_level << std::endl;
            //sleep(1);
            m_tasks.Dequeue(next_task);
            (*next_task).setRetVal((*next_task)());
        }
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}


/*---------------------------------------------------------------------------*/

void Threadpool::fineTune(int diff)
{
    diff = AdjustDiff(diff);
    m_conc_level += diff;

    if (diff < 0)
    {    
        while (diff)
        {
            Async(Function<int(void)>(&Threadpool::deactivateThread,
                                      this, 1), Priority::Max());
            ++diff;
        }
    }
    else
    {
        while (diff)
        {
            m_active_threads_sem.Post();
            --diff;
            /* Async(Function<int(void)>(&Threadpool::activateThread,
                                      this, 1), Priority::Max()); */
        }
    }

}

/*---------------------------------------------------------------------------*/

int Threadpool::AdjustDiff(int diff)
{
    if (diff > 0)
    {
        diff = m_conc_level + diff > m_max_threads ?
                m_max_threads - m_conc_level : diff;
    }
    else
    {
        diff = m_conc_level > 0 ?
                (m_conc_level + diff >= 0? diff : m_conc_level)
                : 0;
    }

    return (diff);
}

/*---------------------------------------------------------------------------*/

int Threadpool::activateThread(int i)
{
    (void)i;
    m_active_threads_sem.Post();
    return (0);
}

/*---------------------------------------------------------------------------*/

int Threadpool::deactivateThread(int i)
{
    (void)i;
    m_active_threads_sem.Wait();
    return (0);
}

/*---------------------------------------------------------------------------*/

void Threadpool::CloseQueueForNewTasks()
{
    // m_threadpool_state = std::shared_ptr<IState>(new Closed());
    /* There's actually a member function of shared_ptr for this */
    m_threadpool_state.reset(new Closed());
}

/*---------------------------------------------------------------------------*/

int Threadpool::selfDestruct(int i)
{
    (void)i;
    throw std::runtime_error ("terminating thread");
    return (0);
}

/*******************  Threadpool State Implementation  ***********************/
/*****************************************************************************/
/**************************  Threadpool::Open  *******************************/

Threadpool::Future Threadpool::Open::HandleTask(Threadpool* tp,
                                                Function<int(void)> func,
                                                Threadpool::Priority pri)
{
    std::shared_ptr<Task> new_task(new Task(func, pri));
    tp->m_tasks.Enqueue(new_task);
    return (Future(new_task));
}

/**************************  Threadpool::Closed  *****************************/

Threadpool::Future Threadpool::Closed::HandleTask(Threadpool* tp,
                                                 Function<int(void)> func,
                                                 Threadpool::Priority pri)
{
    if (!(pri == Threadpool::Priority::Min()))
    {
        throw("cannot add new tasks");
    }
    
    else
    {
        Open tmp_open;
        return (tmp_open.HandleTask(tp, func, pri));
    }
}

/*****************************************************************************/
/************************  Threadpool::Priority  *****************************/
/*****************************************************************************/

Threadpool::Priority::Priority(int prio_i) : m_pri(prio_i) {}

/*---------------------------------------------------------------------------*/

Threadpool::Priority Threadpool::Priority::Min()
{
    return (Priority(0));
}

/*---------------------------------------------------------------------------*/
Threadpool::Priority Threadpool::Priority::Low()
{
	return (Priority(1));
}

/*---------------------------------------------------------------------------*/

Threadpool::Priority Threadpool::Priority::Mid()
{
	return (Priority(2));
}

/*---------------------------------------------------------------------------*/

Threadpool::Priority Threadpool::Priority::High()
{
	return (Priority(3));
}

/*---------------------------------------------------------------------------*/

Threadpool::Priority Threadpool::Priority::Max()
{
	return (Priority(4));
}

/*---------------------------------------------------------------------------*/

bool Threadpool::Priority::operator<(const Priority& other)
{
	return (m_pri < other.m_pri);
}

/*---------------------------------------------------------------------------*/

bool Threadpool::Priority::operator>(const Priority& other)
{
	return (m_pri > other.m_pri);
}

/*---------------------------------------------------------------------------*/

bool Threadpool::Priority::operator==(const Priority& other)
{
	return (m_pri == other.m_pri);
}

/*****************************************************************************/
/**************************  Threadpool::Task  *******************************/
/*****************************************************************************/

Threadpool::Task::Task(Function<int(void)> func, Priority pri)
 : m_task_func(func), m_pri(pri), m_retval(0), m_task_sem(1, 0) {}

/*---------------------------------------------------------------------------*/

void Threadpool::Task::setPriority(Priority pri)
{
    m_pri = pri;
}

/*---------------------------------------------------------------------------*/

int Threadpool::Task::operator()()
{
    return (m_task_func());
}

/*---------------------------------------------------------------------------*/

void Threadpool::Task::setRetVal(int res)
{
    m_retval = res;
    m_task_sem.Post();
}

/*---------------------------------------------------------------------------*/

int Threadpool::Task::WaitForTask()
{
    m_task_sem.Wait();
    return m_retval;
}

/********************  Threadpool::Task::CompareTasks  ***********************/

bool Threadpool::Task::CompareTasks::operator()(
                                    const std::shared_ptr<Task>& task1,
                                    const std::shared_ptr<Task>& task2)
{
    return ((*task1).m_pri < (*task2).m_pri);
}

/*****************************************************************************/
/*************************  Threadpool::Future  ******************************/
/*****************************************************************************/

Threadpool::Future::Future(std::shared_ptr<Threadpool::Task> task_ptr)
 : m_task_ptr(task_ptr) {}

/*---------------------------------------------------------------------------*/

int Threadpool::Future::GetTaskResult()
{
    return ((*m_task_ptr).WaitForTask());
}

/*---------------------------------------------------------------------------*/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/