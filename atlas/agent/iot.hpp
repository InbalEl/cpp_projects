/******************************************************************************
* Project name:					 	iot_agent            ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Jan 11, 2020          *      ****
* Project completion				Jan 13, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
#include <memory>
#pragma once
#include "atlas_header.hpp"
#include "connection.hpp"
#include "storage.hpp"

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {
/*****************************   Class IoT   *********************************/

class IoT
{
public:
    explicit IoT(std::shared_ptr<IConn> connection,
                 std::shared_ptr<IStorage> storage,
                 size_t chunk_size);

    ~IoT() = default;

    IoT(const IoT& other) = delete;
    IoT& operator=(const IoT& other) = delete;

    void Activate();

private:
    void ProcessRequest();

    std::shared_ptr<IConn> m_connection;
    std::shared_ptr<IStorage> m_storage;
    size_t m_chunk;
    std::vector<char> m_data_buffer;
    std::shared_ptr<AtlasHeader> m_ah; // TODO: is this dangerous?
};

/*---------------------------------------------------------------------------*/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/