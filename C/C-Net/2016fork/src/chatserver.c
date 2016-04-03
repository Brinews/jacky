/* 
 * chatserver.c - A simple UDP chat server 
 * usage: chatserver <port>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

struct client_info {
    char cli_name[32];
    char cli_hostname[64];
    char cli_port[8];
    int  cli_status; /* 0: not talking, 1:talking */

    struct client_info *next;
};

struct client_info head = {"", "", "", 0, NULL};

void simq_add_client(struct client_info *head, char *name, char *hostname,
        char *port)
{
    struct client_info *node = (struct client_info *) malloc(sizeof(*node));

    strcpy(node->cli_name, name);
    strcpy(node->cli_hostname, hostname);
    strcpy(node->cli_port, port);

    node->cli_status = 0;

    node->next = head->next;
    head->next = node;
}

struct client_info *simq_query_client(struct client_info *head, char *name,
        struct client_info **ahead)
{
    struct client_info *ptr = head->next;
    *ahead = head;

    while (ptr != NULL) {
        //printf("ptr_name=%s, name=%s\n", ptr->cli_name, name);

        if (strcmp(ptr->cli_name, name) == 0) return ptr;
        
        *ahead = ptr;
        ptr = ptr->next;
    }

    return NULL;
}

void simq_remove_client(struct client_info *head, char *name)
{
    struct client_info *ahead;

    struct client_info *qry = simq_query_client(head, name, &ahead);
    if (qry != NULL) {
        /* remove node */
        ahead->next = qry->next;

        free(qry);
    }
}

void simq_update_client(struct client_info *head, char *name, int status)
{
    struct client_info *ahead;

    struct client_info *qry = simq_query_client(head, name, &ahead);
    if (qry != NULL) {
        qry->cli_status = status;
    }
}


/*
 * error - wrapper for perror
 */
void error(char *msg) {
  perror(msg);
  exit(1);
}

int parse_message(char *buf, char **value)
{
    int i, ret = -1;
    char *cmd;

    cmd = buf;
    for (i = 0; i < strlen(buf); i++) {
        if (buf[i] == ' ') {
            buf[i] = '\0';
            *value = buf+i+1;
            break;
        }
    }

    if (strcasecmp(cmd, "login") == 0) ret = 0;
    if (strcasecmp(cmd, "who") == 0) ret = 1;
    if (strcasecmp(cmd, "talk") == 0) ret = 2;
    if (strcasecmp(cmd, "quit") == 0) ret = 3;

    return ret;
}

int main(int argc, char **argv) {

  int sockfd; /* socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; /* message buf */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */

  /* 
   * check command line arguments 
   */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  /* 
   * socket: create the parent socket 
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");

  /* setsockopt: Handy debugging trick that lets 
   * us rerun the server immediately after we kill it; 
   * otherwise we have to wait about 20 secs. 
   * Eliminates "ERROR on binding: Address already in use" error. 
   */
  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  /*
   * build the server's Internet address
   */
  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  /* 
   * bind: associate the parent socket with a port 
   */
  if (bind(sockfd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  /* 
   * main loop: wait for a datagram, then echo it
   */
  clientlen = sizeof(clientaddr);
  while (1) {

    /*
     * recvfrom: receive a UDP datagram from a client
     */
    bzero(buf, BUFSIZE);

    n = recvfrom(sockfd, buf, BUFSIZE, 0,
		 (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0)
      error("ERROR in recvfrom");

    char *value;

    printf("buf=%s\n", buf);
    int type = parse_message(buf, &value);
    printf("value=%s\n", value);

    struct client_info *ahead;

    switch (type) {
        case 0: /*login username port */
            printf("In Login\n");
            /* 
             * gethostbyaddr: determine who sent the datagram
             */
            hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
                    sizeof(clientaddr.sin_addr.s_addr), AF_INET);
            if (hostp == NULL)
                error("ERROR on gethostbyaddr");
            hostaddrp = inet_ntoa(clientaddr.sin_addr);
            if (hostaddrp == NULL)
                error("ERROR on inet_ntoa\n");
            printf("server received datagram from %s (%s)\n", 
                    hostp->h_name, hostaddrp);
            printf("server received %d/%d bytes: %s\n", strlen(buf), n, buf);

            /* get port */
            char *port = value;
            while (*port != '\0' && *port != ' ') port++;

            if (*port == ' ') *port = '\0', port++;

            //printf("Name=%s,Port=%s\n", value, port);

            if (simq_query_client(&head, value, &ahead) == NULL) {
                simq_add_client(&head, value, hostp->h_name, port);
                bzero(buf, BUFSIZE);
                sprintf(buf, "%s", "LOGIN OK");
            } else {
                bzero(buf, BUFSIZE);
                sprintf(buf, "%s", "ALREADY LOGGED");
            }

            sendto(sockfd, buf, BUFSIZE, 0,
                    (struct sockaddr *)&clientaddr, clientlen);

            break;
        case 1: /* query list: who username */
            printf("In Query");
            if (simq_query_client(&head, value, &ahead) != NULL) {
                struct client_info *ptr = head.next;

                char sndbuf[BUFSIZE];

                while (ptr != NULL) {
                    if (strcmp(ptr->cli_name, value) == 0){
                        ptr = ptr->next;
                        continue;
                    }

                    if (ptr->cli_status == 1) {
                        ptr = ptr->next;
                        continue;
                    }

                    bzero(sndbuf, BUFSIZE);

                    sprintf(sndbuf, "%s %s %s", ptr->cli_name, 
                            ptr->cli_hostname, ptr->cli_port);
                    sendto(sockfd, sndbuf, BUFSIZE, 0,
                            (struct sockaddr *)&clientaddr, clientlen);

                    ptr = ptr->next;
                }

                bzero(buf, BUFSIZE);

                sprintf(buf, "%s", "LISTEND");
                sendto(sockfd, buf, BUFSIZE, 0,
                        (struct sockaddr *)&clientaddr, clientlen);
            }
            break;
        case 2: /* init a talk: talk username TOusername */
            printf("In Talk\n");
            char *touser = value;
            while (*touser != '\0' && *touser != ' ') touser++;

            if (*touser == ' ') *touser = '\0', touser++;

            if (simq_query_client(&head, value, &ahead) != NULL) {
                struct client_info *ptr = simq_query_client(&head, 
                        touser, &ahead);

                bzero(buf, BUFSIZE);
                if (ptr != NULL) {
                    sprintf(buf, "%s %s %s", ptr->cli_name,
                            ptr->cli_hostname, ptr->cli_port);

                } else {
                    sprintf(buf, "%s", "REJECT");
                }

                sendto(sockfd, buf, BUFSIZE, 0,
                        (struct sockaddr *)&clientaddr, clientlen);
            }

            break;
        case 3: /* quit: quit username */
            printf("In Quit\n");

            struct client_info *ptr = simq_query_client(&head, value, &ahead);

            if (ptr != NULL) {
                hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
                        sizeof(clientaddr.sin_addr.s_addr), AF_INET);

                if (strcmp(ptr->cli_hostname, hostp->h_name) == 0) {
                    simq_remove_client(&head, value);
                }
            }
            break;
        default:
            bzero(buf, BUFSIZE);
            strcpy(buf, "INVALID COMMAND");
            sendto(sockfd, buf, BUFSIZE, 0,
                    (struct sockaddr *)&clientaddr, clientlen);
            break;
    }
  }
}
