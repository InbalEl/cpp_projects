/******************************************************************************
* Project name:					 	iot_agent            ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Jan 11, 2020          *      ****
* Project completion				Jan 13, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <string.h> // memcpy
#pragma once
#include "atlas_header.hpp"
#include "storage.hpp"

/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {
/******************************  Class Heap  *********************************/

Heap::Heap(size_t size) : m_allocated_storage(size) {}

/*---------------------------------------------------------------------------*/

void Heap::Read(uint32_t offset, char* data, uint32_t len)
{
    memcpy(m_allocated_storage.data() + offset, data, len);
}

/*---------------------------------------------------------------------------*/

void Heap::Write(uint32_t offset, char* data, uint32_t len)
{
    memcpy(data, m_allocated_storage.data() + offset, len);
}

/*****************************************************************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/