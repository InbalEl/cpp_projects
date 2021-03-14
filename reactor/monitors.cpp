/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <cstdio>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/epoll.h> // for epoll_create1(), epoll_ctl(), struct epoll_event
#include <unistd.h>
#include "monitors.hpp"
#define MAX_EVENTS 5

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {
/****************************  Class Select  *********************************/

Select::Select(): m_max_fd(-1) 
{
    FD_ZERO(&m_read_master);
    FD_ZERO(&m_read_worker);
}

/*---------------------------------------------------------------------------*/

void Select::AddReadFd(int fd)
{
    FD_SET(fd, &m_read_master);
    UpdateMaxAfterAdd(fd);
}

/*---------------------------------------------------------------------------*/

void Select::UpdateMaxAfterAdd(int fd)
{
    m_max_fd = (fd * (m_max_fd < fd)) + (!(m_max_fd < fd) * m_max_fd);
}

/*---------------------------------------------------------------------------*/

void Select::RemoveReadFd(int fd)
{
    FD_CLR(fd, &m_read_master);
    UpdateMaxAfterRm(fd);
}

/*---------------------------------------------------------------------------*/

void Select::UpdateMaxAfterRm(int fd)
{
    fd_set master = m_read_master;
    int curr_max = m_max_fd;
    int new_max = 0;
    int i = 0;

    if (fd == curr_max)
    {
        for (i = 0; i < curr_max; ++i)
        {
            if (FD_ISSET(i, &master))
            {
                new_max = i;
            }
        }
    }

    m_max_fd = new_max;
}

/*---------------------------------------------------------------------------*/

int Select::GetNextReadEventFd()
{
    int maxfd = m_max_fd;
    int i = 0;

    for (i = 0; i <= maxfd; ++i)
    {
        if (FD_ISSET(i, &m_read_worker))
        {
            return (i);
        }
    }

    return (-1);
}

/*---------------------------------------------------------------------------*/

int Select::MonitorReadEvents()
{
    m_read_worker = m_read_master;
    return select(m_max_fd + 1, &m_read_worker, NULL, NULL, NULL);
}

/*****************************  Class Epoll  *********************************/

Epoll::Epoll(int max_events)
 : m_epoll_fd(epoll_create1(0)),
   m_max_events(max_events),
   m_curr_ready(-1), m_events(m_max_events)
{}

/*---------------------------------------------------------------------------*/

Epoll::~Epoll()
{
    close(m_epoll_fd);
}

/*---------------------------------------------------------------------------*/

void Epoll::AddReadFd(int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (0 > epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &ev))
    {
        throw("epoll_ctl failed");
    }
}

/*---------------------------------------------------------------------------*/

void Epoll::RemoveReadFd(int fd)
{
    if (0 > close(fd))
    {
        throw("close");
    }
}

/*---------------------------------------------------------------------------*/

int Epoll::MonitorReadEvents()
{
    if (0 > (m_curr_ready = epoll_wait(m_epoll_fd, 
                                             m_events.data(),
                                             m_max_events, -1)))
    {
        throw("epoll_wait failed");
    }

    return (m_curr_ready);
}

/*---------------------------------------------------------------------------*/

int Epoll::GetNextReadEventFd()
{
    return (m_events[--m_curr_ready].data.fd);
}

/*****************************************************************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/