#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LSIZE 512
#define GETSIZE 512
#define PUTSIZE 382
#define BASESIZE 684
#define NSIZE 128

struct delegation_token {
	unsigned int uid;
	unsigned char* filename;
	int action; //0-get, 1-put, 2-both
	int delegate; //0-can further delegate, 1-cannot delegate
	long int until_time;
	struct delegation_token* next;
} typedef token;

// tell whether a file with name f exists or not
int exists(char* f);

// return "client_uid"
char* get_prefix(unsigned int uid);

// initilize, check or create key and certificate for the current active user
void init(char* prefix);

// wrapper of perror
void err(char* msg);

// connect with the server and return the socket
int start_session(SSL* ssl);

// terminate the session with server
int end_session(SSL* ssl);

// send a GET request to the server to fetch file with name *filename
int GET(char *filename, SSL* ssl, int sock);

// send a PUT request to the server to upload file with name *filename
int PUT(char* filename, SSL* ssl, int sock);

// delegate rights (action) for file *filename to user with ID uid
unsigned char* delegate(unsigned int uid, unsigned char* filename, unsigned long until_time, int action);

// the advanced version of delegate to allow the rights to be further delegated
unsigned char* delegate_plus(unsigned int uid, unsigned char* filename, unsigned int until_time, int action);

// send token file to server, return file size sended
int sendfile_to_server(char *filename, int sockfd);

#endif
