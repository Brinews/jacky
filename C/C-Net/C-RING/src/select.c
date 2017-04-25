/*
 * The following code will put the standard input and the socket descriptor
 * in the select function to be inspected for reading.  When the select
 * function returns, the descriptors are inspected to find out which one
 * is ready.   This code can be modified to handle files and sockets which
 * become ready for writing.
 */

#include <sys/time.h>

#include "header.h"
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#define SYN	'^'
#define DLE '#'
#define STX '@'
#define ETX '~'

char *GenerateData(int n)
{
	printf("%d\n", n);

	return "ACT";
}

int isToken(char *buf)
{
	char tokenbuf[10] = {0};
	tokenbuf[0] = SYN;
	tokenbuf[1] = SYN;
	tokenbuf[2] = DLE;
	tokenbuf[3] = ETX;
	tokenbuf[4] = DLE;
	tokenbuf[5] = ETX;

	if (strcmp(buf, tokenbuf) == 0) return 1;

	return 0;
}

void GenToken(int sig)
{
	GenerateData((int)random());

	/*
	signal(sig, SIG_IGN);
	printf("Want to quit?[y/n]");
	char c = getchar();
	if (c == 'Y' || c == 'y')
		exit(0);
	else
		signal(SIGINT, GenToken);

	getchar();
	*/
}

int main(int argc, char **argv)
{
	if (argc < 5) {
		printf("Usage: %s <ID> <localport> <nexthost> <nexthostport>", argv[0]);
		return 0;
	}

	char id = argv[1][0];
	int port = atoi(argv[2]);
	char *nexthost = argv[3];
	int nport = atoi(argv[4]);

	char dstId;
	int needSend = 0;
	char buffer[100];

	int flags;
	flags = fcntl(0, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(0, F_SETFL, flags);

	srandom(time(NULL));

	/* 0. action binding */
	if (signal(SIGINT, GenToken) == SIG_ERR) {
		perror("signal error");
	}

	/* 1. server listening */
	int sockfd = Server("localhost", port);
	int sd = -1;
	int clifd = -1;

	struct sockaddr_in otheraddr;
	int otherlength;

	/* 2. monitor token ring and user input */

	/* assume sd is the data channel socket & STDIN_FILENO is the
	 * standard input file descriptor (it is 0)
	 */

	int n;
	fd_set rset;		/* declare an fd_set for read descriptors */

	printf("Enter \'quit\' for exit.\n");

	for (;;) {	/* endless loop, if you want continuous operation */

		FD_ZERO(&rset);		/* clear all bits in rset */
		FD_SET(STDIN_FILENO, &rset);	/* set the standard input bit */

		if (sd != -1) {
			FD_SET(sd, &rset);	/* set the socket descriptor bit */
			n = select((sd>STDIN_FILENO? sd:STDIN_FILENO)+1, &rset, NULL, NULL, NULL);

		} else {
			FD_SET(sockfd, &rset);
			n = select((sockfd>STDIN_FILENO? sockfd:STDIN_FILENO)+1, &rset, NULL, NULL, NULL);
		}


		/* select blocks, and n is the number of ready descriptors */
		if ( (n == -1 ) && (errno == EINTR) ) /* interruption */
		   continue;
	  	if (n== -1) {	/* error: you may handle it, if you want */
		 	/* code to handle errors */
			ReportError("Unknown error");
	  	}

		/* after this point, handle the ready descriptor(s) */

	  	/* check for ready data from the keyboard */

	  	if (FD_ISSET(STDIN_FILENO, &rset)) {
			/* read data from the standard input*/
			memset(linebuffer, 0, sizeof(linebuffer));
			fgets(linebuffer, 80, stdin);
			linebuffer[strlen(linebuffer)-1] = '\0';

			if (strlen(linebuffer) == 1) {
				if (linebuffer[0] == '\020') { /* CTRL+P */
					printf("Ctrl+P is entered, send the token out.\n");
					if (clifd == -1) {
						clifd = Client(nexthost, nport);
						//printf("connect client:%d\n", clifd);
					}

					char tokenbuf[10] = {0};

					tokenbuf[0] = SYN;
					tokenbuf[1] = SYN;
					tokenbuf[2] = DLE;
					tokenbuf[3] = ETX;
					tokenbuf[4] = DLE;
					tokenbuf[5] = ETX;

					if (send(clifd, tokenbuf, strlen(tokenbuf), 0) < 0) { // send token to next
						perror("Send next error");
					}

				} else {
					dstId = linebuffer[0];
				}
			} else if (strcmp(linebuffer, "quit") == 0) {
				exit(0);
			} else {
				/* send message to next host */
				if (clifd == -1) {
					clifd = Client(nexthost, nport);

					//printf("connect client:%d\n", clifd);
				}

				//char buffer[100];
				memset(buffer, 0, sizeof(buffer));

				buffer[0] = SYN;
				buffer[1] = SYN;
				buffer[2] = DLE;
				buffer[3] = STX;

				buffer[4] = id;
				buffer[5] = dstId;

				strcpy(buffer+6, linebuffer);

				int ilen = strlen(buffer);
				buffer[ilen] = DLE;
				buffer[ilen+1] = ETX;
				buffer[ilen+2] = '\0';

				needSend = 1;
				/*
				if (send(clifd, buffer, strlen(buffer), 0) < 0) {
					perror("send error\n");
				}
				*/
			}
			n--;
	  	}
	 
	  	/* check for ready data from the communication channel */

	  	if (n > 0 && sd > 0 && (FD_ISSET(sd, &rset)) ) {
			/* socket is ready for reading */
			/* read data from socket */

			memset(linebuffer, 0, sizeof(linebuffer));
			int len = read(sd, linebuffer, 100);
			if (len < 0) {
				perror("Error on read");
			}

			if (clifd < 0) {
				clifd = Client(nexthost, nport);
			}

			if (isToken(linebuffer)) { /* recv a token */
				if (needSend) {
					printf("Got token, begin send message.\n");

					if (send(clifd, buffer, strlen(buffer), 0) < 0) {
						perror("Send next error");
					}

					needSend  = 0; /* send over */
				} else {
					/* send token to next */
					if (send(clifd, linebuffer, strlen(linebuffer), 0) < 0) {
						perror("Send next error");
					}
				}

			} else {

				/* check the packet */
				if (len > 5 && linebuffer[5] == id) {
					/* for me */

					linebuffer[strlen(linebuffer)-2] = '\0';

					if (strlen(linebuffer+6) == 0) {
						printf("RECV: empty message.\n");
					} else {
						printf("RECV:%s\n", linebuffer+6);
					}


					char tokenbuf[10] = {0};
					tokenbuf[0] = SYN;
					tokenbuf[1] = SYN;
					tokenbuf[2] = DLE;
					tokenbuf[3] = ETX;
					tokenbuf[4] = DLE;
					tokenbuf[5] = ETX;

					if (send(clifd, tokenbuf, strlen(tokenbuf), 0) < 0) { // send token to next
						perror("Send next error");
					}

				} else { /* message not for me */

					if (send(clifd, linebuffer, strlen(linebuffer), 0) < 0) {
						perror("Send next error");
					}
				}
			}
		}

		if (n > 0 && sd < 0 && (FD_ISSET(sockfd, &rset))) {
			/* accept connect */
			otherlength = sizeof(otheraddr);
			sd = accept(sockfd, (struct sockaddr *)&otheraddr, &otherlength);
			//printf("Accept: %d\n", sd);

			if (sd < 0) {
				perror("Error on accept");
			}

			memset(linebuffer, 0, sizeof(linebuffer));
			int len = read(sd, linebuffer, 100);
			if (len < 0) {
				perror("Error on read");
			}

			/* check the packet */
			if (linebuffer[1] == id) {
				/* for me */
				printf("RECV:%s\n", linebuffer+2);
			}

			if (linebuffer[0] != id) {
				/* continue send to next */
				if (clifd < 0) {
					clifd = Client(nexthost, nport);
				}

				if (clifd < 0) {
					perror("Connect next error\n");
				}

				if (send(clifd, linebuffer, strlen(linebuffer), 0) < 0) {
					perror("Send next error");
				}
			}

		}
	}


	return 0;
}

