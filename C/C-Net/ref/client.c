#include   <sys/stat.h>     
#include   <sys/types.h>     
#include   <sys/socket.h>     
#include   <stdio.h>     
#include   <malloc.h>     
#include   <netdb.h>     
#include   <fcntl.h>  
#include   <unistd.h>  
#include   <netinet/in.h>  
#include   <arpa/inet.h>  
#define    RES_LENGTH  10240 //接受字符的最大长度  
int     connect_socket(char * server,int serverPort);  
int     send_msg(int sockfd,char * sendBuff);  
char *  recv_msg(int sockfd);  
int     close_socket(int sockfd);  
int main(int argc, char ** argv)  
{  
	int   sockfd=0;  
	char  sendMsg[30]="abc.org/r/n/r";  
	char* res;  
	int   port = 4242;  
	char  ip[128] = {0};  
	strncpy(ip, "127.0.0.1", 128);  
	if(argc > 2)  
	{  
		strncpy(ip, argv[1], 128);  
		port = atoi(argv[2]);  
		printf("Input IP: %s, port : %d/n", ip, port);  
	}  
	else if(argc > 1)  
	{     
		port = atoi(argv[1]);  
		printf("Input port : %d/n", port);  
	}  
	sockfd=connect_socket(ip, port);  

	send_msg(sockfd,sendMsg);  
	/* res=recv_msg(sockfd); */  

	printf("return from recv function/n");  
	printf(res);  
	free(res);  
	close_socket(sockfd);  
	return 0;  
}  
/************************************************************ 
 * 连接SOCKET服务器，如果出错返回-1，否则返回socket处理代码 
 * server：服务器地址(域名或者IP),serverport：端口 
 * ********************************************************/  
int    connect_socket(char * server,int serverPort){  
	int    sockfd=0;  
	struct    sockaddr_in    addr;  
	struct    hostent        * phost;  
	//向系统注册，通知系统建立一个通信端口  
	//AF_INET表示使用IPv4协议  
	//SOCK_STREAM表示使用TCP协议  
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0){  
		herror("Init socket error!");  
		return -1;  
	}  
	bzero(&addr,sizeof(addr));  
	addr.sin_family = AF_INET;  
	addr.sin_port = htons(serverPort);  
	addr.sin_addr.s_addr = inet_addr(server);//按IP初始化  

	if(addr.sin_addr.s_addr == INADDR_NONE){//如果输入的是域名  
		phost = (struct hostent*)gethostbyname(server);  
		if(phost==NULL){  
			herror("Init socket s_addr error!");  
			return -1;  
		}  
		addr.sin_addr.s_addr =((struct in_addr*)phost->h_addr)->s_addr;  
	}  
	if(connect(sockfd,(struct sockaddr*)&addr, sizeof(addr))<0)  
	{  
		perror("Connect server fail!");  
		return -1; //0表示成功，-1表示失败  
	}  
	else  
		return sockfd;  
}  
/************************************************************** 
 * 发送消息，如果出错返回-1，否则返回发送的字符长度 
 * sockfd：socket标识，sendBuff：发送的字符串 
 * *********************************************************/  
int send_msg(int sockfd,char * sendBuff)  
{  
	int sendSize=0;  
	if((sendSize=send(sockfd,sendBuff,strlen(sendBuff),0))<=0){  
		herror("Send msg error!");  
		return -1;  
	}else  
		return sendSize;  
}  
/**************************************************************** 
 *接受消息，如果出错返回NULL，否则返回接受字符串的指针(动态分配，注意释放) 
 *sockfd：socket标识 
 * *********************************************************/  
char* recv_msg(int sockfd){  
	char * response;  
	int  flag=0,recLenth=0;  
	response=(char *)malloc(RES_LENGTH);  
	memset(response,0,RES_LENGTH);  

	for(flag=0;;)  
	{  
		printf("======recv data:/n");  
		if(( recLenth=recv(sockfd,response+flag,RES_LENGTH-flag,0))==-1 )  
		{  
			free(response);  
			printf("Return value : %d/n", recLenth);  
			perror("Recv msg error : ");  
			return NULL;  
		}  
		else if(recLenth==0)  
			break;  
		else  
		{  
			printf("%d char recieved data : %s./n", recLenth, response+flag);  
			flag+=recLenth;  
			recLenth=0;  
		}  
	}  
	printf("Return value : %d/n", recLenth);  
	response[flag]='/0';  
	return response;  
}  
/************************************************** 
 *关闭连接 
 * **********************************************/  
int close_socket(int sockfd)  
{  
	close(sockfd);  
	return 0;  
}  

