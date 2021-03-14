/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "reactor.hpp"
#include "monitors.hpp"
#pragma once

/*****************************************************************************/
namespace ilrd {
namespace rd90 {
/*****************************************************************************/

class TCPServer
{
public:
    explicit TCPServer(Reactor<Select> *reactor, int port = 5000);
    void Start(); // vontains reactor.Run();
private:
    Reactor<Select> *m_reactor;
    int m_port;
    int m_server_sockfd;
    struct sockaddr_in m_sockaddr_server;

    void NewClientHandler(int fd);
    void ReadHandler(int fd);
    void InitSockAddr();
};

/*****************************************************************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/