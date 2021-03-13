/******************************************************************************
* Project name:					 	bio_access           ***     *******
* Developer: 						Inbal Elmalech        *      *
* Project Lauch: 					Nov 18, 2020          *      ****
* Project completion				Nov 19, 2020          *      *
* Reviewer:                                              ***  *  *******
******************************************************************************/
/*********************    PREPROCESSOR DIRECTIVES    *************************/
#define _POSIX_C_SOURCE (200809L)
#include <stddef.h> /* offsetof */
#include <errno.h> /* perror */
#include <fcntl.h> /* open */
#include <linux/nbd.h> /* NBD_CMD_READ ,NBD_CMD_WRITE */
#include <netinet/in.h> /* ntohl */
#include <stdio.h> /* perror */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* memcpy */
#include <sys/ioctl.h> /* ioctl */
#include <sys/socket.h> /* u_int64_t u_int32_t*/
#include <sys/wait.h> /* waitpid */
#include <unistd.h> /* read, write */
#include "bio_access.h"

/*****************************************************************************/
/*
 * These helper functions were taken from cliserv.h in the nbd distribution.
 */
#ifdef WORDS_BIGENDIAN
uint64_t ntohll( uint64_t a) {
    return a;
}
#else
uint64_t Ntohll( uint64_t a)
{
     uint32_t lo = a & 0xffffffff;
     uint32_t hi = a >> 32U;
    lo = ntohl(lo);
    hi = ntohl(hi);
    return (( uint64_t) lo) << 32U | hi;
}
#endif
#define htonll ntohll

/*******************************  Structures  *********************************/

typedef struct nbd_message
{
    BioRequest bio_Req;
    int nbd_socket;
    struct nbd_reply reply;
    char buffer[1];
}NbdMessage;

/*********************  Static Function Declarations  *************************/

static void KernelSide(int sockfd);
static void WriteAll(int fd, char* buf, size_t len);
static int ReadAll(int fd, char* buf, size_t len);

/***********************  API Function Definition  ***************************/

int BioDevOpen(const char* dev_file, uint64_t size)
{
    int sp[2] = {0};
    int nbd = 0, err = 0;
    pid_t pid = 0;

    /* not regular errors - we can put them together. If one fails,
    the others will and nothing will work. */
    if ((-1) == (socketpair(AF_UNIX, SOCK_STREAM, 0, sp)))
    {
        perror("socketpair: ");
        return (-1);
    }

    /* Opening nbd socket */
    if ((-1) == (nbd = open(dev_file, O_RDWR)))
    {
        perror("open: ");
		close(sp[1]);
		close(sp[0]);
        return (-1);
    }

    /* add size to nbd_driver */
    err -= ioctl(nbd, NBD_SET_SIZE, size);          /* Setting message size */
    err -= ioctl(nbd, NBD_CLEAR_QUE);               /* cleaning queue */
    err -= ioctl(nbd, NBD_CLEAR_SOCK);              /* Cleaning socket */
    err -= ioctl(nbd, NBD_SET_SOCK, sp[1]);          /* setting socket */
    err += ioctl(nbd, NBD_SET_TIMEOUT, 1);
    
    if (err != 0)
    {
		close(sp[1]);
		close(sp[0]);
        return (-1);
    }
    
    /* forking child to run NBD_DO_IT */
    pid = fork();

    if ((-1) == pid)
    {
        perror("fork: ");
		close(sp[1]);
		close(sp[0]);
        return (-1);
    }
    
    /* Inside child */
    if (0 == pid)
    {
        close(sp[0]);
        printf("Running NBD_DO_IT in child\n");
        KernelSide(nbd);
        return (-1);
    }

    /* Inside parent */
    close(sp[1]);
    if (-1 == (waitpid(pid, 0, WNOHANG)))
    {
        return (-1);
    }

    return (sp[0]);
}

/*---------------------------------------------------------------------------*/

BioRequest* BioRequestRead(int bioDesc)
{
    NbdMessage* nbd_message = NULL;
    struct nbd_request request;
    size_t data_len = 0;
    uint32_t req_type = 0;

    /* Reading request from socket */
    ReadAll(bioDesc, (char *)&request, sizeof(struct nbd_request));

    /*if (request.magic != htonl(NBD_REQUEST_MAGIC))
    {
        printf("magic number doesn't match NBD_REQUEST_MAGIC\n");
        return (NULL);
    }*/

    req_type = ntohl(request.type);

    if ((req_type == NBD_CMD_READ) || (req_type == NBD_CMD_WRITE))
    {
        data_len = ntohl(request.len); /*  */
    }

    /* Allocate memory for nbd_message */
    if (NULL == (nbd_message = (NbdMessage *)malloc
                                (offsetof(NbdMessage, buffer) + data_len)))
    {
        return (NULL);
    }

    nbd_message->reply.magic = htonl(NBD_REPLY_MAGIC);
    nbd_message->reply.error = htonl(0);
    memcpy(nbd_message->reply.handle, request.handle, sizeof(nbd_message->reply.handle));

    nbd_message->bio_Req.reqType = req_type;
    nbd_message->bio_Req.dataLen = data_len;
    nbd_message->bio_Req.offset = Ntohll(request.from);
    nbd_message->bio_Req.dataBuf = nbd_message->buffer;
    nbd_message->nbd_socket = bioDesc;

    if (nbd_message->bio_Req.reqType == NBD_CMD_WRITE)
    {
        ReadAll(bioDesc, (nbd_message->buffer), data_len);
    }

    return ((BioRequest*)nbd_message);
}

/*---------------------------------------------------------------------------*/

void BioRequestDone(BioRequest* req, int error)
{
    NbdMessage *nbd_msg = (NbdMessage *)req;
    size_t len_to_write = sizeof(struct nbd_reply);
    nbd_msg->reply.error = htonl(error); /* htonl? */
    
    len_to_write += (req->reqType == NBD_CMD_READ) * nbd_msg->bio_Req.dataLen;

    WriteAll(nbd_msg->nbd_socket, (char*)&(nbd_msg->reply), len_to_write);
    
    free(nbd_msg);
}

/*********************  Static Function Definition  **************************/

static void KernelSide(int sockfd)
{
    int err = 0;
    if ((-1) == (err = ioctl(sockfd, NBD_DO_IT)))
    {
        exit(EXIT_FAILURE);
    } 

    ioctl(sockfd, NBD_CLEAR_QUE);
    ioctl(sockfd, NBD_CLEAR_SOCK); 
}

/*---------------------------------------------------------------------------*/

static int ReadAll(int fd, char* buf, size_t len)
{
    int bytes_read = 0;

    while (len)
    {
        bytes_read = read(fd, buf, len);
        if (bytes_read > 0)
        {
            return (bytes_read);
        }
        buf += bytes_read;
        len -= bytes_read;
    }

    return (0);
}

/*---------------------------------------------------------------------------*/

static void WriteAll(int fd, char* buf, size_t len)
{
    int bytes_written = 1;

    while (len && bytes_written)
    {
        bytes_written = write(fd, buf, len);
        buf += bytes_written;
        len -= bytes_written;
    }
}

/*****************************************************************************/

#if(0)

#define _POSIX_C_SOURCE (200809L)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <linux/nbd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "bio_access.h"

#define DEV_OPN_FAILURE -1

/* Utility functions declaration */
u_int64_t Ntohll(u_int64_t a);
static int WriteAll(int fd, char* buf, size_t count);

typedef struct 
{
    BioRequest 		 request;
    int 			 nbd_socket;
    struct nbd_reply reply;
    char 			 buffer[1];
} nbd_message;


/*----------------------------------------------------------------------------*/
int BioDevOpen(const char* dev_file, uint64_t size)
{
	int sp[2];
	int nbd, err;

	if (-1 == (err = socketpair(AF_UNIX, SOCK_STREAM, 0, sp)))
	{
		puts("can't create socket pair");
		return (DEV_OPN_FAILURE);
	}
	

	/* achieving file descriptor for /dev/nbd0 */
	if (-1 == (nbd = open(dev_file, O_RDWR)))
	{
		puts("can't open");
		close(sp[1]);
		close(sp[0]);
		return (DEV_OPN_FAILURE);
	}
		
	if (-1 == (err = ioctl(nbd, NBD_SET_SIZE, size)))
	{
		return (DEV_OPN_FAILURE);
	}
	
		
	ioctl(nbd, NBD_CLEAR_QUE);
	if (-1 == (err = ioctl(nbd, NBD_CLEAR_SOCK)))
	{
		return (DEV_OPN_FAILURE);
	}


	if(ioctl(nbd, NBD_SET_SOCK, sp[1]))
	{
		return (DEV_OPN_FAILURE);
	}


	pid_t pid = fork();
	if (-1 == pid)
	{
		return (DEV_OPN_FAILURE);
	}

	if (0 == pid) /* child process */
	{
		close(sp[0]);
		puts("before do it");
		
		ioctl(nbd, NBD_DO_IT);

		puts("after do it");
		exit(EXIT_SUCCESS);
	}
	else /* parent process */
	{
		close(sp[1]);

		if (-1 == (waitpid(pid, 0, WNOHANG)))
        {
            return (DEV_OPN_FAILURE);
        }
	}

	close(sp[1]);

	return (sp[0]);
}
/*----------------------------------------------------------------------------*/
BioRequest* BioRequestRead(int bioDesc)
{
	nbd_message* message = NULL;
	struct nbd_request request;
	struct nbd_reply reply;
	size_t data_len = 0;

	/* reads request message */
	ReadAll(bioDesc, (char*)&request, sizeof(struct nbd_request));
	
	/* handles only read and write request */
	data_len = (ntohl(request.type) == NBD_CMD_READ || ntohl(request.type) == NBD_CMD_WRITE) * 
				ntohl(request.len);
		
	 /* alloc new nbd_message */
    message = (nbd_message *)malloc(offsetof(nbd_message, buffer) + data_len);
    
    if (NULL == message)
    {
        return (NULL);
    }
    
    /* init reply members */
    reply.magic = htonl(NBD_REPLY_MAGIC);
    reply.error = htonl(0);
    memcpy(reply.handle, request.handle, sizeof(reply.handle));

    /* init message */
    message->request.reqType = ntohl(request.type);
    message->request.dataLen = data_len;
    message->request.offset = Ntohll(request.from);
    message->request.dataBuf = message->buffer;

    /* init reply in message */
    memcpy(&message->reply, &reply, sizeof(struct nbd_reply));
    /* init socket in message */
    message->nbd_socket = bioDesc;

		
	if(message->request.reqType == NBD_CMD_WRITE)
    {
       ReadAll(bioDesc, message->request.dataBuf, message->request.dataLen);
    }
	
                          
    return ((BioRequest*)message);
	
}
/*----------------------------------------------------------------------------*/
void BioRequestDone(BioRequest *req, int error)
{
	nbd_message *message = (nbd_message*)req;
	message->reply.error = htonl(error); 

	switch (req->reqType)
	{
		case (NBD_CMD_WRITE):

	        WriteAll(message->nbd_socket, (char*)&(message->reply),
	                 sizeof(struct nbd_reply));
		break;

    	case (NBD_CMD_READ):
    
        	WriteAll(message->nbd_socket, (char*)&(message->reply),
                 sizeof(struct nbd_reply) + message->request.dataLen);
    		break;

		default:
			break;
	}
    
    free(req);

}

/*----------------------------------------------------------------------------*/
#ifdef WORDS_BIGENDIAN
u_int64_t Ntohll(u_int64_t a) {
  return a;
}
#else
u_int64_t Ntohll(u_int64_t a) 
{
  u_int32_t lo = a & 0xffffffff;
  u_int32_t hi = a >> 32U;
  lo = ntohl(lo);
  hi = ntohl(hi);
  return ((u_int64_t) lo) << 32U | hi;
}
#endif
#define htonll Ntohll
/*----------------------------------------------------------------------------*/
/* reads from fd to buf */
int ReadAll(int fd, char* buf, size_t count)
{
  int bytes_read;

  while (count > 0) 
  {
    bytes_read = read(fd, buf, count);
    buf += bytes_read;
    count -= bytes_read;
  }

  return 0;
}
/*----------------------------------------------------------------------------*/
/* writes to fd from buf */
static int WriteAll(int fd, char* buf, size_t count)
{
  int bytes_written;

  while (count > 0) 
  {
    bytes_written = write(fd, buf, count);
    buf += bytes_written;
    count -= bytes_written;
  }

  return 0;
}

#endif