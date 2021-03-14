/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <vector>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#pragma once

/*****************************************************************************/
namespace ilrd {
namespace rd90 {
/*****************************************************************************/

class IMonitor
{
public:
    virtual ~IMonitor() = default;
    virtual void AddReadFd(int fd) = 0;
    virtual void RemoveReadFd(int fd) = 0;
    virtual int MonitorReadEvents() = 0;
    virtual int GetNextReadEventFd() = 0;
};

/*****************************************************************************/

class Select : public IMonitor
{
public:
    explicit Select();

    void AddReadFd(int fd);
    void RemoveReadFd(int fd);
    int MonitorReadEvents();
    int GetNextReadEventFd();

private:
    int m_max_fd;
    fd_set m_read_master;
    fd_set m_read_worker;

    void UpdateMaxAfterAdd(int fd);
    void UpdateMaxAfterRm(int fd);
};

/*****************************************************************************/

class Epoll : public IMonitor
{
public:
    explicit Epoll(int max_events = 1);
    ~Epoll();
    void AddReadFd(int fd);
    void RemoveReadFd(int fd);
    int MonitorReadEvents();
    int GetNextReadEventFd();

private:
    int m_epoll_fd;
    int m_max_events;
    int m_curr_ready;
    std::vector<struct epoll_event> m_events;
};

} // namespace rd90
} // namespace ilrd

/*****************************************************************************/