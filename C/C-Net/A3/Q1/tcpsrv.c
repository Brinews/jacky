

/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define PORT 5000


int main()
{
	int sock, connected, bytes_recieved , true = 1; 
	char send_data [1024] , recv_data[1024];      

	struct sockaddr_in server_addr,client_addr;   
	int sin_size;

	/* 1. get socket fd */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	/* set addr reuse */
	if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
		perror("Setsockopt");
		exit(1);
	}

	server_addr.sin_family = AF_INET;        
	server_addr.sin_port = htons(PORT);    
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8);

	/* 2. bind address */
	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
			== -1) {
		perror("Unable to bind");
		exit(1);
	}

	/* 3. listen on the socket:port */
	if (listen(sock, 5) == -1) {
		perror("Listen");
		exit(1);
	}

	printf("\nTCPServer Waiting for client on port 5000");
	fflush(stdout);

	while(1)
	{ 
		sin_size = sizeof(struct sockaddr_in);

		/* 4. accept client's connection */
		connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

		printf("\n I got a connection from (%s , %d)",
				inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

		/* interaction with client */
		while (1)
		{
			/* send data to client */
			printf("\n SEND (q or Q to quit) : ");
			fgets(send_data, sizeof(send_data), stdin);
			send_data[strlen(send_data)-1] = '\0';

			if (strcmp(send_data , "q") == 0 
					|| strcmp(send_data , "Q") == 0)
			{
				send(connected, send_data,strlen(send_data), 0);
				close(connected);
				break;
			}
			else
				send(connected, send_data,strlen(send_data), 0); 

			/* receive data from client */
			bytes_recieved = recv(connected,recv_data,1024,0);
			recv_data[bytes_recieved] = '\0';

			if (strcmp(recv_data , "q") == 0 
					|| strcmp(recv_data , "Q") == 0)
			{
				close(connected);
				break;
			}
			else
				printf("\n RECIEVED DATA = %s " , recv_data);

			fflush(stdout);
		}
	}      

	close(sock);

	return 0;
} 
