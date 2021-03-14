#include <vector>           // vector
#include <unordered_map>    // umap
#include <string>           // string
#include <memory>
#pragma once
#include "reactor.hpp"      // reactor
#include "monitors.hpp"     // epoll
#include "slicer.hpp" // slicer
#include "distributor.hpp" // Distributor

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {
/************************   Classes declarations    **************************/

class RequestDispatcher
{
public:
    // if also registers handler of bio_fd, needs to pass as a paremeter
    explicit RequestDispatcher(Reactor<Epoll>& react, int bio_fd,
                               std::shared_ptr<IDistributor> dist); 

    void RegisterIoT(const std::string& ip_addr); // will also register handler 
                                                  // in reactor?
    // void RegisterHandler(int bio_fd);   // if handlers' registration is explicit

    RequestDispatcher(const RequestDispatcher&) = delete;
    RequestDispatcher& operator=(const RequestDispatcher&) = delete;
    ~RequestDispatcher() = default;

private:
    void RequestHandler(int bio_fd);
    void ReplyHandler(int iot_fd);
    /* void TimerHandler(int iot_fd); */
    static int OpenIotSock(const std::string& ip_addr);

    uint32_t m_uid_counter;
    Reactor<Epoll>& m_react;
    std::shared_ptr<IDistributor> m_distributor;
    std::unordered_map<uint32_t, std::shared_ptr<Slicer>> m_slicers;
};

/*************************   Class Implementation    *************************/

/***************************   Public functions    ***************************/

/*---------------------------------------------------------------------------*/

} // namespace rd90
} // namespace ilrd



