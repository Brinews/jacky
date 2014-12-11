#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

#include <iconv.h>
#include <locale.h>
#include <wchar.h>

#include "dnshelper.h"
#include "dnsdb.h"

#define DNSPORT	53
#define MSGLEN	1024 

void hex_print(char *str, int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		printf("0x%02x ", str[i]);
		if (i > 0 && i % 16 == 0)
			printf("\n");
	}
}

int code_convert(char *from_charset,char *to_charset,
		char *inbuf,int inlen,char *outbuf,int outlen)
{
	iconv_t cd;
	int rc;

	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);
	if (cd==0)
		return -1;

	memset(outbuf,0,outlen);

	if (iconv(cd,pin,&inlen,pout,&outlen) == -1)
		return -1;

	iconv_close(cd);

	return 0;
}

int u2g(char *inbuf,int inlen,char *outbuf,int outlen)
{
	return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}

int u2ui(char *inbuf,int inlen,char *outbuf,int outlen)
{
	return code_convert("utf-8","unicode",inbuf,inlen,outbuf,outlen);
}

int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
	//return code_convert("gbk","utf-8",inbuf,inlen,outbuf,outlen);
}

int main(int argc, char **argv)
{
	int listenfd, connfd, n, yes = 1;
	struct sockaddr_in servaddr, cliaddr;

	if (argc < 3)
	{
		printf("usage: %s <bind ip> <cache file>\n", argv[0]);
		return -1;
	}

	list *head = NULL;
	read_dns_file(&head, argv[2]);

	setlocale(LC_ALL, "zh_CN.UTF-8");
	//setlocale(LC_CTYPE, "C");
	wchar_t wstr[MSGLEN]; //unicode

	socklen_t clilen;
	pid_t childpid;

	char mesg[MSGLEN] = {0};

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	//设置为可重复使用
	if(setsockopt(listenfd ,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
	{
		perror("setsockopt");
		return 0;
	}

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port = htons(DNSPORT);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	listen(listenfd, 1024);

	printf("----------------------------------------\n");
	printf("-Name Server: %-20s------\n", argv[1]);
	printf("----------------------------------------\n");

	for (;;)
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

		if ((childpid = fork()) == 0)
		{
			close(listenfd);
			close(1);

			for (;;)
			{
				n = recvfrom(connfd, mesg, MSGLEN, 0, (struct sockaddr *)&cliaddr, &clilen);

				/* read the cache and echo result */
				peer_rq *rq = (peer_rq *)mesg;

				//printf("hn=%s, n= %d\n", rq->hostname, n);
				node *t = find_dns(head, rq->hostname);

				if (NULL != t)
				{
					/* find the ip */
					strcpy(rq->ip, t->ip);

					// 1: peer request, 2: found, 0:not found
					strcpy(rq->type, "2"); 
				}
				else
				{
					/* return the next query dns */
					char nhost[100] = {0};
					char *d = "的";
					int i = 0, flag = 0;

					//printf("len=%d\n", strlen(rq->hostname));
					for (i = 0; i < strlen(rq->hostname); i++)
					{
						nhost[i] = rq->hostname[i];

						char z = rq->hostname[i];

						/* 中文字符 */
						if (z & 0x80)
						{
							int j = 0;
							flag = 1;
							for (j = 0; j < strlen(d) && i+j < strlen(rq->hostname); j++)
							{
								if (d[j] != rq->hostname[i+j])
									flag = 2;
							}

							if (flag == 1 && j == strlen(d))
							{
								/* 找到一个“的" */
								nhost[i] = 0;

								/* 查找cache */
								t = find_dns(head, nhost);
								if (NULL != t)
								{
									/* the next query dns-server */
									strcpy(rq->ip, t->ip);
									strcpy(rq->type, "1");

									printf("Return Next DNS: ip=%s\n", rq->ip);
									break;
								}

								/* 继续找下一个“的" */
								strcat(nhost, d);
								i += 2;
							}
						}
					}
				}

				if (t == NULL)
				{
					/* no found and no next */
					strcpy(rq->type, "0");
				}

				/* return the query result */
				sendto(connfd, mesg, MSGLEN, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
				
			}
		}

		close(connfd);
	}

	destroy_list(head);

	return 0;
}
