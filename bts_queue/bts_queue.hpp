/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#pragma once

/*****************************************************************************/

template <typename T, typename CMP>
class BtsQueue
{
public:
	BtsQueue() = default;
	~BtsQueue() = default;

	BtsQueue(const BtsQueue& other) = delete;
	BtsQueue& operator=(const BtsQueue& other) = delete;

	void Enqueue(const T& element);
	void Dequeue(T& elem);

private:
	std::priority_queue<T, std::vector<T>, CMP> m_pq;
	std::mutex m_mutex; // not copyable
	std::condition_variable m_cond_var;
};

/**************************  Class  Functions  *******************************/

template <typename T, typename CMP>
void BtsQueue<T, CMP>::Enqueue(const T& element)
{
	{
		std::unique_lock<std::mutex> u_m_lock(m_mutex);
		m_pq.push(element);
	} // lock releases at end of scope

	m_cond_var.notify_one();
}

/*---------------------------------------------------------------------------*/

template <typename T, typename CMP>
void BtsQueue<T, CMP>::Dequeue(T& elem) 
{
	std::unique_lock<std::mutex> u_m_lock(m_mutex);

	while (m_pq.empty())
	{
		m_cond_var.wait(u_m_lock);
	}

	elem = m_pq.top();
	m_pq.pop();
}

/*****************************************************************************/