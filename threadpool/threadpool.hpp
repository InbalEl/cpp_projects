/******************************************************************************
* Project name:					 	threadpool           ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Dec 07, 2020          *      ****
* Project completion				Dec 10, 2020          *      *
* Reviewer:                                              ***  *  *******
******************************************************************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <vector>
#include <memory>
#include <future>
#include <thread>
#include "bts_queue.hpp"
#include "function.hpp"
#include "semaphore.hpp"

#pragma once

/*
int main ()
{
	Threadpool t_pool(4);
	std::future<> task1 = Async(Function<int(void)> , low);
	std::future<> task2 = Async(Function<int(void)> , mid);
	std::future<> task3 = Async(Function<int(void)> , high);
	std::future<> task4 = Async(Function<int(void)> , low);

	return (0);
}

- tasks must be copyable

*/

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {

/**************************  Class Threadpool  *******************************/

class Threadpool
{
private:
	class Task;
public:
	class Future;

/************************  Threadpool::Priority  *****************************/
	class Priority
	{
	public:
		/* private ctor; generated dtor cctor and oper= */
		static Priority Low();
		static Priority Mid();
		static Priority High();

		bool operator<(const Priority& other);
		bool operator>(const Priority& other);
		bool operator==(const Priority& other);

	private:
		friend class Threadpool; //TODO: check if necessary
		explicit Priority(int prio_i = 0);
		static Priority Max();
		static Priority Min();

		int m_pri;
	}; // class Priority

/*************************  Threadpool::Future  ******************************/

	class Future
	{
	public:
		explicit Future(){}
		explicit Future(std::shared_ptr<Task> task_ptr);
		int GetTaskResult();

	private:
		std::shared_ptr<Threadpool::Task> m_task_ptr;
	}; // class Future

/*****************************************************************************/

	explicit Threadpool(size_t nof_threads = 1);
	~Threadpool();
	Threadpool(const Threadpool& other) = delete;
	Threadpool& operator=(const Threadpool& other) = delete;

	void fineTune(int diff);

	Future Async(Function<int(void)> func, Priority pri);
	/*
	A comment about this function:
	If the Future returning from Async is not saved, it'll be a temporary
	and upon reaching the ; at the end of the line, the shared_ptr in
	Future will be destroyed.
	If we save it locally in a variable, it'll also be destroyed, but at the
	end of the scope.
	*/

private: // in class Threadpool

/**************************  Threadpool::Task  *******************************/
	
	class Task
	{
	public:
		explicit Task();
		explicit Task(Function<int(void)> func, Priority pri);
		void setPriority(Priority pri);
		int operator()();
		int WaitForTask();
		void setRetVal(int res);
		struct CompareTasks
		{
			bool operator()(const std::shared_ptr<Task>& task1,
							const std::shared_ptr<Task>& task2);
		};

	private:
		Function<int(void)> m_task_func;
		Priority m_pri;
		int m_retval;
		Semaphore m_task_sem;
	}; // class Task

/*************************  Threadpool::IState  ******************************/
// TODO: REMOVE FROM HPP
	class IState
	{
	public:
		IState() = default;
		~IState() = default;

		virtual Future HandleTask(Threadpool* tp, Function<int(void)> func,
                                  Threadpool::Priority pri) = 0;
	}; // class IState

/**************************  Threadpool::Open  *******************************/

	class Open : public IState
	{
	public:
		~Open() = default;
		Future HandleTask(Threadpool* tp, Function<int(void)> func,
                                                Threadpool::Priority pri);
	}; // class Open

/************************  Threadpool::Closed  *******************************/

	class Closed : public IState
	{
	public:
		~Closed() = default;
		Future HandleTask(Threadpool* tp, Function<int(void)> func,
                                                  Threadpool::Priority pri);
	}; // class Closed

/*---------------------------------------------------------------------------*/

	void threadFunc();
    int activateThread(int i);
    int deactivateThread(int i);
    int selfDestruct(int i);
	int AdjustDiff(int diff);
	void CloseQueueForNewTasks();

	size_t m_max_threads;
	size_t m_conc_level;
	Semaphore m_active_threads_sem;

	std::vector<std::thread> m_pool;
	std::shared_ptr<IState> m_threadpool_state;
	BtsQueue<std::shared_ptr<Task>, Threadpool::Task::CompareTasks> m_tasks;
}; // class Threadpool

/*---------------------------------------------------------------------------*/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/