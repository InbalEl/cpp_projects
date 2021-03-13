/*
int main(void)
{
    // open connection to nbd 
    int bio_fd = BioDevOpen();

    // create reactor 
    Reactor<Epoll> react(new Epoll(3));

    // create ReqDisp 
    RequestDispatcher disp(react, bio_fd);
    
    // Register IoTs
    disp.RegisterIoT("4.5.5.2");
    disp.RegisterIoT("4.5.5.3");
    disp.RegisterIoT("4.5.5.4");

    // register handlers ? 

    // run
    react.Run();
    // disp.Start(); ? 
}


We will have to run this command:
config for device drivers, using a command with a redirection
this makes requests smaller
we need to do this right after modprobe
It'll make the requests 4k in size
sudo sh -c "echo 4 > /sys/block/nbd0/queue/max_sectors_kb"

only after this, we can do mkfs

* DON'T MOUNT AFTER MKFS!!!!!

Exiting order - first close daemon, then ctrl_c on iot servers

*/


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



