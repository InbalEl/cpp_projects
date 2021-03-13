/******************************************************************************
* Project name:					 	Distributor          ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Dec 16, 2020          *      ****
* Project completion				Dec ??, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <memory>
#include <vector>
#include <cstddef> /* size_t */
#include <stdint.h>
#include <unistd.h>
#include "slicer.hpp"

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {
const static int frag_size = 1024;
/*****************************  Class Task  **********************************/

Task::Task(int iot_fd) : m_iot_fd(iot_fd) {}

/**************************  Class Distributor  ******************************/

DistModulo::DistModulo() : m_iot_fd_map(), m_nof_iots(0) {}

/*---------------------------------------------------------------------------*/

void DistModulo::AddIotFd(int iot_fd)
{
    m_iot_fd_map[m_nof_iots++] = iot_fd;
}

/*---------------------------------------------------------------------------*/

void DistModulo::RemoveIotFd(int iot_fd)
{
    m_iot_fd_map.erase(m_iot_fd_map.find(iot_fd));
    --m_nof_iots;
}

/*------------------------------------------------------------ in request---------------*/

void DistModulo::Distribute(std::shared_ptr<Slicer> slicer)
{
    std::unordered_map<int, std::shared_ptr<Task>> tasks;
    size_t nof_fragments = slicer->GetReqSize() / frag_size;
    unsigned int first_iot = (slicer->GetReqOffset() / frag_size) % m_nof_iots;
    unsigned int global_index = slicer->GetReqOffset() / frag_size;

    /* Find number of tasks */
    unsigned int nof_tasks = m_nof_iots * (nof_fragments >= m_nof_iots) +
                    nof_fragments * (nof_fragments < m_nof_iots);

    /* Create tasks */
    for (size_t i = 0; i < nof_tasks; ++i)
    {
        tasks[(first_iot + i) % m_nof_iots] =
        std::shared_ptr<Task>(new Task(m_iot_fd_map[(first_iot + i) % m_nof_iots]));
    }
    
    Task::Fragment curr_frag;

    /* Add fragments to tasks */
    for (size_t i = 0; i < nof_fragments; ++i, ++global_index)
    {
        curr_frag.frag_num = i;
        curr_frag.local_iot_index = global_index % m_nof_iots;
        tasks[(first_iot + i) % m_nof_iots]->m_fragments.push_back(curr_frag);
        slicer->AddFragment(static_cast<int>(i));
    }

    // Call slicer->PerformTask(task) for each task created
    for (size_t i = 0; i < nof_tasks; ++i)
    {
        size_t curr = (first_iot + i) % m_nof_iots;
        slicer->PerformTask(tasks[curr]);
        tasks.erase(curr);
    }
}

/*****************************************************************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/