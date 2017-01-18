#include "consrv.h"

void DieWithSystemMessage(const char *msg) {
    perror(msg);
    exit(1);
}

int connectServer()
{
    const char *server = "127.0.0.1";
    in_port_t servPort =atoi("9090");

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

	return sock;
}
