/******************************************************************************
* Project name:					 	iot_agent            ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Jan 11, 2020          *      ****
* Project completion				Jan 13, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
#include <memory>
#include <vector>
#include <netdb.h> // AI_PASSIVE, addrinfo
#pragma once
#include "atlas_header.hpp"

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {
/****************************   Class IConn   ********************************/

class IConn
{
public:
    IConn() = default; // TODO
    virtual ~IConn() = default;

    IConn(const IConn&) = delete;
    IConn& operator=(const IConn&) = delete;

    virtual void Send(std::vector<char>& buffer, size_t len) = 0;
    virtual void Receive(std::vector<char>& buffer, size_t len) = 0;
};

/***************************   Class IStorage   ******************************/

class TcpConn : public IConn
{
public:
    explicit TcpConn(size_t port, int port_postfix);
    ~TcpConn();

    void Send(std::vector<char>& buffer, size_t len);
    /* void Send(std::vector<char>& buffer, size_t len); */ // sendmsg
    void Receive(std::vector<char>& buffer, size_t len);

private:
    int Accept();
    int CreateSocket(int family, int socktype, int flags);
    static void InitHints(addrinfo* hints, int family, int socktype, int flags);

    int m_passive_sockfd;
    int m_communication_fd;
    std::shared_ptr<char> m_port;
};

/*---------------------------------------------------------------------------*/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/