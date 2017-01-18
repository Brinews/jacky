#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "json/json.h"
#include <iostream>

#define BUFLEN	512

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

int main(int argc, char *argv[]) 
{
    if (argc < 2 || argc > 3) // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)","<Server Name> <port num> ");

    char *server = argv[1];
    // First arg: server IP address (dotted quad)

    in_port_t servPort =atoi(argv[2]);

    // Create a reliable, stream socket using TCP
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
        DieWithSystemMessage("socket() failed");

    // Construct the server address structure
    struct sockaddr_in servAddr;
    // Zero out structure
    memset(&servAddr, 0, sizeof(servAddr)); 

    servAddr.sin_family = AF_INET;

    if ((servAddr.sin_addr.s_addr = inet_addr(server)) == (unsigned long)INADDR_NONE)
    {
         
        /* When passing the host name of the server as a */
        /* parameter to this program, use the gethostbyname() */
        /* function to retrieve the address of the host server. */
        /***************************************************/
        /* get host address */
        struct hostent *hostp = gethostbyname(server);
        
        if(hostp == (struct hostent *)NULL)
        {
            printf("HOST NOT FOUND");
            exit(-1);
        }

        memcpy((void *)&servAddr.sin_addr, (const void *)hostp->h_addr, sizeof(servAddr.sin_addr));
    }

    // Server port
    servAddr.sin_port = htons(servPort);

    // Establish the connection to the echo server
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("connect() failed");

    // Receive the same string back from the server
    unsigned int totalBytesRcvd = 0; // Count of total bytes received
    fputs("Client Send:\n", stdout);

	Json::StyledWriter style_writer;

    // Setup to print the echoed string
    while (1) {
        char buffer[BUFLEN]; // I/O buffer
		Json::Value val;

        fputs("Username:", stdout);
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, BUFLEN, stdin);
        buffer[strlen(buffer)-1] = '\0';
        val["name"] = buffer;

        fputs("Password:", stdout);
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, BUFLEN, stdin);
        buffer[strlen(buffer)-1] = '\0';
        val["pwd"] = buffer;

		val["type"] = "get";
        val["nickname"] = "";
        val["picname"] = "";

        std::string sendbuffer = val.toStyledString();

        printf("len=%d\n", sendbuffer.length());

        strcpy(buffer, sendbuffer.c_str());

        //printf("%s\n", buffer);

        send(sock, buffer, BUFLEN, 0);

        int numBytes = recv(sock, buffer, BUFLEN, 0);

        if (numBytes < 0)
            DieWithSystemMessage("recv() failed");
        else if (numBytes == 0)
            DieWithUserMessage("recv()", "connection closed prematurely");

        //buffer[numBytes-1] = '\0';
        fputs(buffer, stdout);
    }

    fputc('\n', stdout); // Print a final linefeed
    close(sock);

    exit(0);
}

