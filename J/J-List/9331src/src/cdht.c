/*
 * Circular DHT, 2015-05-23
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define SUCCESS 0
#define ERROR   1

#define TCPPORT	50000
#define UDPPORT 55000

#define BUFLEN	1024
#define END_LINE 0x0

#define MINID -1

#define TARGETHOST "127.0.0.1"

int selfId, firstId, nextId, prevId = MINID, sprevId = MINID;
int tcpPort, udpPort;

int udpfd1, udpfd2;
struct sockaddr_in srvAddr1, srvAddr2;

int nfd; /* share file desc */

typedef struct IdItem {
    int fId;
    int pId;
} IdItem;

/*
 * @param peerId peer node id
 * @return int TCP port
 */
int peerTcpPort(int peerId)
{
    return TCPPORT + peerId;
}

/*
 * @param peerId peer node id
 * @return int UDP port
 */
int peerUdpPort(int peerId)
{
    return UDPPORT + peerId;
}

/*
 * @param sockfd server socket
 * @param srvAddr server address pointer
 * @return int -1 for error
 */
int sendPingRequest(int sockfd, struct sockaddr_in *srvAddr)
{
    if (sockfd > 0) {
        char buf[BUFLEN];

        sprintf(buf, "ping %d", selfId);
        sendto(sockfd, buf, strlen(buf)+1, 0,
                (struct sockaddr *)srvAddr, sizeof(*srvAddr));

        return 0;
    }

    return -1;
}

/*
 * @param sockfd the udp socket
 */
void recvPingResponse(int sockfd)
{
    int addrLen;
    int fromId;
    char buf[BUFLEN];
    char cmd[BUFLEN];
    struct sockaddr_in addr;

    recvfrom(sockfd, buf, sizeof(buf), 0, 
            (struct sockaddr *)&addr, &addrLen);

    sscanf(buf, "%s%d", cmd, &fromId);

    //printf("ping response:%s\n", buf);

    if (strcmp(cmd, "response") == 0) {
        printf("A ping response message was received from Peer %d.\n", fromId);
    }
}

/*
 * udp connection init
 */
void udpConnectInit()
{	
    udpfd1 = socket(PF_INET, SOCK_DGRAM, 0);

    bzero(&srvAddr1, sizeof(srvAddr1));
    srvAddr1.sin_family = AF_INET;
    srvAddr1.sin_port = htons(peerUdpPort(firstId));
    srvAddr1.sin_addr.s_addr = inet_addr(TARGETHOST);

    udpfd2 = socket(PF_INET, SOCK_DGRAM, 0);

    bzero(&srvAddr2, sizeof(srvAddr2));
    srvAddr2.sin_family = AF_INET;
    srvAddr2.sin_port = htons(peerUdpPort(nextId));
    srvAddr2.sin_addr.s_addr = inet_addr(TARGETHOST);
}

/*
 * @param sig int signal
 */
void ping_handler(int sig)
{
    struct IdItem it;
    lseek(nfd, 0, SEEK_SET);
    read(nfd, &it, sizeof(it));

    firstId = it.fId;
    nextId = it.pId;

    srvAddr1.sin_port = htons(peerUdpPort(firstId));
    srvAddr2.sin_port = htons(peerUdpPort(nextId));

    /* send ping to successor */
    if (udpfd1 != -1) {
        sendPingRequest(udpfd1, &srvAddr1);
        recvPingResponse(udpfd1);
    }

    /* send ping to next-successor */
    if (udpfd2 != -1) {
        sendPingRequest(udpfd2, &srvAddr2);
        recvPingResponse(udpfd2);
    }

    alarm(15);
}

/*
 * @param fileNo int
 * @return int node id in CDHT
 */
int hashFileNo(int fileNo)
{
    /* just simple hash */

    return fileNo % 256;
}

/*
 * @param fileNo file number
 * @param selfId node id
 * @param prevId presuccessor id
 * @return 1 or 0
 */
int findLocation(int fileNo, int selfId, int prevId)
{
    int peerId = hashFileNo(fileNo);

    //printf("peerId=%d\n", peerId);

    if (prevId < selfId) {
        if (peerId >= prevId && peerId <= selfId) return 1;
    } else {
        if (peerId > selfId && peerId > prevId) return 1;
    }

    return 0;
}

/*
 * @param remoteIp target host ip or name
 * @param remotePort target host's port
 * @return int sockfd connected, -1 for error
 */
int doTcpConnect(char *remoteIp, int remotePort)
{
    struct sockaddr_in srvAddr;
    int rs, sockfd;

    struct hostent *h = gethostbyname(remoteIp);
    if (h == NULL) {
        printf("Unknown host:%s\n", remoteIp);
        return -1;
    }

    srvAddr.sin_family = h->h_addrtype;
    memcpy((char *)&srvAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    srvAddr.sin_port = htons(remotePort);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("doTcpConnect:SOCK_STREAM error");
        return -1;
    }

    rs = connect(sockfd, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if (rs < 0) {
        perror("doTcpConnect: connect error");
        return -1;
    }

    return sockfd;
}

/*
 * @param sockfd connection socket
 * @param foundId node has the file number
 * @param fileNo file number
 * @return int -1:error
 */
int sendFoundMessage(int sockfd, int foundId, int fileNo)
{
    char buf[BUFLEN];

    sprintf(buf, "found %d %d", foundId, fileNo);

    return send(sockfd, buf, strlen(buf)+1, 0);
}

/*
 * @param sockfd connection socket
 * @param quitId to quit node
 * @param firstId node's successor
 * @param nextId node's next successor
 * @param flag 1:first, 2:next, 3:update prevId
 * @return int -1:error
 */
int sendQuitMessage(int sockfd, int quitId, int firstId, int nextId, int flag)
{
    char buf[BUFLEN];

    if (flag == 1)
        sprintf(buf, "quit1 %d %d", quitId, nextId);
    else if (flag == 2)
        sprintf(buf, "quit2 %d %d", quitId, firstId);
    else if (flag == 3)
        sprintf(buf, "quit3 %d", quitId);

    return send(sockfd, buf, strlen(buf)+1, 0);
}

/*
 * @param sockfd connection socket
 * @param fileNo to find file nubmer
 * @param reqId	request node id
 * @param prevId previous node id
 * @return int -1: error
 */
int sendRequestMessage(int sockfd, int fileNo, int reqId, int prevId)
{
    char buf[BUFLEN];

    sprintf(buf, "request %d %d %d", fileNo, reqId, prevId);

    return send(sockfd, buf, strlen(buf)+1, 0);
}

/* rcv_line is my function readline(). Data is read from the socket when */
/* needed, but not byte after bytes. All the received data is read.      */
/* This means only one call to recv(), instead of one call for           */
/* each received byte.                                                   */
/* You can set END_CHAR to whatever means endofline for you. (0x0A is \n)*/
/* read_lin returns the number of bytes returned in line_to_return       */
int read_line(int newSd, char *line_to_return) 
{

    static int rcv_ptr=0;
    static char rcv_msg[BUFLEN];
    static int n;
    int offset;

    offset=0;

    while(1) {
        if(rcv_ptr==0) {
            /* read data from socket */
            memset(rcv_msg,0x0,BUFLEN); /* init buffer */
            n = recv(newSd, rcv_msg, BUFLEN, 0); /* wait for data */
            if (n<0) {
                perror(" cannot receive data ");
                return ERROR;
            } else if (n==0) {
                //printf(" connection closed by client\n");
                close(newSd);
                return ERROR;
            }
        }

        /* if new data read on socket */
        /* OR */
        /* if another line is still in buffer */

        /* copy line into 'line_to_return' */
        while(*(rcv_msg+rcv_ptr)!=END_LINE && rcv_ptr<n) {
            memcpy(line_to_return+offset,rcv_msg+rcv_ptr,1);
            offset++;
            rcv_ptr++;
        }

        /* end of line + end of buffer => return line */
        if(rcv_ptr==n-1) { 
            /* set last byte to END_LINE */
            *(line_to_return+offset)=END_LINE;
            rcv_ptr=0;
            return ++offset;
        } 

        /* end of line but still some data in buffer => return line */
        if(rcv_ptr <n-1) {
            /* set last byte to END_LINE */
            *(line_to_return+offset)=END_LINE;
            rcv_ptr++;
            return ++offset;
        }

        /* end of buffer but line is not ended => */
        /*  wait for more data to arrive on socket */
        if(rcv_ptr == n) {
            rcv_ptr = 0;
        } 

    } /* while */
}

/*
 * @param line char buffer
 * @return int ret < 0 : error else the file number
 */
int checkFileNo(char *line)
{
    char cmd[BUFLEN], filename[BUFLEN];

    if (sscanf(line, "%s%s", cmd, filename) != 2) {
        return -1;
    }

    if (strlen(filename) != 4) {
        return -1;
    }

    int i, ret = 0;
    for (i = 0; i < strlen(filename); i++) {
        if (filename[i] < '0' || filename[i] > '9') return -1;

        ret = ret * 10 + (filename[i] - '0');
    }

    return ret;
}

/*
 * @param udpPort udp port number
 * @return int udp-socket fd
 */
int doUdpBind(int udpPort)
{
    int sockfd;

    struct sockaddr_in srvAddr;

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);

    bzero(&srvAddr, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAddr.sin_port = htons(udpPort);

    int optval = 1;
    int optlen = sizeof(optval);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, optlen);
    bind(sockfd, (struct sockaddr *)&srvAddr, sizeof(srvAddr));

    return sockfd;
}

int main(int argc, char **argv)
{

    int listenfd, clientfd, cliLen;
    struct sockaddr_in cliAddr, tcpAddr;

    if (argc != 4) {
        printf("usage: <%s> self-id first-successor second-successor\n", 
                argv[0]);
        return ERROR;
    }

    selfId = atoi(argv[1]); /* self-id */
    firstId = atoi(argv[2]); /* first successor */
    nextId = atoi(argv[3]); /* second successor */

    char fileName[16];
    sprintf(fileName, "id-%d.txt", selfId);

    int fd = open(fileName, O_RDWR|O_CREAT, 0666);
    if (fd < 0) {
        printf("file open failed.\n");
        return ERROR;
    }

    sprintf(fileName, "next-%d.txt", selfId);
    nfd = open(fileName, O_RDWR|O_CREAT, 0666);
    if (nfd < 0) {
        printf("file open failed.\n");
        return ERROR;
    }

    struct IdItem it;
    it.fId = firstId;
    it.pId = nextId;

    lseek(nfd, 0, SEEK_SET);
    write(nfd, &it, sizeof(it));

    tcpPort = peerTcpPort(selfId);
    udpPort = peerUdpPort(selfId);

    /* create socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("cannot open socket");
        return ERROR;
    }

    int optval = 1;
    int optlen = sizeof(optval);

    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, optlen);

    /* bind server port */
    tcpAddr.sin_family = AF_INET;
    tcpAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcpAddr.sin_port = htons(tcpPort);

    if (bind(listenfd, (struct sockaddr *) &tcpAddr, sizeof(tcpAddr)) < 0) {
        perror("cannot bind port");
        return ERROR;
    }

    listen(listenfd, 5);

    /* 1. fork and listen the request */
    pid_t pid1, pid2;
    int status;

    if ((pid1 = fork()) < 0) {
        perror("fork error");
        return ERROR;
    } else if (pid1 == 0) {
        /* child process */
        char line[BUFLEN];

        printf("Waiting For Connect from Peer..\n");

        while (1) {
            cliLen = sizeof(cliAddr);
            clientfd = accept(listenfd, (struct sockaddr *) &cliAddr, &cliLen);
            if (clientfd < 0) {
                perror("cannot accept connection");
                exit(0);
            }

            memset(line, 0x0, BUFLEN);

            /* dispose connection */
            while (read_line(clientfd, line) != ERROR) {
                char cmd[BUFLEN];

                sscanf(line, "%s", cmd);

                if (strcmp(cmd, "request") == 0) {
                    int prevId, reqId, fileNo;

                    sscanf(line, "%s%d%d%d", cmd, &fileNo, &reqId, &prevId);

                    if (findLocation(fileNo, selfId, prevId)) {
                        printf("File %04d is here.\n", fileNo);
                        int confd = doTcpConnect(TARGETHOST, peerTcpPort(reqId));
                        if (confd > 0) {
                            printf("A response message, destined for peer %d, has been sent.\n", reqId);
                            sendFoundMessage(confd, selfId, fileNo);
                            close(confd);
                        }
                    } else {
                        printf("File %04d is not stored here.\n", fileNo);
                        int confd = doTcpConnect(TARGETHOST, peerTcpPort(firstId));
                        if (confd > 0) {
                            printf("File request message has been forwarded to my successor\n");
                            sendRequestMessage(confd, fileNo, reqId, selfId);
                            close(confd);
                        }
                    }
                } else if (strcmp(cmd, "found") == 0) {
                    int foundId, fileNo;
                    sscanf(line, "%s%d%d", cmd, &foundId, &fileNo);

                    printf("Received a response message from peer %d, which has the file %04d.\n", foundId, fileNo);

                } else if (strcmp(cmd, "quit1") == 0) {
                    int quitId;

                    firstId = nextId;
                    sscanf(line, "%s%d%d", cmd, &quitId, &nextId);

                    printf("Peer %d will depart from the network.\n", quitId);
                    printf("My first succesor is now peer %d.\n", firstId);
                    printf("My second succesor is now peer %d.\n", nextId);

                    struct IdItem it;
                    it.fId = firstId;
                    it.pId = nextId;

                    lseek(nfd, 0, SEEK_SET);
                    write(nfd, &it, sizeof(it));

                } else if (strcmp(cmd, "quit2") == 0) {
                    int quitId = 0;
                    sscanf(line, "%s%d%d", cmd, &quitId, &nextId);

                    printf("Peer %d will depart from the network.\n", quitId);
                    printf("My first succesor is now peer %d.\n", firstId);
                    printf("My second succesor is now peer %d.\n", nextId);

                    struct IdItem it;
                    it.fId = firstId;
                    it.pId = nextId;

                    lseek(nfd, 0, SEEK_SET);
                    write(nfd, &it, sizeof(it));

                } else if (strcmp(cmd, "quit3") == 0) {
                    prevId = sprevId = -1;

                    struct IdItem item;
                    item.fId = prevId;
                    item.pId = sprevId;

                    lseek(fd, 0, SEEK_SET);
                    write(fd, &item, sizeof(item));
                }

                memset(line, 0x0, BUFLEN);
            }
        }

        exit(0);
    } else {
        //if (wait(&status) < 0) perror("wait error");
    }

    /* 2. prepare the upd server */
    int udpfd = doUdpBind(udpPort);
    udpConnectInit();

    /* 3. do the ping connection */
    if (signal(SIGALRM, ping_handler) == SIG_ERR) {
        perror("signal error");
    }

    alarm(15);

    char buf[BUFLEN];
    char cmd[BUFLEN];

    /* fork upd server */
    if ((pid2 = fork()) == 0) {
        struct sockaddr_in cli_addr;
        int addrLen = sizeof(cli_addr);

        while (1) {
            recvfrom(udpfd, buf, sizeof(buf), 0, 
                    (struct sockaddr *)&cli_addr, &addrLen);

            int fromId;
            sscanf(buf, "%s%d", cmd, &fromId);

            if (strcmp(cmd, "ping") == 0) {
                printf("A ping request message was received from Peer %d\n", fromId);

                struct IdItem item;

                read(fd, &item, sizeof(item));

                if (prevId != -1 && sprevId != -1) {
                    prevId = item.fId;
                    sprevId = item.pId;
                }

                if (prevId == -1) { /* update predecessor id */
                    prevId = fromId;
                } 

                if (sprevId == -1 && fromId != prevId) {
                    sprevId = fromId;
                }

                if (prevId != -1 && sprevId != -1) {

                    if (sprevId < prevId && prevId < selfId) {
                        item.fId = prevId;
                        item.pId = sprevId;
                    } else if (sprevId > prevId && sprevId > selfId && prevId < selfId) {
                        item.fId = prevId;
                        item.pId = sprevId;
                    } else {
                        item.fId = sprevId;
                        item.pId = prevId;
                    }

                    lseek(fd, 0, SEEK_SET);
                    //printf("write=%d,%d,%d\n", item.fId, item.pId, sizeof(item));
                    write(fd, &item, sizeof(item));
                }

                /* send response back */
                sprintf(buf, "response %d", selfId);
                sendto(udpfd, buf, strlen(buf)+1, 0,
                        (struct sockaddr *)&cli_addr, addrLen);
            }
        }

        exit(0);
    }

    /* 4. wait for user input and reponse it */

    fd_set rfds; /* fds for select */
    int maxfd;
    struct timeval tv;

    //printf("pid1=%d, pid2=%d\n", pid1, pid2);

    while (1) {

        FD_ZERO(&rfds);
        FD_SET(0, &rfds);  /* listen stdin */
        maxfd = 0;

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        if (select(maxfd+1, &rfds, NULL, NULL, &tv) == 0) {
            continue;
        } else {
            if (FD_ISSET(0, &rfds)) { /* handle input */
                if (fgets(buf, BUFLEN, stdin) == NULL) {
                    perror("input error");
                    break;
                }

                sscanf(buf, "%s", cmd); /* interpret commands */

                if (strcasecmp(cmd, "quit") == 0) {
                    /* send quit message to predecessors */

                    /* load id */
                    lseek(fd, 0, SEEK_SET);
                    struct IdItem item;

                    read(fd, &item, sizeof(item));
                    prevId = item.fId;
                    sprevId = item.pId;

                    printf("pId=%d,spId=%d\n", prevId, sprevId);

                    int confd = doTcpConnect(TARGETHOST, peerTcpPort(prevId));
                    if (confd > 0) {
                        sendQuitMessage(confd, selfId, firstId, nextId, 1);
                        close(confd); /* close */
                    }

                    confd = doTcpConnect(TARGETHOST, peerTcpPort(sprevId));
                    if (confd > 0) {
                        sendQuitMessage(confd, selfId, firstId, nextId, 2);
                        close(confd);
                    }

                    confd = doTcpConnect(TARGETHOST, peerTcpPort(firstId));
                    if (confd > 0) {
                        sendQuitMessage(confd, selfId, firstId, nextId, 3);
                        close(confd);
                    }

                    confd = doTcpConnect(TARGETHOST, peerTcpPort(nextId));
                    if (confd > 0) {
                        sendQuitMessage(confd, selfId, firstId, nextId, 3);
                        close(confd);
                    }

                    kill(pid1, SIGINT);
                    kill(pid2, SIGINT);
                    exit(0);

                    break;

                } else if (strcasecmp(cmd, "request") == 0) {
                    int fileNo = checkFileNo(buf);

                    if (fileNo < 0) { /* simulate file location */
                        printf("File Name Not Valid.\n");
                    } else {

                        //printf("fileNo=%04d\n", fileNo);

                        if (hashFileNo(fileNo) == selfId) {
                            printf("File %04d is here.\n", fileNo);
                        } else {
                            printf("File %04d is not Stored here.\n", fileNo);
                            /* do connect */
                            int confd = doTcpConnect(TARGETHOST, peerTcpPort(firstId));
                            /* send message */

                            if (confd > 0) {
                                printf("File request message has been forwarded to my successor\n");
                                sendRequestMessage(confd, fileNo, selfId, selfId);
                                close(confd); /* close */
                            }
                        }
                    }
                }
            } /* end input */
        } /* end select */
    }

    wait(&status);

    close(fd);

    return SUCCESS;
}
