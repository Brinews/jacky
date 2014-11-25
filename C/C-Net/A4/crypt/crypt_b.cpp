#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <poll.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORTNUM		59000
#define MAXEVENT	500

int epollfd;
struct epoll_event	eventList[MAXEVENT];

int epoll_init(int listenfd);
void epoll_loop(int listenfd);

void accept_connection(int listenfd);
void request_handle(int clientfd);

int epoll_init(int sockListen)
{
	/*1. epoll init */
	epollfd = epoll_create(MAXEVENT);
	struct epoll_event event;

	event.events = EPOLLIN|EPOLLET;
	event.data.fd = sockListen;

	/* 2. epoll ctrol, add listen socket */
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockListen, &event) < 0) {
		perror("epoll_ctl");
		return -1;
	}

	return 0;
}

void accept_connection(int listenfd)
{
	struct sockaddr_in client;
	socklen_t len = sizeof(client);

	bzero(&client, len);

	int clientfd = accept(listenfd, (struct sockaddr*)&client, &len);

	if (clientfd < 0) {
		perror("accept\n");
		return;
	}
	else {
		printf("Accept connection :%d\n", clientfd);
	}

	/* 4. add client fd into epoll list */
	struct epoll_event event;
	event.data.fd = clientfd;
	event.events = EPOLLIN|EPOLLET|EPOLLOUT;

	epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &event);
}

void request_handle(int clientfd)
{
	int MAXLEN = 8, recvLen = 0;
	char recvbuf[MAXLEN+1];

	memset(recvbuf, 0, MAXLEN);

	printf("Client Recv:\n");
	while (1) {
		int ret = recv(clientfd, (char *)recvbuf+recvLen, MAXLEN-recvLen, 0);

		if (ret <= 0){
			recvLen = 0;
			break;
		}

		/* data recv into buffer */
		recvLen = recvLen + ret;
		if (recvLen < MAXLEN) continue;
		else {
			recvbuf[MAXLEN] = '\0';
			printf("[8 chars]=%s\n", recvbuf);
			recvLen = 0;
			break;
		}
	}
}

void epoll_loop(int sockListen)
{
	int timeout = 3000; /* wait infinity */

	while (1) {
		int ret = epoll_wait(epollfd, eventList, MAXEVENT, timeout);

		if (ret < 0) {
			perror("epoll wait error\n");
			break;
		} else if (ret == 0) {
			perror("timeout..\n");
			continue;
		}

		int i;
		for (i = 0; i < ret; i++) {
			/* exit when error */
			if ((eventList[i].events & EPOLLERR) ||
					(eventList[i].events & EPOLLHUP) ||
					!((eventList[i].events & EPOLLIN) ||
					 (eventList[i].events & EPOLLOUT))) {
				perror("epoll error\n");
				close(eventList[i].data.fd);
				return;
			}

			if (eventList[i].data.fd == sockListen) {
				accept_connection(sockListen);
			}
			else {
				request_handle(eventList[i].data.fd);
			}
		}
	}
}

int main(int argc, char **argv)
{
	int opt = 1;
	int sockListen;

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

	/* epoll server loop */
	if (epoll_init(sockListen) < 0) {
		return -1;
	}

	epoll_loop(sockListen);

	close(epollfd);
	close(sockListen);

	printf("server end.\n");

	return 0;
}

