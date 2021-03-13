/******************************************************************************
* Project name:					 	iot_agent            ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Jan 11, 2020          *      ****
* Project completion				Jan 13, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
#include <vector>
#include <cstdio>
#include <stdexcept> /* std:runtime_error */
#include <unistd.h> /* close */
#include <string.h> /* memset */
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h> // ioctl
#include <netdb.h> // AI_PASSIVE, addrinfo
#include "atlas_header.hpp"
#include "connection.hpp"

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {

/***************************   Class TcpConn   *******************************/

TcpConn::TcpConn(size_t port, int port_postfix)
 : m_port(new char[6]),
   m_passive_sockfd(CreateSocket(AF_INET, SOCK_STREAM, AI_PASSIVE))
{
    sprintf(&*m_port, "%d", port + port_postfix); // TODO: fix &*
    listen(m_passive_sockfd, 10);
    m_communication_fd = Accept();
}

/*---------------------------------------------------------------------------*/

int TcpConn::CreateSocket(int family, int socktype, int flags)
{
    addrinfo hints;
    InitHints(&hints, family, socktype, flags);

    addrinfo* servinfo;
    if (0 > getaddrinfo(NULL, &*m_port, &hints, &servinfo)) // TODO: fix &*
    {
        throw(std::runtime_error("getaddrinfo"));
    }

    int sockfd(SetSocket(servinfo));

    if(0 > bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen))
    {
        throw(std::runtime_error("bind"));
    }

    freeaddrinfo(servinfo);

    return(sockfd);
}

/*---------------------------------------------------------------------------*/

void TcpConn::InitHints(addrinfo* hints, int family, int socktype, int flags)
{
    memset(hints, 0, sizeof(*hints));
    hints->ai_family = family; /* Allow IPv4 */
    hints->ai_socktype = socktype; /* socket */
    hints->ai_flags = flags; /* assign the address of my local host */
}

/*---------------------------------------------------------------------------*/

int SetSocket(addrinfo *servinfo)
{  
    int sockfd = 0;
    int sockopt = 1;
            
    if (0 > (sockfd = socket(servinfo->ai_family, servinfo->ai_socktype,
                             servinfo->ai_protocol)))
    {
        throw(std::runtime_error("socket"));
    }
  
    if (0 > setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt,
                       sizeof(sockopt)))
    {
        throw(std::runtime_error("setsockopt"));
    }
 
    return (sockfd);
}

/*---------------------------------------------------------------------------*/

int TcpConn::Accept()
{
    sockaddr_storage client_addr;
    socklen_t client_addr_len = 0;
    int accept_fd = 0;

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr_len = sizeof(client_addr);

    if (0 > (accept_fd = accept(m_passive_sockfd,
                                reinterpret_cast<sockaddr *>(&client_addr),
                                &client_addr_len)))
    {
        throw(std::runtime_error("accept"));
    }

    return (accept_fd);
}

/*---------------------------------------------------------------------------*/

TcpConn::~TcpConn()
{
    close(m_passive_sockfd);
}

/*---------------------------------------------------------------------------*/

void TcpConn::Send(std::vector<char>& buffer, size_t len)
{

}

/*---------------------------------------------------------------------------*/

void TcpConn::Receive(std::vector<char>& buffer, size_t len)
{

}

/*---------------------------------------------------------------------------*/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/