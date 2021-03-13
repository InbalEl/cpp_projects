/******************************************************************************
* Project name:					 	req_disp             ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Dec 02, 2020          *      ****
* Project completion				Dec 03, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <memory>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include "slicer.hpp"
#include "atlas_header.hpp"
#include "distributor.hpp"
/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {

/*****************************************************************************/
void ReadAll(int fd, char* buffer, size_t len);
const static int frag_size = 1024;
/****************************  class Slicer  *********************************/

Slicer::Slicer(int bio_fd, uint32_t req_uid)
 : m_bio_fd(bio_fd), m_req_uid(req_uid), m_fragments(), m_bio_req(NULL)
{
    m_bio_req = BioRequestRead(bio_fd);
    if (NULL == m_bio_req)
    {
        throw("BioRequestRead");
    }
}

/*---------------------------------------------------------------------------*/

Slicer::~Slicer()
{
    BioRequestDone(reinterpret_cast<BioRequest*>(&*m_bio_req), 0);
}

/*---------------------------------------------------------------------------*/

uint32_t Slicer::GetReqSize()
{
    return (m_bio_req->dataLen);
}

/*---------------------------------------------------------------------------*/

uint32_t Slicer::GetReqType()
{
    return (m_bio_req->reqType);
}

/*---------------------------------------------------------------------------*/

uint32_t Slicer::GetReqOffset()
{
    return (m_bio_req->offset);
}

/*---------------------------------------------------------------------------*/

void Slicer::AddFragment(int frag_num)
{
    m_fragments.insert(frag_num);
}

/*---------------------------------------------------------------------------*/

void Slicer::PerformTask(std::shared_ptr<Task> task)
{
    while (!task->m_fragments.empty())
    {
        Task::Fragment frag = task->m_fragments.back();
        WriteFragment(task->m_iot_fd, frag.frag_num, frag.local_iot_index);
        task->m_fragments.pop_back();
    }
}

/*---------------------------------------------------------------------------*/

void InitIovec(struct iovec *msg_data, int num_in_vec , void* add, size_t size)
{
    msg_data[num_in_vec].iov_base = add;
    msg_data[num_in_vec].iov_len = size;
}

/*---------------------------------------------------------------------------*/

void Slicer::WriteFragment(int iot_fd, int frag, int local_index)
{
    struct msghdr message_header;
    struct iovec *message_data;
    uint32_t req_type = m_bio_req->reqType; 
    bool is_write = (req_type == NBD_CMD_WRITE) ? 1 : 0; 
    message_data = new iovec[is_write + 1];

    AtlasHeader iot_ah;

    iot_ah.m_requestUid = m_req_uid;
    iot_ah.m_len = frag_size;
    iot_ah.m_fragmentNum = frag;
    iot_ah.m_type = req_type;
    iot_ah.m_iotOffset = local_index * frag_size;
    iot_ah.m_alarmUid = 0;

    memset(&message_header, 0, sizeof(message_header));

    message_header.msg_iov = message_data;
    message_header.msg_name = NULL;
    message_header.msg_namelen = 0;

    InitIovec(message_data, 0 , &iot_ah, sizeof(AtlasHeader));
    message_header.msg_iovlen = 1;

    if(is_write)
    {
        InitIovec(message_data, 1 , m_bio_req->dataBuf + (frag * frag_size),
                  frag_size);
        message_header.msg_iovlen = 2;
    }

    if (sendmsg(iot_fd, &message_header, 0) < 0)
    {
        throw("sendmsg");
    }  
  
    delete[] message_data;
}

/*---------------------------------------------------------------------------*/

bool Slicer::HandleReply(int iot_fd, uint32_t req_uid)
{
    AtlasHeader iot_reply_ah;
    memset(&iot_reply_ah, 0, sizeof(AtlasHeader));
    iot_reply_ah.m_requestUid = req_uid;

    std::cout << "Slicer::HandleReply" << std::endl;
    std::cout << "reqid = " << req_uid << std::endl;
    std::cout << "iot_fd = " << iot_fd << std::endl;

    ReadAll(iot_fd,
            reinterpret_cast<char*>(&iot_reply_ah) + sizeof(uint32_t),
            sizeof(AtlasHeader) - sizeof(uint32_t));

    std::cout << "first read all completed" << std::endl;

    if (iot_reply_ah.m_type == NBD_CMD_READ)
    {
        std::cout << "read all for NBD_CMD_READ" << std::endl;
        ReadAll(iot_fd, (m_bio_req->dataBuf) + (iot_reply_ah.m_fragmentNum * frag_size),
        frag_size);
    }
    
    std::cout << "erasing fragment" << std::endl;
    m_fragments.erase(iot_reply_ah.m_fragmentNum);

    std::cout << "returning" << std::endl;
    return (m_fragments.empty());
}

/*---------------------------------------------------------------------------*/

uint32_t Slicer::GetRequestID(int iot_fd)
{
    uint32_t req_uid;
    ReadAll(iot_fd, reinterpret_cast<char *>(&req_uid), sizeof(uint32_t));
    return (req_uid);
}

/*****************************************************************************/

void ReadAll(int fd, char* buffer, size_t len)
{
    int bytes_read;

    while (len) 
    {
        std::cout << "inside ReadAll" << std::endl;
        if ((-1) == (bytes_read = read(fd, buffer, len)))
        {
            throw("read failed\n");
        }
        buffer += bytes_read;
        len -= bytes_read;
    }
}

/*****************************************************************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/