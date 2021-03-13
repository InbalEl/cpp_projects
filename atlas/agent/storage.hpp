/******************************************************************************
* Project name:					 	iot_agent            ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Jan 11, 2020          *      ****
* Project completion				Jan 13, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
#include <fstream>
#include <vector>
#include <string>
#pragma once

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {

/***************************   Class IStorage   ******************************/

class IStorage
{
public:
    IStorage() = default; // TODO
    virtual ~IStorage() = default;

    IStorage(const IStorage&) = delete;
    IStorage& operator=(const IStorage&) = delete;

    virtual void Read(uint32_t offset, char* data, uint32_t len) = 0;
    virtual void Write(uint32_t offset, char* data, uint32_t len) = 0;
};

/****************************   Class Heap    ********************************/

class Heap : public IStorage
{
public:
    explicit Heap(size_t size);
    ~Heap() = default;

    virtual void Read(uint32_t offset, char* data, uint32_t len);
    virtual void Write(uint32_t offset, char* data, uint32_t len);

private:    
    std::vector<char> m_allocated_storage;
};

/****************************   Class Heap    ********************************/

class Disk : public IStorage
{
public:
    explicit Disk(const std::string& filename)
    : m_storage_file(filename.data(), std::fstream::in | std::fstream::out)
    {}

    ~Disk() = default;

    virtual void Read(uint32_t offset, char* data, uint32_t len);
    virtual void Write(uint32_t offset, char* data, uint32_t len);

private:
    std::fstream m_storage_file;
};

/*---------------------------------------------------------------------------*/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/