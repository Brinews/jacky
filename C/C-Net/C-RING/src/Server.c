/*****************************************************************************/
/*                                                                           */
/* This routine establishes a passive open connection.  That is, it creates  */
/* a socket, and passively wait for a connection.  Once a connection request */
/* has been received, it echoes "connected" on the screen, and return        */
/* a file descriptor to be used to communicate with the remote machine.      */
/* Make sure that you change the machine name from "vulcan" to that you    */
/* will be running your process on. Also, change the port number to          */
/* a suitable port number as indicated in the project writeup.               */
/*                                                                           */
/*****************************************************************************/

#include "header.h"

int Server(char *host, int port)
{
	struct sockaddr_in myaddr, otheraddr;
	struct hostent *myname;

	int s, fd; /*, otherlength; */
	FILE *fdopen(), *fp;
	char *hostname = host, ch;
	int hostnamelength;

	pid_t pid;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) ReportError ("socket");
	myname = gethostbyname(hostname);

	int optval = 1;
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
			(const void *)&optval, sizeof(int));

	bzero(&myaddr, sizeof(myaddr));
	myaddr.sin_family  = AF_INET;
	myaddr.sin_port = htons(port);
	bcopy(myname->h_addr_list[0], &myaddr.sin_addr, myname->h_length);
	bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr));

	listen(s, 5);

	/*
	otherlength = sizeof(otheraddr);
	fd = accept(s, &otheraddr, &otherlength);

	fprintf(stdout, "Connected");

	return(fd);
	*/

	return s; /* return listener socket */
}
