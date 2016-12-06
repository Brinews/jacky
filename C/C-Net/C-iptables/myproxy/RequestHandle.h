#ifndef _RequestHandle_h
#define _RequestHandle_h


#include "HttpRequest.h"
#include "HttpResponse.h"

#include "defs.h"

/// A structure to hold a file descriptor and a message.
typedef struct {
	/// Socket/File Descriptor.
	int sock;
	
	/// The address in the communications space of the socket.
	struct sockaddr_in address;
	
	/// Message sent by fd.
	char *msg;
} sock_msg;

/// Handle an HTTP request.
void *handleRequest(void *arg);

#endif
