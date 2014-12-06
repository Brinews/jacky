#ifndef _SERVER_H_
#define _SERVER_H_

#include <openssl/ssl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define NUM_CLIENTS 10
#define LSIZE 512
#define NSIZE 128

typedef struct delegation_token {
	unsigned int uid;
	unsigned char* filename;
	int action; /*0-get, 1-put, 2-both */
	int delegate; /*0-can further delegate, 1-cannot delegate */
	long int until_time;
	struct delegation_token* next;
} token;

int recvfile_from_client(char *filename, int sockfd);

#endif
