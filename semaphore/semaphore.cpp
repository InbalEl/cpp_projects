/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include "semaphore.hpp"
/**************************  Class  Functions  *******************************/

Semaphore::Semaphore(size_t max_resources, size_t init_value)
 : m_max_resources(max_resources), m_curr_val(init_value) {}

/*---------------------------------------------------------------------------*/

size_t Semaphore::GetValue()
{
    m_mtx.lock();
    size_t curr_val = m_curr_val;
	m_mtx.unlock();
    return (curr_val);
}

/*---------------------------------------------------------------------------*/

void Semaphore::Wait()
{
    std::unique_lock<std::mutex> u_lock(m_mtx);

	while (!m_curr_val)
	{
		m_cv.wait(u_lock);
	}

	--m_curr_val;
}

/*---------------------------------------------------------------------------*/

void Semaphore::Post()
{
	std::unique_lock<std::mutex> u_lock(m_mtx);

	if (m_curr_val < m_max_resources)
	{
		++m_curr_val;
		m_cv.notify_one();
	}
}

/*****************************************************************************/