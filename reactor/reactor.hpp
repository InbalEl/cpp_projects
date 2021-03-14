/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <memory>
#include <iostream>
#include <unordered_map>
#include "function.hpp"
#include "monitors.hpp"
#pragma once

/*****************************************************************************/
namespace ilrd {
namespace rd90 {

template <typename T>
class Reactor
{
public:
	explicit Reactor(T* mon);
	~Reactor() = default;
	Reactor(const Reactor&) = delete;
	Reactor& operator=(const Reactor&) = delete;

	void AddReadHandler(int fd, const Function<void(int)>& handler);
	void RemoveReadHandler(int fd);
	void Run();

private:
	// strategy (policy based design) - SOLID
	std::shared_ptr<T> m_monitor;
	// to avoid copying the monitor, we'll prefer a pointer to the actual object
	std::unordered_map<int, Function<void(int)>> m_umap;
};

/**************************  Class  Functions  *******************************/

template <typename T>
Reactor<T>::Reactor(T* mon) : m_monitor(mon) // m_umap?
{
	IMonitor* please_inherit_IMonitor = mon; // forcing user to
	// inherit by requiring the possibility of an implicit cast
	(void)please_inherit_IMonitor;

	// alternatively:
	// (void)static_cast<IMonitor*>(mon);
}

/*---------------------------------------------------------------------------*/

template <typename T>
void Reactor<T>::AddReadHandler(int fd, const Function<void(int)>& handler)
{
	m_monitor->T::AddReadFd(fd);
	m_umap[fd] = handler;
}

/*---------------------------------------------------------------------------*/

template <typename T>
void Reactor<T>::RemoveReadHandler(int fd)
{
	m_umap.erase(fd);
	m_monitor->T::RemoveReadFd(fd);
}

/*---------------------------------------------------------------------------*/

template <typename T>
void Reactor<T>::Run()
{
    while (1)
    {
		int nof_fds = m_monitor->T::MonitorReadEvents();
		std::cout << "Reactor<T>::Run() read events = " << nof_fds << std::endl;
        for (int i = 0; i < nof_fds; ++i)
        {
			int next_fd = m_monitor->T::GetNextReadEventFd();
			std::cout << "next read event fd = " << next_fd << std::endl;
			(m_umap[next_fd])(next_fd);
        }
    }
}

/*---------------------------------------------------------------------------*/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/