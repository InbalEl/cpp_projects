/******************************************************************************
* Project name:					 	Distributor          ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Dec 16, 2020          *      ****
* Project completion				Dec ??, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <memory>
#include <unordered_map>
#include <vector>
#pragma once
#include "slicer.hpp"

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {
/*****************************  Class Task  **********************************/

class Task
{
public:
    explicit Task(int iot_fd); 

    struct Fragment
    {
        int frag_num;
        int local_iot_index;
    };

    int m_iot_fd;
    std::vector<Fragment> m_fragments;
};

/**********************  Abstract Class IDistributor  ************************/
class Slicer;

class IDistributor
{
public:
    virtual void Distribute(std::shared_ptr<Slicer> slicer) = 0;
    virtual void AddIotFd(int iot_fd) = 0;
    virtual void RemoveIotFd(int iot_fd) = 0;
};

/***************************  Class DistModulo  ******************************/

class DistModulo : public IDistributor
{
public:
    explicit DistModulo();
    ~DistModulo() = default;

    DistModulo(const DistModulo&) = delete;
    DistModulo& operator=(const DistModulo&) = delete;

    virtual void Distribute(std::shared_ptr<Slicer> slicer);
    virtual void RemoveIotFd(int iot_fd);
    virtual void AddIotFd(int iot_fd);

private:
    /* key = iot number, value = iot fd */
    std::unordered_map<int, int>m_iot_fd_map;
    size_t m_nof_iots;
};

/*****************************************************************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/

#if(0)

class Task
{
public:
    Task(/* args */);
    ~Task();

private:
    std::
};


class slicer
{
public:
    slicer(/* args */);
    ~slicer();

    static int GetRequestID();
    void PerformTask(Task *t); // in a loop, sends all slices on their own
    void HandleReply(); // maybe can give back status indicating whether it was the last reply or not

private:
    std::set<int>;
    BioReq; // contains ptr to full message buffer
};

class Distributor
{
public:
    Distributor(/* args */);
    ~Distributor();

    Distribute(slicer *s); // creates a Task per IoT device, and
    // calls s->PerformTask(Task) for each one
};

slicer::HandleReply()
{
    ReadHeader();
    ReadDataIntoBuffer(); // the header will contain the offset in BioReq buffer,
    // to write the reply into
    EraseSlicefromSet();
}


/*---------------------------------------------------------------------------*/

dispatcher::HandleRequest(int fd)
{
    slicer s = CreateSlicerObj(BioGetRequest());
    InsertToMap(s);
    distributor.Distribute(s);
}
/*---------------------------------------------------------------------------*/

dispatcher::HandleReply()
{
    uid = slicer::GetRequestID();
    slice * s = RetrieveSlicerfromMap();
    if (LastRply = s->HandleReply())
    {
        BioDone() // perhaps inside ~Slicer?
        RemovefromMap(s);
    }
}
#endif
/*---------------------------------------------------------------------------*/
