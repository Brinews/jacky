

/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define PORT 5000

int main(int argc, char **argv)
{
	int sock, bytes_recieved; 
	char send_data[1024],recv_data[1024];
	struct hostent *host;
	struct sockaddr_in server_addr; 

	if (argc < 2) {
		printf("Usage: %s <server ip>", argv[0]);
		return -1;
	}

	host = gethostbyname(argv[1]);

	/* 1. get socket fd */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;    
	server_addr.sin_port = htons(PORT);  
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);

	/* 2. do connect */
	if (connect(sock, (struct sockaddr *)&server_addr,
				sizeof(struct sockaddr)) == -1)
	{
		perror("Connect");
		exit(1);
	}

	/* 3. interaction with server */
	while(1)
	{
		/* first receive from server */
		bytes_recieved=recv(sock,recv_data,1024,0);
		recv_data[bytes_recieved] = '\0';

		if (strcmp(recv_data , "q") == 0 || 
				strcmp(recv_data , "Q") == 0)
		{
			close(sock);
			break;
		} else
			printf("\nRecieved data = %s " , recv_data);

		/* second send to server */
		printf("\nSEND (q or Q to quit) : ");
		fgets(send_data, sizeof(send_data), stdin);
		send_data[strlen(send_data)-1] = '\0';

		if (strcmp(send_data , "q") != 0 && 
				strcmp(send_data , "Q") != 0)
			send(sock,send_data,strlen(send_data), 0);
		else {
			send(sock,send_data,strlen(send_data), 0);  
			close(sock);
			break;
		}

	}  

	return 0;
}
