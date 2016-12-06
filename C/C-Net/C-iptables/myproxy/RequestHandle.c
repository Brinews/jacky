#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "RequestHandle.h"

#include "MyFilter.h"
#include "StringHelper.h"

#define BUFFER_SIZE 2048


// Parse an HTTP request.
HTTPRequest processRequest(char *requestString, int *error);

// Determine whether a request should be filtered out or not.
bool shouldAllowRequest(HTTPRequest request);

// Determine whether a server should be filtered out or not.
bool shouldAllowServer(const char *server);

// Send an HTTP Status Line to a socket.
bool sendHTTPStatusToSocket(int status, int client);

void *handleRequest(void *argument)
{
	// Unpack argument into variables.
	sock_msg *arg = (sock_msg *)argument;
	int fd = arg->sock;
	struct sockaddr_in client = arg->address;
	char *requestString = arg->msg;
	
	// Process the request string into a HTTPRequest.
	int error = 0;
	HTTPRequest request = processRequest(requestString, &error);
	if ( !request ) {
		sendHTTPStatusToSocket(error, fd);
		
		goto end;
	}
	
	/* When detects a request that should be filtered, return an HTTP error 403 (forbidden) */
	
	// Read the IP Address into a string.
	char *ip_addr = inet_ntoa((struct in_addr)client.sin_addr);
	
	// Figure out if the request should be filtered out.
	if ( !shouldAllowRequest(request) ) {
		printf("%s: %s [FILTERED]\n", ip_addr, request[HttpReqHeaderField_Request_Line]);
		
		// Send back HTTP Error 403 Forbidden.
		sendHTTPStatusToSocket(403, fd);
		goto end;
	} else {
		// Print Request Line.
		printf("%s: %s\n", ip_addr, request[HttpReqHeaderField_Request_Line]);
	}
	
	/* forward the appropriate HTTP request headers to the requested server, then send the responses back to the client.  */
	
	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if ( serverSocket < 0 ) {
		perror("socket()");
		goto end;
	}
	
	// Server
	struct sockaddr_in server;
	server.sin_family = PF_INET;
	
	struct hostent *hp = gethostbyname(request[HttpReqHeaderField_Host]);
	if ( hp == NULL ) {
		perror("Unknown host");
		goto end;
	}
	
	// Could also use memcpy
	bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
	unsigned short port = 80;
	server.sin_port = htons(port);
	
	// Connect.
	if ( connect(serverSocket, (struct sockaddr *)&server, sizeof(server) ) < 0 ) {
		perror("connect()");
		goto end;
	}
	
	// Strip out Accept-Encoding to prevent chunking (not yet supported).
	char *accept_encoding = request[HttpReqHeaderField_Accept_Encoding];
	if ( accept_encoding != NULL ) {
		free(accept_encoding);
		request[HttpReqHeaderField_Accept_Encoding] = NULL;
	}
	
	// Get the request string.
	char *serverRequestString = requestStringFromRequest(request);
	if ( !serverRequestString ) {
		goto end;
	}
	
	// Send.
	ssize_t send_n = send(serverSocket, serverRequestString, strlen(serverRequestString), 0);
	if ( send_n < strlen(serverRequestString) ) {
		perror("send()");
		goto end;
	}
	
	// Buffer to load received messages into.
	char buffer[BUFFER_SIZE];
	
	// Receive.
	while (1) {
		// BLOCK
		ssize_t received_n = recv(serverSocket, buffer, BUFFER_SIZE - 1, 0);
		if ( received_n == 0 ) {
			// Peer has closed its half side of the (TCP) connection.
			break;
		} else if ( received_n < 0 ) {
			perror("recv()");
			goto end;
		} else {
			buffer[received_n] = '\0';
			
#ifdef DEBUG
			//printf("\n\nReceived message from %s:\n%s\n\n", inet_ntoa((struct in_addr)server.sin_addr), buffer);
#endif
			
			ssize_t send_client_n = send(fd, buffer, strlen(buffer), 0);
			if ( send_client_n < strlen(buffer) ) {
				perror("send()");
				goto end;
			}
		}
	}
	
end:
	
	if ( requestString ) {
		free(requestString);
	}
	
	if ( serverRequestString ) {
		free(serverRequestString);
	}
	
	close(serverSocket);
	
	if ( request != NULL ) {
		HTTPRequestFree(request);
	}
	
	close(fd);
	
	pthread_exit(NULL);
	
	return NULL;
}

HTTPRequest processRequest(char *requestString, int *error)
{
	HTTPRequest request = HTTPRequestCreate();
	
	// Start the parsing at the beginning of the request.
	char *parse = requestString;
	static const char * const delimiter = "\r\n";
	
	// Parse out the rest of the headers.
	for ( int i=0; i<HttpReqHeaderFieldsCount; ++i ) {
		
		char *next = strstr(parse, delimiter);
		
		char *line = substring(parse, next);
		
		// Advance the parse pointer to the end of the line, and after the delimiter.
		parse = next + strlen(delimiter);
		
		if ( line == NULL ) {
			break;
		}
		
		// The first line is different than the rest.
		if ( i == 0 ) {
			
			/* handle GET, HEAD, and POST request methods.  */
			
			char *Method = NULL;
			char *MethodArgs = NULL;
			if ( !splitStringAtString(line, " ", &Method, &MethodArgs) ) {
				// Invalid Request-Line.
				*error = 400;
				return false;
			}
			
			char *Request_URI = NULL;
			char *HTTP_Version = NULL;
			if ( !splitStringAtString(MethodArgs, " ", &Request_URI, &HTTP_Version) ) {
				// Invalid Request-Line.
				*error = 400;
				return false;
			}
			
			unsigned long valueLength = strlen(Method) + 1 + strlen(Request_URI) + 1 + strlen(HTTP_Version) + 1;
			char *value = malloc(sizeof(char) * valueLength);
			strcpy(value, Method);
			strcat(value, " ");
			strcat(value, Request_URI);
			strcat(value, " ");
			strcat(value, HTTP_Version);
			value[valueLength-1] = '\0';
			
			if ( stringEquality(Method, "GET") ) {
				request[HttpReqHeaderField_Request_Line] = value;
			} else if ( stringEquality(Method, "HEAD")) {
				request[HttpReqHeaderField_Request_Line] = value;
			} else if ( stringEquality(Method, "POST") ) {
				request[HttpReqHeaderField_Request_Line] = value;
			} else {
				/* refuse to process any HTTP request method other than GET, HEAD, and POST. */
				HTTPRequestFree(request);
				*error = 405;
				return NULL;
			}
			
		} else {
			// Split the line into field name and value.
			char *fieldName = NULL;
			char *fieldValue = NULL;
			if ( !splitStringAtString(line, ": ", &fieldName, &fieldValue) ) {
				continue;
			}
			
			HttpReqHeaderField field = HttpReqHeaderFieldForFieldNamed(fieldName);
			if ( (int) field == -1 ) {
				// Conversion failed.
				continue;
			}
			request[field] = fieldValue;
		}
	}
	
	// Check request to see if the request's valid.
	if ( !validateRequest(request) ) {
		HTTPRequestFree(request);
		*error = 400;
		return NULL;
	}
	
	return request;
}

bool shouldAllowRequest(HTTPRequest request)
{
	if ( request == NULL ) {
		return false;
	}
	
	return shouldAllowServer(request[HttpReqHeaderField_Host]);
}

bool shouldAllowServer(const char *server)
{
	//used for finding suffix matches
	const char *serverEnd = server + strlen(server);
	
	for ( int i=0; i<*filtersCount; ++i ) {
		const char *filter = filters[i];
		
		size_t filterLen = strlen(filter);
		
		// Check prefix.
		if ( strcaseCmp(server, filter, filterLen) == 0 ) {
			return false;
		}
		
		// Check suffix.
		if ( strcaseCmp(serverEnd - filterLen, filter, filterLen) == 0 ) {
			return false;
		}
		
	}
	
	return true;
}

bool sendHTTPStatusToSocket(int status, int client)
{
	// Find the maximum length of the status string.
	unsigned long maxLength =  8  // "HTTP/1.1"
						    +  1
							+  3  // 3-digit status code.
						    +  1
							+ 36  // Max length of status string
	                        +  5; // Double carriage return/newline + null-terminator
	
	char *status_str = malloc(sizeof(char) * maxLength);
	
	char *HTTP_Version = "HTTP/1.1";
	
	int sprinted = snprintf(status_str, maxLength, "%s %d %s\r\n\r\n", HTTP_Version, status, statusStringForStatusCode(status));
	
	
	// Send the status.
	ssize_t send_n = send(client, status_str, strlen(status_str), 0);
	free(status_str);
	
	if ( send_n < strlen(status_str) ) {
		perror("send()");
		return false;
	}
	
	return true;
}
