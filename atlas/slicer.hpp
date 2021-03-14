/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <iostream>
#include <memory>
#include <vector>
#include <set>
#include <stdint.h>
#pragma once
#include "bio_access/bio_access.h"
#include "distributor.hpp"

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {
/****************************  class Slicer  *********************************/

class Slicer
{
public:
    explicit Slicer(int bio_fd, uint32_t req_uid);
    ~Slicer();
    Slicer(const Slicer& other) = delete;
    Slicer& operator=(const Slicer& other) = delete;

    uint32_t GetReqOffset();    
    uint32_t GetReqSize();    
    uint32_t GetReqType();
    void AddFragment(int frag_num);

    void PerformTask(std::shared_ptr<Task> task);
    bool HandleReply(int iot_fd, uint32_t req_uid);
    static uint32_t GetRequestID(int iot_fd);

private:
    void WriteFragment(int fd, int frag, int local_index);

    int m_bio_fd;
    uint32_t m_req_uid;
    std::set<int> m_fragments;
    BioRequest* m_bio_req; // contains ptr to full message buffer
};

/*****************************************************************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/