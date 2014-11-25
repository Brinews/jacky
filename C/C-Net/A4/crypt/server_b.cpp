#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <poll.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "rsa.h"

#define PORTNUM		59000

/* global vars */
rsa_key pubkey, prikey;
sym_key skey;	/* key from A */

void handler(int sig);

void accept_loop(int listenfd);
void request_handle(int clientfd);

void handler(int sig)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
}

void accept_loop(int listenfd)
{
	struct sockaddr_in client;
	socklen_t len = sizeof(client);

	pid_t pid; /* multi-process server */

	bzero(&client, len);

	while (1) {
		int clientfd = accept(listenfd, (struct sockaddr*)&client, &len);

		if (clientfd < 0) {
			perror("accept\n");
			return;
		}

		printf("Accept connection from host ip: %s\n", 
				inet_ntoa(client.sin_addr));

		pid = fork();

		if (pid < 0) { perror("fork error\n"); return; }
		else if (pid == 0) { /* child process */
			close(listenfd);

			request_handle(clientfd); /* handler */
			exit(0);
		} else {
			close(clientfd);
		}
	}
}

void request_handle(int clientfd)
{
	int MAXLEN = 1024, recvLen = 0;
	char recvbuf[MAXLEN];

	memset(recvbuf, 0, MAXLEN);

	/* 2. send pub-key to client */
	sprintf(recvbuf, "%d %d", pubkey.rsa_prime, pubkey.rsa_product);
	send(clientfd, recvbuf, strlen(recvbuf), 0);

	/* 3. recv sym-key from client */
	recv(clientfd, recvbuf, MAXLEN, 0);
	sscanf(recvbuf, "%d%d%d", 
			&(skey.sym_op1), &(skey.sym_op2), &(skey.sym_op3));

	if (debug) {
		printf("Receive Key From A:%d,%d,%d\n",
				skey.sym_op1, skey.sym_op2, skey.sym_op3);
	}

	/* 4. decrypt sym-key */
	sym_key nkey = sym_decrypt_key(&skey, &prikey);

	if (debug) {
		printf("Key After Decrypt:%d,%d,%d\n",
				nkey.sym_op1, nkey.sym_op2, nkey.sym_op3);
	}

	printf("\nBegin Receive Data From A:\n");

	while (1) {
		memset(recvbuf, 0, MAXLEN);
		int ret = recv(clientfd, recvbuf, MAXLEN, 0);

		if (ret <= 0) break;

		printf("Received DATA:%s\n", recvbuf);

		char *pbuf = easy_decrypt(recvbuf, &nkey);
		printf("Original DATA:%s\n", pbuf);
	}

	/*
	while (1) {

		int ret = recv(clientfd, (char *)recvbuf+recvLen, MAXLEN-recvLen, 0);

		if (ret <= 0){
			break;
		}

		recvLen = recvLen + ret;
		if (recvLen < MAXLEN) continue;
		else {
			recvbuf[MAXLEN] = '\0';
			printf("[8 chars]=%s\n", recvbuf);
			recvLen = 0;
		}
	}
	*/
}

int main(int argc, char **argv)
{
	int opt = 1;
	int sockListen;

	signal(SIGCHLD, handler);

	struct sockaddr_in server_addr;

	/* socket fd */
	if ((sockListen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket error\n");
		return -1;
	}

	if (setsockopt(sockListen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1) {
		perror("setsockopt");
		return -1;
	}

	/* bind */
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORTNUM);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockListen, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("bind error\n");
		return -1;
	}

	/* listen */
	if (listen(sockListen, 5) < 0) {
		perror("listen");
		return -1;
	}

	/* 1. yeild keys */
	rsa_yield_keys(&pubkey, &prikey);

	if (debug) {
		printf("pubkey:%d,%d\n", pubkey.rsa_prime, pubkey.rsa_product);
		printf("prikey:%d,%d\n", prikey.rsa_prime, prikey.rsa_product);
	}

	accept_loop(sockListen);
	close(sockListen);

	printf("server end.\n");

	return 0;
}

