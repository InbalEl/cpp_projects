#if(0)


#if (0)
/*
 * busexmp - example memory-based block device using BUSE
 * Copyright (C) 2013 Adam Cozzette
 *
 * This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
review:

- no c99 :(
- mkfs.ext2 -c -c /dev/nbd0 will give better feedback for whether nkfs works
- simplify parsing - see screenshot
- the bio_access user should not see the word NBD
- ctrl + C will kill parent, and the TCP connection will help the child
see that the connection is broken and exit the DO_IT loop.
so no need for wait in parent.
- no asserts for sys calls
- API function definition in the top of the file,
static functions at the bottom
- u_int vs. uint - without underline should be more portable, check and change
- write, in the context of sockets (and especially TCP) tends to block
until all data has been written, which mean we don't need the
WriteAll function. Read up on this! (according to Eli, that's his experience).
If we had an MT issue (if Done all would have been possibly called from
different threads) - we would have needed a mutex for writing.
for a regular write - the system implements a mutex.
Read, on the other hand, does require a read all because it's possible in
this context to get back less bytes than we requesred to read.

- no mention of NBD should be in the test - create wrappers for defines!
*/

#define _POSIX_C_SOURCE (200809L)

#include <stdint.h>
#include <err.h>
#include <stdio.h>
#include <argp.h>
#include <stdlib.h>
#include <string.h>

#include "bio_access.h"

/* BUSE callbacks */
static void *data;

/* argument parsing using argp */

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output", 0},
    {0},
};

struct arguments {
    uint64_t size;
    char * device;
    int verbose;
};

static uint64_t strtoull_with_prefix(const char * str, char * * end);
/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state);

static struct argp argp = {options, parse_opt, "SIZE DEVICE",
                           "BUSE virtual block device that stores its"
                           "content in memory.\n"
                           "`SIZE` accepts suffixes K, M, G. `DEVICE` is"
                           "path to block device, for example \"/dev/nbd0\".",
                           NULL, NULL, NULL};

typedef void (*req_handle_func_r)(char *buff, uint32_t len,
                                  uint64_t offset, int verbose);

void HandleReadRequest(char *buff, uint32_t len, uint64_t offset, int verbose);
void HandleWriteRequest(char *buff, uint32_t len, uint64_t offset, int verbose);

/*****************************************************************************/

int main(int argc, char *argv[])
{
    req_handle_func_r ReqHandlingLut[2];
    
    int nbd_sockfd = 0;
    int error = 0;
    BioRequest *nbd_req = NULL;

    struct arguments arguments = {0};
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (NULL == (data = malloc(arguments.size)))
    {
        err(EXIT_FAILURE, "failed to alloc space for data");
    }

    if (0 > (nbd_sockfd = BioDevOpen(arguments.device, arguments.size)))
    {
        free(data);
        printf("BioDevOpen: error\n");
        return (1);
    }

    ReqHandlingLut[NBD_CMD_READ] = &HandleReadRequest;
    ReqHandlingLut[NBD_CMD_WRITE] = &HandleWriteRequest;

    while (1)
    {
        /* select */
        if (NULL == (nbd_req = BioRequestRead(nbd_sockfd)))
        {
            free(data);
            printf("BioRequestRead: error\n");
            return (1);
        }

        ReqHandlingLut[nbd_req->reqType](nbd_req->dataBuf,
                                         nbd_req->dataLen,
                                         nbd_req->offset,
                                         arguments.verbose);
    }
    
    BioRequestDone(nbd_req, error);

    return(0);
}

/*****************************************************************************/

void HandleReadRequest(char *buff, uint32_t len, uint64_t offset, int verbose)
{
    if (verbose)
    {
        fprintf(stderr, "R - %lu, %u\n", offset, len);
    }
    memcpy(buff, (char *)data + offset, len);
}

/*---------------------------------------------------------------------------*/

void HandleWriteRequest(char *buff, uint32_t len, uint64_t offset, int verbose)
{
    if (verbose)
    {
        fprintf(stderr, "W - %lu, %u\n", offset, len);
    }
    memcpy((char *)data + offset, buff, len);
}

/*****************************************************************************/

static uint64_t strtoull_with_prefix(const char * str, char * * end) {
    uint64_t v = strtoull(str, end, 0);
    switch (**end) {
        case 'K':
            v *= 1024;
            *end += 1;
            break;
        case 'M':
            v *= 1024 * 1024;
            *end += 1;
            break;
        case 'G':
            v *= 1024 * 1024 * 1024;
            *end += 1;
            break;
    }
    return v;
}

/*---------------------------------------------------------------------------*/
/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    char * endptr;

    switch (key) {

        case 'v':
            arguments->verbose = 1;
            break;

        case ARGP_KEY_ARG:
            switch (state->arg_num) {

                case 0:
                    arguments->size = strtoull_with_prefix(arg, &endptr);
                    if (*endptr != '\0') {
                        /* failed to parse integer */
                        errx(EXIT_FAILURE, "SIZE must be an integer");
                    }
                    break;

                case 1:
                    arguments->device = arg;
                    break;

                default:
                    /* Too many arguments. */
                    return ARGP_ERR_UNKNOWN;
            }
            break;

        case ARGP_KEY_END:
            if (state->arg_num < 2) {
                warnx("not enough arguments");
                argp_usage(state);
            }
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/*****************************************************************************/

#endif

#define _POSIX_C_SOURCE (200809L)
#include <argp.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/nbd.h>
#include <netinet/in.h>
#include <assert.h>
#include <unistd.h>

#include "bio_access.h"

#include <argp.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <assert.h>
#include <unistd.h>

#include "bio_access.h"

#ifndef BUSE_DEBUG
  #define BUSE_DEBUG (0)
#endif

/* function declarations */
static void *data;
static int ReadAll(int fd, char* buf, size_t count);

static int HandleRead(void *buf, u_int32_t len, u_int64_t offset)
{
  fprintf(stderr, "R - %lu, %u\n", offset, len);
    
  memcpy(buf, (char *)data + offset, len);

  return (0);
}

static int HandleWrite(const void *buf, u_int32_t len, u_int64_t offset)
{
  fprintf(stderr, "W - %lu, %u\n", offset, len);
    
  memcpy((char *)data + offset, buf, len);

  return (0);
}

/* argument parsing using argp */
static struct argp_option options[] = 
{
  {"verbose", 'v', 0, 0, "Produce verbose output", 0},
  {0},
};

struct arguments {
  uint64_t size;
  char *device;
  int verbose;
};

static uint64_t strtoull_with_prefix(const char * str, char * * end) 
{
  uint64_t v = strtoull(str, end, 0);
  
  switch (**end) 
  {
    case 'K':
      v *= 1024;
      *end += 1;
      break;

    case 'M':
      v *= 1024 * 1024;
      *end += 1;
      break;

    case 'G':
      v *= 1024 * 1024 * 1024;
      *end += 1;
      break;
  }

  return (v);
}

/* Parse a single option. */
static error_t ParseOpt(int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;
  char *endptr;

  switch (key)
  {

    case 'v':
      arguments->verbose = 1;
      break;

    case ARGP_KEY_ARG:
      switch (state->arg_num) 
      {

        case 0:
          arguments->size = strtoull_with_prefix(arg, &endptr);

          if (*endptr != '\0') 
          {
            /* failed to parse integer */
            errx(EXIT_FAILURE, "SIZE must be an integer");
          }

          break;

        case 1:
          arguments->device = arg;
          break;

        default:
          /* Too many arguments. */
          return (ARGP_ERR_UNKNOWN);
      }

      break;

    case ARGP_KEY_END:
      if (state->arg_num < 2) 
      {
        warnx("not enough arguments");
        argp_usage(state);
      }

      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }

  return (0);
}

static struct argp argp = {options, ParseOpt, "SIZE DEVICE",
            "BUSE virtual block device that stores its content in memory.\n"
         "`SIZE` accepts suffixes K, M, G. `DEVICE` is path to block device, for example \"/dev/nbd0\".",
         NULL, NULL, NULL};



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

int main(int argc, char *argv[]) 
{
  int nbd_socket = 0;
  BioRequest* next_request = NULL;
	uint32_t error = 0;

  struct arguments arguments = {0};

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  data = malloc(arguments.size);

  if (data == NULL) 
  {
    err(EXIT_FAILURE, "failed to alloc space for data");
  }

  printf("dev size is : %ld\t dev name is: %s\n", arguments.size, arguments.device);
  /*int nbd_socket = buse_main(arguments.device, &aop, (void *)&arguments.verbose);*/
  nbd_socket = BioDevOpen(arguments.device, arguments.size);

  while (1)
  {
    /* serve NBD socket */
     next_request = BioRequestRead(nbd_socket);
		if (NULL == next_request)
		{
			fprintf(stderr, "request read failed\n");
			close(nbd_socket);
      
			return(EXIT_FAILURE);
		}
		
		switch(next_request->reqType)
		{

		case NBD_CMD_READ:
					
			error = HandleRead(next_request->dataBuf, 
							next_request->dataLen, 
							next_request->offset);
			break;

		case NBD_CMD_WRITE:
	
			ReadAll(nbd_socket, next_request->dataBuf, next_request->dataLen);

			error = HandleWrite(next_request->dataBuf, 
							 next_request->dataLen, 
							 next_request->offset);
			break;
		}

		BioRequestDone(next_request, error);
  }

  return (0);
}

#endif

#include <argp.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/nbd.h>
#include <netinet/in.h>
#include <assert.h>
#include <unistd.h>

#include "bio_access.h"

#define DEV_OPN_FAILURE -1

/* BUSE callbacks */
static void *data; /* represent the device */

static error_t ParseOpt(int key, char *arg, struct argp_state *state);
static void UserRead(void *buf, u_int32_t len, u_int64_t offset);
static void UserWrite(const void *buf, u_int32_t len, u_int64_t offset);
static unsigned long long StrToUllWithPrefix(const char *str, char **end);

/*----------------------------------------------------------------------------*/


struct arguments 
{
	unsigned long long size;
	char 			   *device;
	int 			   verbose;
};
/*----------------------------------------------------------------------------*/


/* argument parsing using argp */

static struct argp_option options[] = 
{
  {"verbose", 'v', 0, 0, "Produce verbose output", 0},
  {0},
};
/*----------------------------------------------------------------------------*/

static struct argp argp = 
{
  .options = options,
  .parser = ParseOpt,
  .args_doc = "SIZE DEVICE",
  .doc = "BUSE virtual block device that stores its content in memory.\n"
         "`SIZE` accepts suffixes K, M, G. `DEVICE` is path to block device,"
		 " for example \"/dev/nbd0\".",
};

/*----------------------------------------------------------------------------*/
int main(int argc, char *argv[]) 
{
	int nbd_socket = 0;
    int error = 0;
    BioRequest *bioRequest = NULL;

	struct arguments arguments = 
	{
		.verbose = 0,
	};

	argp_parse(&argp, argc, argv, 0, 0, &arguments);
	printf("size = %lld\n", arguments.size);
	data = malloc(arguments.size);
	if (NULL == data)
	{
		perror("malloc failed");
		return (EXIT_FAILURE);
	}

	nbd_socket = BioDevOpen(arguments.device, arguments.size);
	if (DEV_OPN_FAILURE == nbd_socket)
	{
		perror("BioDevOpen failed");
		puts(arguments.device);
		free(data);
		return (EXIT_FAILURE);
	}

	while (1)
	{
		
		bioRequest = BioRequestRead(nbd_socket);
		if (NULL == bioRequest)
		{
			free(data);
			return (EXIT_FAILURE);
		}

		switch (bioRequest->reqType)
		{
			case (NBD_CMD_READ):

				UserRead(bioRequest->dataBuf, bioRequest->dataLen, bioRequest->offset);
				break;

			case (NBD_CMD_WRITE):

				UserWrite(bioRequest->dataBuf, bioRequest->dataLen, bioRequest->offset);
				break;

			default:
				break;
		}

	BioRequestDone(bioRequest, error);
	}


	return 0;
}

static void UserRead(void *buf, u_int32_t len, u_int64_t offset)
{
	fprintf(stderr, "R - %lu, %u\n", offset, len);

	memcpy(buf, (char *)data + offset, len);
}
/*----------------------------------------------------------------------------*/
static void UserWrite(const void *buf, u_int32_t len, u_int64_t offset)
{
	fprintf(stderr, "W - %lu, %u\n", offset, len);

	memcpy((char *)data + offset, buf, len);
}

/*----------------------------------------------------------------------------*/


static unsigned long long StrToUllWithPrefix(const char *str, char **end) 
{
	unsigned long long v = strtoull(str, end, 0);

	switch (**end) 
	{
		case 'K':
			v *= 1024;
			*end += 1;
			break;
		case 'M':
			v *= 1024 * 1024;
			*end += 1;
			break;
		case 'G':
			v *= 1024 * 1024 * 1024;
			*end += 1;
			break;
	}
	return v;
}
/*----------------------------------------------------------------------------*/
/* Parse a single option. */
static error_t ParseOpt(int key, char *arg, struct argp_state *state) 
{
  struct arguments *arguments = state->input;
  char * endptr;

  switch (key) 
  {

    case 'v':
      arguments->verbose = 1;
      break;

    case ARGP_KEY_ARG:
      switch (state->arg_num) 
      {

        case 0:
          arguments->size = StrToUllWithPrefix(arg, &endptr);
          if (*endptr != '\0') 
          {
            /* failed to parse integer */
            errx(EXIT_FAILURE, "SIZE must be an integer");
          }
          break;

        case 1:
          arguments->device = arg;
          break;

        default:
          /* Too many arguments. */
          return ARGP_ERR_UNKNOWN;
      }
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 2) 
      {
        warnx("not enough arguments");
        argp_usage(state);
      }
      break;

    default:
		return ARGP_ERR_UNKNOWN;
  }

  return 0;
}