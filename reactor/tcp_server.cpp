/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "reactor.hpp"
#include "tcp_server.hpp"

#define SA struct sockaddr

/*****************************************************************************/

namespace ilrd {
namespace rd90 {

/*****************************************************************************/

TCPServer::TCPServer(Reactor<Select> *reactor, int port)
 : m_reactor(reactor), m_port(port), m_server_sockfd(-1)
{
    int option = 1;
    int sockfd;
    socklen_t sa_in_len = sizeof(struct sockaddr_in);

    InitSockAddr();

    if (0 > (sockfd = socket(PF_INET, SOCK_STREAM, 0)))
    {
        throw("socket() failed");
    }

    if(setsockopt(sockfd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR),
                  reinterpret_cast<char*>(&option), sizeof(option)) == -1)
    {
        throw;
    } 

    if (0 > (bind(sockfd, reinterpret_cast<SA*>(&m_sockaddr_server),
                  sizeof(sa_in_len))))
    {
        throw("bind() failed");
    }
    
    if (0 > (listen(sockfd, 5)))
    {
        throw("listen() failed");
    }

    m_server_sockfd = sockfd;
}

/*---------------------------------------------------------------------------*/

void TCPServer::InitSockAddr()
{
    // assign IP, PORT
    memset(&m_sockaddr_server, 0, sizeof(m_sockaddr_server));
    m_sockaddr_server.sin_family = AF_INET; 
    m_sockaddr_server.sin_addr.s_addr = htonl(INADDR_ANY); 
    m_sockaddr_server.sin_port = htons(m_port);
}

/*---------------------------------------------------------------------------*/

void TCPServer::NewClientHandler(int fd)
{
    struct sockaddr_in sockaddr_client;
    socklen_t sa_in_len = sizeof(sockaddr_client);
    int connfd = 0;

    if (0 > (connfd = accept(fd,
                             reinterpret_cast<SA*>(&sockaddr_client),
                             &sa_in_len)))
    {
        throw("accept() failed");
    }

    m_reactor->AddReadHandler(connfd, Bind(&TCPServer::ReadHandler, this));
}

/*---------------------------------------------------------------------------*/

void TCPServer::ReadHandler(int fd)
{
    static char buffer[64] = {0};
    int n = 0;
    const char *pong = "pong!";

    n = recv(fd, const_cast<char*>(buffer), 64, 0);

    if ((-1) == n)
    {
        throw("recv failed");
    }
    
    else if (0 != n)
    {
        buffer[n] = '\0';
        std::cout << "client sent: " << buffer << std::endl;

        if (0 > (n = send(fd, pong, strlen(pong), 0)))
        {
            throw("server send failed");
        }   
    }
    else // n == 0, client closed connection
    {
        m_reactor->RemoveReadHandler(fd);
    }
}

/*---------------------------------------------------------------------------*/

void TCPServer::Start()
{
    m_reactor->AddReadHandler(m_server_sockfd, Bind(&TCPServer::NewClientHandler,
                              this));
    m_reactor->Run();
}

/*****************************************************************************/
} // namespace rd90
} // namespace ilrd
