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