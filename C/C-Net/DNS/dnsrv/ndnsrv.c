#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "dnshelper.h"
#include "dnsdb.h"

//#define ROOT_SERVER_IP "198.41.0.4"
//#define ROOT_SERVER_IP "172.24.2.71"
//#define ROOT_SERVER "a.root-servers.net"
//#define DNS_SERVER "172.24.2.71"

#define ROOT_SERVER_IP "127.0.0.1"
#define ROOT_SERVER "a.com"
#define DNS_SERVER "127.0.0.1"

char hostcache[100][100];
char ipcache[100][20];
// Next Free Index
int nfi = 0;
int maxi = 0;

union longchar {
	char  a[5];
	long p;
};
typedef union longchar lchar;

char *myCheckCache(char *host, list *head)
{
	node *t = find_dns(head, host);

	if (NULL != t)
	{
		return t->ip;
	}

	return NULL;
}

void myAddToCache(char *host, char *ip, list **head)
{
	node *n = (node *) malloc(sizeof(node));

	strcpy(n->hostname, host);
	n->no = 8600;
	strcpy(n->in, "IN");
	strcpy(n->type, "A");
	strcpy(n->ip, ip);

	add_dns(head, n);
}

void handleDNSRequest(int, list **head);

int main(int argc, char const *argv[])
{
	int sock;

	if (argc < 2)
	{
		printf("Usage: %s <dns_cache_file>\n", argv[0]);
		exit(1);
	}

	list *head = NULL;
	/* dns-db init */
	read_dns_file(&head, argv[1]);

	fd_set rset;
	// Construct the server address structure
	struct addrinfo addrCriteria;                   // Criteria for address
	memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
	addrCriteria.ai_family = AF_UNSPEC;             // Any address family
	addrCriteria.ai_flags = AI_PASSIVE;             // Accept on any address/port
	addrCriteria.ai_socktype = SOCK_DGRAM;          // Only datagram socket
	addrCriteria.ai_protocol = IPPROTO_UDP;         // Only UDP socket

	struct addrinfo *servAddr; // List of server addresses
	int rtnVal = getaddrinfo(NULL, "53", &addrCriteria, &servAddr);
	if (rtnVal != 0)
	{
		perror("getaddrinfo() failed");
		exit(1);
	}

	// Create socket for incoming connections
	if((sock = socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol)) < 0)
	{
		perror("socket() failed");
		exit(1);
	}

	//设置为可重复使用
	int yes = 1;
	if(setsockopt(sock ,SOL_SOCKET,SO_REUSEADDR|SO_BROADCAST, &yes, sizeof(int))==-1)
	{
		perror("setsockopt");
		return 0;
	}

	// Bind to the local address
	if (bind(sock, servAddr->ai_addr, servAddr->ai_addrlen) < 0)
	{
		perror("bind() failed");
		exit(1);
	}

	// Free address list allocated by getaddrinfo()
	freeaddrinfo(servAddr);

	FD_ZERO(&rset);
	FD_SET(sock, &rset);
	while(1) {
		select(sock + 1, &rset, NULL, NULL, NULL);

		if(FD_ISSET(sock, &rset)) {
			handleDNSRequest(sock, &head);
		}
	}

	write_out_dns(head, argv[1]);
	destroy_list(head);

	return 0;
}

void handleDNSRequest(int sock, list **head) 
{
	char  buf[65536],*qname, *writer, hname[20], *host;
	unsigned short reqid, qcount;
	unsigned int replysize = 0;
	struct DNS_HEADER *dns = NULL;
	struct QUESTION *qinfo = NULL;
	struct RES_RECORD *answer,*auth;
	int stop = 0, recursion_desired;
	char *ip;
	lchar lc;
	ghreply hostdetails, tmphd;
	struct in_addr *addr;

	struct sockaddr_storage clntAddr;
	socklen_t clntAddrLen = sizeof(clntAddr);
	addr = (struct in_addr*)malloc(sizeof(struct in_addr));

	if(recvfrom(sock,(char*)buf , 65536, 0, (struct sockaddr*)&clntAddr, &clntAddrLen) < 0) {
		perror("recvfrom");
	}
	dns = (struct DNS_HEADER*) buf;

	if(dns->qr != 0) {
		return;
	}
	writer = &buf[sizeof(struct DNS_HEADER)];
	// information about request
	host = ReadName(writer,buf,&stop);
	recursion_desired = dns->rd;
	reqid = dns->id;
	qcount = ntohs(dns->q_count);

	// preparing response
	bzero(buf,sizeof(buf));
	dns = (struct DNS_HEADER *)&buf;
	dns->id = reqid;
	dns->qr = 1;
	dns->opcode = 0;
	dns->aa = 0;
	dns->tc = 0;
	dns->rd = 1;
	dns->ra = 1;
	dns->z = 0;
	dns->ad = 0;
	dns->cd = 0;

	dns->rcode = 0;
	// 0 - NoError
	// 3 - NXDomain
	// 4 - NotImp

	dns->q_count = htons(1);
	dns->ans_count = htons(0);
	dns->auth_count = htons(0);
	dns->add_count = htons(0);

	qname =(char *)&buf[sizeof(struct DNS_HEADER)];
	ChangetoDnsNameFormat(qname , host);
	host[strlen(host)-1] = '\0';
	qinfo =(struct QUESTION*)&buf[sizeof(struct DNS_HEADER) + (strlen((const char*)qname) + 1)];
	qinfo->qtype = htons( T_A );
	qinfo->qclass = htons(1);
	writer = &buf[sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION)];
	replysize = sizeof(struct DNS_HEADER) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION);


	if(qcount != 1) {
		dns->rcode = 4;
	}
	else if((ip = myCheckCache(host, *head)) != NULL) {        // check cache for hostname
		printf("Host %s found in cache.\n",host);
		dns->rcode = 0;   // NoError
		dns->ans_count = htons(1);
	}
	else if(recursion_desired) 
	{
		/* 递归解析方式 */

		hostdetails = ngethostbyname(host, ROOT_SERVER_IP, 1, 1);
		host[strlen(host)-1] = '\0';

		while (hostdetails.type == 1) {

			/* 查询一级DNS */
			tmphd = ngethostbyname(hostdetails.details, DNS_SERVER, 1, 1);
			if(tmphd.type == -1) {
				// Not Found
				// Serious Error
				dns->rcode = 2;
			}
			else {
				hostdetails = ngethostbyname(host, tmphd.details, 1, 1);
				host[strlen(host)-1] = '\0';
			}
		}

		if(dns->rcode != 2) {

			if(hostdetails.type == -1) {
				printf("Host %s not found.\n",host);
				ip = NULL;
				dns->rcode = 3; // NXDomain
			}
			else {
				printf("Host %s found using root server.\n",host);
				ip = (char*)malloc(sizeof(char)*20);
				strncpy(ip,hostdetails.details,20);
			}
		}
	}
	else {
		// iterative 迭代解析
		/* 查询上级DNS服务器 */
		char mybuf[1024] = {0};
		char nhost[100] = {0};

		int i = 0, flag = 0;
		char *d = "的";
		node *t = NULL;
		peer_rq *rq = (peer_rq *)&mybuf;

		//printf("h=%s, n=%d\n", host, strlen(host));
		for (i = 0; i < strlen(host); i++)
		{
			/* 依次找每级DNS */
			nhost[i] = host[i];

			char z = host[i];
			if (z & 0x80)
			{
				int j = 0;
				flag = 1;
				for (j = 0; j < strlen(d) && i+j < strlen(host); j++)
				{
					if (d[j] != host[i+j])
						flag = 2;
				}

				if (flag == 1 && j == strlen(d))
				{
					/* 分出一段 */
					nhost[i] = 0;

					//printf("nhost=%s\n", nhost);
					/* 查找cache */
					t = find_dns(*head, nhost);

					if (NULL != t)
					{
						strcpy(rq->ip, t->ip);
						strcpy(rq->type, "1");
						strcpy(rq->hostname, host);

						/* 进行查询DNS */
						printf("Send to %s for query...\n", t->ip);
						rq = peerRequest(mybuf, 1024, t->ip);

						//printf("ret=%s, %s, %s\n", rq->hostname, rq->type, rq->ip);
						if (strcmp(rq->type, "2") == 0)
						{
							/* found ip */
							ip = rq->ip;
							printf("Found ip=%s\n", ip);

							break;
						}
						else if (strcmp(rq->type, "1") == 0)
						{ 
							printf("Send to %s for query...\n", rq->ip);

							while (strcmp(rq->type, "1") == 0)
							{
								rq = peerRequest(mybuf, 1024, rq->ip);
								//printf("in while=%s, %s, %s\n", rq->hostname, rq->type, rq->ip);
							}

							if (strcmp(rq->type, "2") == 0)
							{
								ip = rq->ip;
							}

							break;
						}
					}

					/* 接着找 */
					strcat(nhost, d);
					i+=2;
				}
			}
		}

		dns->rcode = 0;
		dns->ans_count = htons(1);

		/*
		dns->auth_count = htons(1);
		dns->add_count = htons(0);

		auth = (struct RES_RECORD*)malloc(sizeof(struct RES_RECORD));
		strcpy(writer,qname);
		writer = writer + strlen((const char*)qname)+1;
		replysize += strlen((const char*)qname)+1;

		auth->resource = (struct R_DATA*)(writer);
		auth->resource->type = htons(2);
		auth->resource->data_len = htons(strlen(ROOT_SERVER));
		auth->resource->ttl = htonl(146016);
		auth->resource->_class = htons(1);
		writer = writer + sizeof(struct R_DATA);
		replysize += sizeof(struct R_DATA);

		strcpy(writer," ");
		writer = writer + 1;
		replysize++;
		strcpy(writer,ROOT_SERVER);
		writer = writer + strlen(ROOT_SERVER);
		replysize += strlen(ROOT_SERVER);
		*/
	}

	if(ip != NULL) {
		// add to cache

		//addToCache(host,ip);
		if (myCheckCache(host, *head) == NULL)
		{
			myAddToCache(host, ip, head);
		}

		dns->rcode = 0;
		dns->ans_count = htons(1);

		dns->auth_count = htons(0);
		dns->add_count = htons(0);

		answer = (struct RES_RECORD*)malloc(sizeof(struct RES_RECORD));
		strcpy(writer,qname);
		writer = writer + strlen((const char*)qname)+1;
		replysize += strlen((const char*)qname)+1;

		answer->resource = (struct R_DATA*)(writer);
		answer->resource->type = htons(1);
		answer->resource->data_len = htons(4);
		answer->resource->ttl = htonl(1800);
		answer->resource->_class = htons(1);
		writer = writer + sizeof(struct R_DATA);
		replysize += sizeof(struct R_DATA);

		inet_aton(ip, addr);
		lc.p = addr->s_addr;
		writer[0] = lc.a[0];
		writer[1] = lc.a[1];
		writer[2] = lc.a[2];
		writer[3] = lc.a[3];
		writer = writer + 4;
		replysize += 4;
	}

	if(sendto(sock,(char*)buf, replysize, 0, (struct sockaddr*)&clntAddr,clntAddrLen) < 0) {
		perror("sendto failed");
	}
}
