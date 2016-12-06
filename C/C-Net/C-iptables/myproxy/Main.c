#include <stdio.h>
#include <stdlib.h>

#include <limits.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <pthread.h>

#include <sys/errno.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "defs.h"
#include "MyFilter.h"
#include "RequestHandle.h"
#include "StringHelper.h"

extern int errno;

#define BUFFER_SIZE 2048

#define MAX_CLIENTS 128
#define MAX_THREADS 128

#define START_INDEX_FILTERS 2

int main(int argc, const char *argv[])
{
	unsigned short port = 8129;
	if ( argc > 1 ) {
		int pt = atoi(argv[1]);
		port = (unsigned short) pt;
	}
	
	filtersCount = malloc(sizeof(unsigned int));
	*filtersCount = 0;
	
	if ( argc > START_INDEX_FILTERS ) {
		*filtersCount = argc - START_INDEX_FILTERS;
		filters = &argv[START_INDEX_FILTERS];
	}
	
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	if ( sock < 0 ) {
		perror("socket()");
		exit(1);
	}
	
	// Create the server.
	struct sockaddr_in server;
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	
	// Bind.
	socklen_t len = sizeof(server);
	if ( bind(sock, (struct sockaddr *) &server, len) < 0 ) {
		perror("bind()");
		exit(1);
	}
	
	// Create the client.
	struct sockaddr_in client;
	socklen_t fromlen = sizeof(client);
	listen(sock, 5); // 5 is the number of backlogged waiting clients.
	printf("Listener socket created and bound to port %d\n", port);
	
	// Threads
	pthread_t tid[MAX_THREADS];
	
	while (true) {
		// Accept client connection.
		int fd = accept(sock, (struct sockaddr *) &client, &fromlen);
		
#ifdef DEBUG
		printf("Accepted client connection on fd: %d\n", fd);
#endif
		
		// Create a buffer to read the message into.
		char buffer[BUFFER_SIZE];
		
		// Receive the message.
		ssize_t n = recv(fd, buffer, BUFFER_SIZE - 1, 0);
		// Check recv() return value.
		
		// Stream has errored or ended.
		if ( n <= 0 ) {
			// Errored.
			perror("recv()");
			continue;
		} else {
			// Stream received message.
			buffer[n] = '\0';
		}
		
		// Create a thread to handle message.
		sock_msg *arg = malloc(sizeof(sock_msg));
		arg->sock = fd;
		arg->address = server;
		arg->msg = stringDuplicate(buffer);

		if ( pthread_create(&tid[fd], NULL, handleRequest, (void *) arg) != 0 ) {
			perror("Could not create thread.");
			free(arg->msg);
		}
	}
	
	return EXIT_SUCCESS;
}
