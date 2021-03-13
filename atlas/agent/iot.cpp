/******************************************************************************
* Project name:					 	iot_agent            ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Jan 11, 2020          *      ****
* Project completion				Jan 13, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
#include <vector>
#include <unistd.h> /* close */
#include "atlas_header.hpp"
#include "iot.hpp"
#include "connection.hpp"

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {

/***************************   Class TcpConn   *******************************/

IoT::IoT(std::shared_ptr<IConn> connection, std::shared_ptr<IStorage> storage,
         size_t chunk_size)
 : m_connection(connection), m_storage(storage), m_chunk(chunk_size),
   m_ah(new AtlasHeader) {}

/*---------------------------------------------------------------------------*/

void IoT::Activate()
{
    while (1)
    {
        ProcessRequest();
    }
}

/*---------------------------------------------------------------------------*/

void IoT::ProcessRequest()
{
    /*
    1. read ah from connection to m_ah
    2. prepare ah for reply
    3. if read: read from storage into buffer
                send ah and buffer via connection
    4. if write: read data into buffer
                 write data from buffer to storage
                 send ah via connection
    
    IMPORTANT: if in any of these the connection is lost,
    TcpConn calls accept internaly!
    */
}

/*---------------------------------------------------------------------------*/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/