/* 
 * chatclient.c - A simple chat client
 * usage: chatclient <host> <tcpport> <udpport>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <signal.h>


#define BUFSIZE 1024

/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

char *vcmd[] = { "login", "who", "talk", "quit" };

int valid_command(char *cmd)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (strcasecmp(cmd, vcmd[i]) == 0) return i;
    }

    return -1;
}

void tcp_client(char *host, char *port)
{
    int sockfd, portno, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char hostname[64] = {0};
    char buf[BUFSIZE];

    /* check command line arguments */
    strcpy(hostname, host);
    portno = atoi(port);

    printf("To Connect Talk Server:%s,%d\n", host, portno);

    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    /* connect: create a connection with the server */
    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) 
      error("ERROR connecting");

    printf("Enter message: ");

    bzero(buf, BUFSIZE);

    while (fgets(buf, BUFSIZE, stdin) != NULL) {
        /* get message line from the user */

        if (strcasecmp(buf, "exit\n") == 0) break;

        /* send the message line to the server */
        n = write(sockfd, buf, strlen(buf));
        if (n < 0) 
            error("ERROR writing to socket");

        /* print the server's reply */
        bzero(buf, BUFSIZE);
        n = read(sockfd, buf, BUFSIZE);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("Echo from server: %s", buf);

        bzero(buf, BUFSIZE);

        printf("Enter Message:");
    }

    close(sockfd);
}

/*
 * chat server
 */
void tcp_server(char *port)
{
  int parentfd; /* parent socket */
  int childfd; /* child socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buffer */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */

  /* 
   * check command line arguments 
   */
  portno = atoi(port);

  /* 
   * socket: create the parent socket 
   */
  parentfd = socket(AF_INET, SOCK_STREAM, 0);
  if (parentfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));

  /* this is an Internet address */
  serveraddr.sin_family = AF_INET;

  /* let the system figure out our IP address */
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* this is the port we will listen on */
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(parentfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  /* 
   * listen: make this socket ready to accept connection requests 
   */
  if (listen(parentfd, 5) < 0) /* allow 5 requests to queue up */ 
    error("ERROR on listen");

  /* 
   * main loop: wait for a connection request, echo input line, 
   * then close connection.
   */
  clientlen = sizeof(clientaddr);
  while (1) {

    /* 
     * accept: wait for a connection request 
     */
    childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
    if (childfd < 0) 
      error("ERROR on accept");
    
    /* 
     * gethostbyaddr: determine who sent the message 
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    printf("server established connection with %s (%s)\n", 
	   hostp->h_name, hostaddrp);
    
    while (1) {
        /* 
         * read: read input string from the client
         */
        bzero(buf, BUFSIZE);
        n = read(childfd, buf, BUFSIZE);
        if (n < 0) 
            error("ERROR reading from socket");
        printf("server received %d bytes: %s", n, buf);

        if (strcasecmp(buf, "exit") == 0) break;
        if (n == 0) break;

        //fgets(buf, BUFSIZE, stdin);

        /* 
         * write: echo the input string back to the client 
         */
        n = write(childfd, buf, strlen(buf));
        if (n < 0) 
            error("ERROR writing to socket");
    }

    close(childfd);
  }
}

int main(int argc, char **argv) {
    pid_t childpid;

    /* check command line arguments */
    if (argc != 4) {
       fprintf(stderr,"usage: %s <hostname> <udpport> <tcpport>\n", argv[0]);
       exit(0);
    }

    if (!(childpid = fork())) {
        printf("Start talking listen server... Completed!\n");
        tcp_server(argv[3]);
    } else {

        int sockfd, portno, n;
        int serverlen;
        struct sockaddr_in serveraddr;
        struct hostent *server;
        char *hostname;

        char buf[BUFSIZE];
        char cmd[BUFSIZE];

        hostname = argv[1];
        portno = atoi(argv[2]);

        /* socket: create the socket */
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) 
            error("ERROR opening socket");

        /* gethostbyname: get the server's DNS entry */
        server = gethostbyname(hostname);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host as %s\n", hostname);
            exit(0);
        }

        /* build the server's Internet address */
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, 
                (char *)&serveraddr.sin_addr.s_addr, server->h_length);
        serveraddr.sin_port = htons(portno);

        printf("Welcome to use simQ system...\n");
        printf("Commands:\n\tlogin <yourname> <tcpport>\n");
        printf("\twho <yourname>\n");
        printf("\ttalk <yourname> <to-talk-user-name>\n");
        printf("\tquit <yourname>\n");
        printf("\n");

        while (1) {
            /* get a message from the user */
            bzero(buf, BUFSIZE);
            printf(">>");
            fgets(buf, BUFSIZE, stdin);
            buf[strlen(buf)-1] = '\0';

            bzero(cmd, BUFSIZE);
            char *ptr = buf;
            int i = 0;
            while (*ptr != '\0' && *ptr != ' ') {
                cmd[i] = buf[i];
                i++;
                ptr++;
            }
            cmd[i] = '\0';

            int ret;
            if ((ret = valid_command(cmd)) < 0) continue;

            /* send the message to the server */
            serverlen = sizeof(serveraddr);
            n = sendto(sockfd, buf, BUFSIZE, 0, 
                    (struct sockaddr *)&serveraddr, serverlen);
            if (n < 0) 
                error("ERROR in sendto");

            bzero(buf, BUFSIZE);

            if (ret == 0) { /* login */
                /* print the server's reply */
                n = recvfrom(sockfd, buf, BUFSIZE, 0, 
                        (struct sockaddr *)&serveraddr, &serverlen);
                if (n < 0) 
                    error("ERROR in recvfrom");

                printf("%s\n", buf);

            } else if (ret == 1) { /* who */
                int count = 0;
                bzero(buf, BUFSIZE);
                while ((n = recvfrom(sockfd, buf, BUFSIZE, 0,
                                (struct sockaddr *)&serveraddr, &serverlen)) > 0) {
                    if (strcmp(buf, "LISTEND") == 0) break;

                    count++;
                    printf("%s\n", buf);
                    bzero(buf, BUFSIZE);
                }

                if (count == 0) {
                    printf("No users online, or self not login.\n");
                }

            } else if (ret == 2) { /* talk */

                n = recvfrom(sockfd, buf, BUFSIZE, 0, 
                        (struct sockaddr *)&serveraddr, &serverlen);
                if (n < 0) 
                    error("ERROR in recvfrom");

                if (strcmp(buf, "REJECT") == 0) {
                    printf("%s", buf);
                } else {
                    printf("Begin to connect remote user...\n");
                    char username[32];
                    char hostname[64];
                    char port[8];

                    printf("buf=%s\n", buf);
                    sscanf(buf, "%s%s%s", username, hostname, port);
                    /* make a connection with user */

                    pid_t cpid;
                    if (!(cpid = fork())) {
                        /* child process */
                        printf("Start talking session process...\n");
                        tcp_client(hostname, port);
                    }

                    waitpid(cpid, 0, 0);
                }

            } else if (ret == 3) { /* quit */
                kill(childpid, SIGKILL);
                break;
            }
        }
    }

    return 0;
}
