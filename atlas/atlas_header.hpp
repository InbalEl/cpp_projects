/******************************************************************************
* Project name:					 	req_disp             ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Dec 02, 2020          *      ****
* Project completion				Dec 03, 2020          *      *
*                                                        ***  *  *******
******************************************************************************/
/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdint.h>
#pragma once
/*****************************  ilrd::ol90  **********************************/
namespace ilrd {
namespace rd90 {
/************************  Struct AtlasHeader  *******************************/

typedef struct AtlasHeader
{
    uint32_t    m_requestUid;   // not to be used by IoT; must be first field
    uint32_t    m_fragmentNum;  // not to be used by IoT
    uint32_t    m_alarmUid;     // not to be used by IoT
    uint32_t    m_iotOffset;
    uint32_t    m_type;
    uint32_t    m_len;          // how many bytes to write OR to read
}AtlasHeader;

/*****************************************************************************/
} // namespace rd90
} // namespace ilrd
/*****************************************************************************/