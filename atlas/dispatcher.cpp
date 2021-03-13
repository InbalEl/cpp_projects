/******************************************************************************
* Project name:					 	req_disp             ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Dec 02, 2020          *      ****
* Project completion				Dec 03, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "bio_access.h"
#include "threadpool.hpp"
#include "reactor.hpp"
#include "dispatcher.hpp"
#include "distributor.hpp"
#include "monitors.hpp"

/*****************************************************************************/
/*
Code review #2:
 - check DevOpen for return value
 - create a read all function
 - run the line Eli told us about (in matching hpp file)
 - Add the NBD ioctl that Eli mentioned in whatsapp (also in screenshot)
 - no point in reusing socket (setsockopt) if we don't bind - meaning,
   if we're not the server (disp is the client in this context
 - Don't bother too much with cleanups at the moment, just throw
 - for hiding the pointer in the atlas header struct - *(biorequest **)&a_header->first_member = request;
 - create wrappers for those NBD macros
*/

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {

typedef struct addrinfo addrinfo_t;
typedef struct sockaddr sockaddr_t;
static const char *port = "29000";

/**********************  Class RequestDispatcher  ****************************/

RequestDispatcher::RequestDispatcher(Reactor<Epoll>& react, int bio_fd,
                               std::shared_ptr<IDistributor> dist)
 : m_uid_counter(0), m_react(react), m_distributor(dist), m_slicers()
{
    m_react.AddReadHandler(bio_fd, Bind(&RequestDispatcher::RequestHandler, this));
}

/*---------------------------------------------------------------------------*/

void RequestDispatcher::RegisterIoT(const std::string& ip_addr)
{
    int iot_sockfd = OpenIotSock(ip_addr);
    m_react.AddReadHandler(iot_sockfd,
                           Bind(&RequestDispatcher::ReplyHandler, this));

    m_distributor->AddIotFd(iot_sockfd);
}

/*---------------------------------------------------------------------------*/

int RequestDispatcher::OpenIotSock(const std::string& ip_addr)
{
    addrinfo hints;
    addrinfo *serverinfo;
    int iot_sockfd = 0;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype= SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    std::cout << ip_addr.data() << std::endl;

    /* get address info */
    if (getaddrinfo(ip_addr.data(), port, &hints, &serverinfo))
    {
        throw("getaddrinfo");
    }
    
    /* get socket fd */
    iot_sockfd = socket(serverinfo->ai_family, serverinfo->ai_socktype,
                        serverinfo->ai_protocol);

    std::cout << "iot_socket = " << iot_sockfd << std::endl;                                             

    if (0 > iot_sockfd)
    {
        throw("socket");
    }
    
    if (connect(iot_sockfd, serverinfo->ai_addr, serverinfo->ai_addrlen))
    {
        throw("connect");
    }
    
    return (iot_sockfd);
}

/*---------------------------------------------------------------------------*/

void RequestDispatcher::RequestHandler(int bio_fd)
{
    std::shared_ptr<Slicer> slicer(new Slicer(bio_fd, m_uid_counter));
    std::cout << "RequestDispatcher::RequestHandler new reqid = " << m_uid_counter << std::endl;
    m_slicers[m_uid_counter++] = slicer;
    m_distributor->Distribute(slicer);
}

/*---------------------------------------------------------------------------*/

void RequestDispatcher::ReplyHandler(int iot_fd)
{
    uint32_t req_uid = Slicer::GetRequestID(iot_fd);
    std::cout << "RequestDispatcher::ReplyHandler reqid = " << req_uid << std::endl;
    if (m_slicers[req_uid]->HandleReply(iot_fd, req_uid))
    {
        std::cout << "HandleReply returned nonzero - erasing slicer" << std::endl;
        m_slicers.erase(req_uid); // in ~Slicer(): BioRequestDone(req, 0);
    }
}

/*****************************************************************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/