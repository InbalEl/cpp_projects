						  /*********************
						  *	 ***     *******   *
						  *	  *      *		   *
						  *	  *      ****	   *
						  *	  *      *		   *
						  *  ***  *  *******   *
						  **********************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <mutex>
#include <condition_variable>
#pragma once

/*******************************  inbalel  ***********************************/
namespace inbalel {
/***************************  Class Semaphore  *******************************/

class Semaphore
{
public:
	explicit Semaphore(size_t max_resources = 1, size_t init_value = 0);
	void Wait();
	void Post();
	size_t GetValue();

private:
	size_t m_max_resources;
	size_t m_curr_val;
	std::mutex m_mtx;
	std::condition_variable m_cv;
};

/*---------------------------------------------------------------------------*/
} // namespace inbalel
/*****************************************************************************/