#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#include <unistd.h>

#include <arpa/inet.h>

#define PORTNUM	59000

#include "rsa.h"

rsa_key pubkey; /* key from B */
sym_key mykey;	/* local key */

int connect_server(char *ipaddr)
{
	int sockfd = -1;

	/* 0. get a socket fd */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket error\n");
		return -1;
	}

	/* 1. init sockadd_in */
	struct sockaddr_in server;

	server.sin_family = AF_INET;
	server.sin_port = htons(PORTNUM);

	server.sin_addr.s_addr = inet_addr(ipaddr);

	if (server.sin_addr.s_addr == INADDR_NONE) {
		struct hostent *host = gethostbyname(ipaddr);
		server.sin_addr = *((struct in_addr*)host->h_addr);
	}

	bzero(&(server.sin_zero), 8);

	/* 2. do connect */
	if (connect(sockfd, (struct sockaddr *)&server, 
				sizeof(struct sockaddr)) == -1) {
		perror("connect\n");

		return -1;
	}

	return sockfd;
}

void make_request(int clientfd)
{
	int MAXLEN = 1024;
	char buf[MAXLEN];

	/* 2. get pub-key */
	recv(clientfd, buf, MAXLEN, 0);
	sscanf(buf, "%d%d", &(pubkey.rsa_prime), &(pubkey.rsa_product));

	if (debug) {
		printf("Receive Pubkey from B:%d,%d\n",
				pubkey.rsa_prime, pubkey.rsa_product);
	}

	/* 3. encrypt my-key and send to server B */
	sym_key skey = sym_encrypt_key(&mykey, &pubkey);

	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%d %d %d", skey.sym_op1, skey.sym_op2, skey.sym_op3);
	send(clientfd, buf, strlen(buf), 0);

	if (debug) {
		printf("Send Key To B:%d,%d,%d\n", 
				skey.sym_op1, skey.sym_op2, skey.sym_op3);
	}

	if (debug) {
		printf("Please Input to-send Data('quit' for exit):\n");
	}

	while (1) {
		/* read data from command line, and send it to B */
		fgets(buf, sizeof(buf), stdin);
		buf[strlen(buf)-1] = '\0';

		if (strcmp("quit", buf) == 0) break;

		char *pbuf = easy_encrypt(buf, &mykey);

		if (debug) {
			printf("After Encrypted:%s\n", pbuf);
		}

		send(clientfd, buf, strlen(buf), 0);
	}
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		printf("usage: %s <server ip>\n", argv[0]);
		return -1;
	}

	/* 1. yield sym key: some digit */
	mykey.sym_op1 = 11;
	mykey.sym_op2 = 3;
	mykey.sym_op3 = 23;
	if (debug) {
		printf("Yield My Key:%d,%d,%d\n",
				mykey.sym_op1, mykey.sym_op2, mykey.sym_op3);
	}

	/* connect to sever */
	int sockfd = connect_server(argv[1]);

	if (sockfd < 0) return -1;

	make_request(sockfd);

	close(sockfd);

	return 0;
}
