#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "json/json.h"
#include <iostream>

void DieWithUserMessage(const char *msg, const char *detail) {
    fputs(msg, stderr);
    fputs(": ", stderr);
    fputs(detail, stderr);
    fputc('\n', stderr);
    exit(1);
}

void DieWithSystemMessage(const char *msg) {
    perror(msg);
    exit(1);
}

void HandleTCPClient(int);//function: Receive and Send msg with Client

static const int MAXPENDING = 5; // Maximum outstanding connection requests


int main(int argc, char *argv[]) 
{
    if (argc != 2) // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<port num>");

    in_port_t servPort = atoi(argv[1]); // Set local port to be 5000

    // Create socket for incoming connections
    int servSock; // Socket descriptor for server
    if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithSystemMessage("socket() failed");
    // Construct local address structure
    struct sockaddr_in servAddr;          //Local address
    memset(&servAddr, 0, sizeof(servAddr));       //Zero out structure
    servAddr.sin_family = AF_INET;            //IPv4 address family
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //Any incoming interface
    servAddr.sin_port = htons(servPort);      //Local port

    // Bind to the local address
    if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("bind() failed");
    // Mark the socket so it will listen for incoming connections
    if (listen(servSock, MAXPENDING) < 0)
        DieWithSystemMessage("listen() failed");

    for (;;) { // Run forever
        struct sockaddr_in clntAddr; // Client address
        // Set length of client address structure (in-out parameter)
        socklen_t clntAddrLen = sizeof(clntAddr);
        // Wait for a client to connect
        int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
        if (clntSock < 0)
            DieWithSystemMessage("accept() failed");
        // clntSock is connected to a client!
        char clntName[INET_ADDRSTRLEN]; // String to contain client address
        if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
            printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
        else
            puts("Unable to get client address");

        HandleTCPClient(clntSock);
        close(clntSock);
    }
}


void HandleTCPClient(int clntSocket) {
    char buffer[BUFSIZ]; // Buffer for echo string

    // Receive message from client
    ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZ, 0);
    if (numBytesRcvd < 0)
        DieWithSystemMessage("recv() failed");

    // Send received string and receive again until end of stream
    while (numBytesRcvd > 0) { // 0 indicates end of stream

		printf("Receive from client:%d, %s\n", numBytesRcvd, buffer);

		Json::Value val;
		Json::Reader reader;

		if (reader.parse(buffer, val)) {
			std::string name = val["name"].asString();
			std::cout << name << std::endl;
		}

        // Echo message back to client
        ssize_t numBytesSent = send(clntSocket, buffer, BUFSIZ-1, 0);
        if (numBytesSent < 0)
            DieWithSystemMessage("send() failed");
        else if (numBytesSent != numBytesRcvd)
            DieWithUserMessage("send()", "sent unexpected number of bytes");
        // See if there is more data to receive
        numBytesRcvd = recv(clntSocket, buffer, BUFSIZ, 0);
        if (numBytesRcvd < 0)
            DieWithSystemMessage("recv() failed");
    }
}

