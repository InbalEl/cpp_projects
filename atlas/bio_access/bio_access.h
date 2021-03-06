#pragma once
#include <stdint.h>
#include <linux/nbd.h>  /* shouldn't depend on nbd enum - we chould wrap it */

/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    uint32_t   reqType;
    uint32_t   dataLen;
    uint64_t   offset;
    char*      dataBuf;
} BioRequest;

int BioDevOpen(const char* dev_file, uint64_t size);
BioRequest* BioRequestRead(int bioDesc);
void BioRequestDone(BioRequest* req, int error);
/* void BioDevDisconnect(); */
/*int ReadAll(int fd, char* buf, size_t count);*/
#ifdef __cplusplus
}
#endif


/*****************************************************************************/