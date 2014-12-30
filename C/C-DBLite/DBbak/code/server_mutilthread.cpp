// Function: server in muitiple thread

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <io.h>
#include <direct.h>

#include <string>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT 5150
#define DEFAULT_BUFFER 2048
#define NUM_BACKLOG 5

#define dprintf	printf

char szServer[128]="127.0.0.1", szMsg[1024];
int iPort = DEFAULT_PORT;

typedef struct user_info_node {
	char name[256];
	char pwd[256];
	struct user_info_node *next;
}user_info_list;

user_info_list *head = NULL;

#define	BASEDIR		"../repo/"

#ifdef _WIN32
#define ACCESS	_access
#define MKDIR(a) _mkdir((a))
#elif _LINUX
#define ACCESS	access
#define MKDIR(a) mkdir((a), 0755)
#endif

int recv_file_name(char *filename, int len, SOCKET sockfd);
int create_file_dir(char *filename);
int recv_file(char *file, SOCKET sockfd);

void destroy_list(user_info_list *h);
char *find_list(char *name, user_info_list *h);
user_info_list *append_list(char *name, char *pwd, user_info_list *head);

void destroy_list(user_info_list *h)
{
	if (h != NULL) {
		destroy_list(h->next);
		free(h);
	}
}

char *find_list(char *name, user_info_list *h)
{
	while (h != NULL) {
		if (strcmp(h->name, name) == 0)
			return h->pwd;
	}

	return NULL;
}

user_info_list *append_list(char *name, char *pwd, user_info_list *h)
{
	user_info_list *n = (user_info_list *) malloc(sizeof(user_info_list));

	strcpy(n->name, name);
	strcpy(n->pwd, pwd);
	n->next = h;

	return n;
}

int recv_file_name(char *filename, int len, SOCKET sockfd)
{
	int rlen = 0;
	do {
		rlen = recv(sockfd, filename, len, 0);
	} while (0);

	return rlen;
}

// return 0: success, -1 : error
int create_file_dir(char *filename)
{
	int ret;
	int len = strlen(filename), i;

	for (i = 0; i < len; i++) {
		if (filename[i] == '\\' || filename[i] == '/') {
			// directory here
			filename[i] = '\0';

			// create if not exist 
			ret = ACCESS(filename, 0);
			if (ret != 0) {
				ret = MKDIR(filename);
				if (ret != 0)
					return -1;

				dprintf("Create Directory: %s.\n", filename);
			}

			filename[i] = '/';
		}
	}

	return 0;
}

// receive file from client, return file size
int recv_file(char *filename, SOCKET sockfd)
{
	FILE* fp = fopen(filename, "wb+");

	if (fp == NULL) return -1;

	int recvlen = 0, rlen, wlen;
	char buf[BUFSIZ];

	while (1) {
		memset(buf, 0, BUFSIZ);

		rlen = recv(sockfd, buf, BUFSIZ, 0);

		if (rlen <= 0) break;

		wlen = fwrite(buf, 1, rlen, fp);

		recvlen += wlen;

		// recv ends
		if (rlen < BUFSIZ) break;
	}

	dprintf("Write file %s bytes %d\n", filename, recvlen);
	fclose(fp);

	return recvlen;
}

int auth_user_info(int opt, char *name, char *pwd)
{
	if (opt == 1) {
		if (find_list(name, head) != NULL)
			return 0;

		head = append_list(name, pwd, head);

		return 3;
	}
	else if (opt == 2) {
		char *npwd;
		if ((npwd = find_list(name, head)) == NULL)
			return 1;

		if (strcmp(npwd, pwd) != 0)
			return 2;

		return 4;
	}

	// error
	return -1;
}

int recv_user_info(SOCKET sockfd)
{
	char rcvbuf[1024] = {0};
	int ret = 0, slen, rlen;

	memset(rcvbuf, '\0', sizeof(rcvbuf));

	rlen = SOCKET_ERROR;

	while (rlen == SOCKET_ERROR) {
		rlen = recv(sockfd, rcvbuf, 1024, 0);
		if (rlen == 0 || rlen == WSAECONNRESET) {
			printf("Connection Closed.\n");
			break;
		}
		
		if (rlen < 0) return -1;

		dprintf("Recv User Info:%s.\n", rcvbuf);
	}

	int opt;
	char name[256], pwd[256];

	sscanf(rcvbuf, "%d %s %s", &opt, name, pwd);

	ret = auth_user_info(opt, name, pwd);

	sprintf(rcvbuf, "%d", ret);
	slen = send(sockfd, rcvbuf, 1, 0);

	if (ret > 2) ret = 0; /* ok */
	else ret = 1;

	return ret;
}

void send_end_tag(char *tag, SOCKET sockfd)
{
	char szbuf[1024];

	memset(szbuf, '\0', sizeof(szbuf));
	strcat(szbuf, tag);

	send(sockfd, szbuf, sizeof(szbuf), 0);
}

void send_dir_list(string path, SOCKET sockfd)
{
	int done = 0, handle;
	char szbuf[1024];

	struct _finddata_t filefind;
	string cur = path + "\\*.*";

	dprintf("[%s]\n", cur.c_str());

	if ((handle = _findfirst(cur.c_str(), &filefind)) == -1)
		return;

	while (!(done = _findnext(handle, &filefind))) {
		if (!strcmp(filefind.name, ".."))
			continue;

		if ((_A_SUBDIR == filefind.attrib)) {

			cur = path + "/" + filefind.name;
			send_dir_list(cur, sockfd);
		}
		else {
			memset(szbuf, '\0', sizeof(szbuf));
			strcat(szbuf, path.substr(strlen(BASEDIR)).c_str());
			strcat(szbuf, "/");
			strcat(szbuf, filefind.name);

			send(sockfd, szbuf, sizeof(szbuf), 0);
		}
	}
}

void send_get_file(SOCKET sockfd)
{
	char filename[1024];
	char szbuf[1024];

	memset(szbuf, '\0', sizeof(szbuf));
	memset(filename, '\0', sizeof(filename));

	// 1. recv filename
	recv(sockfd, filename, sizeof(filename), 0);
	
	strcat(szbuf, BASEDIR);
	strcat(szbuf, filename);

	// 2. check file exists
	FILE *fp = fopen(szbuf, "rb");
	if (fp == NULL) {
		send(sockfd, "GET-ERR", 7, 0);
		return ;
	}

	send(sockfd, "GET-BEG", 7, 0);
	
	int rlen, slen, sndlen = 0;
	char sendbuf[BUFSIZ];

	// 3. send file
	while (1) {
		rlen = fread(sendbuf, 1, BUFSIZ, fp);

		if (rlen <=0 ) break;

		do {
			slen = send(sockfd, sendbuf, rlen, 0);
		} while (slen != rlen);

		sndlen += slen;
	}

	dprintf("send get file %s to client %d bytes.\n",
			szbuf, sndlen);

	fclose(fp);
}

DWORD  WINAPI  AnswerThread(LPVOID  lparam)     
{   
	SOCKET  clientSocket = (SOCKET)(LPVOID)lparam; 
	
    int bytesSent;
    int bytesRecv = SOCKET_ERROR;


	// receive user info to authentificate
	int ret;
	do {
		ret = recv_user_info(clientSocket);
	} while (ret);

	char command[4];
	int rlen, i;

	while (1) {

		//1. recv command
		rlen = SOCKET_ERROR;

		memset(command, '\0', sizeof(command));

		while (rlen == SOCKET_ERROR) {
			rlen = recv(clientSocket, command, 3, 0);
			if (rlen == 0 || rlen == WSAECONNRESET) {
				printf("Connection Closed.\n");
				break;
			}

			if (rlen < 0) return -1;
		}

		dprintf("Recv Command:%s.\n", command);

		//2. dispatch command
		if (strcmp(command, "LST") == 0) {
			// send directory list to client
			send_dir_list(BASEDIR, clientSocket);
			send_end_tag("END-LST", clientSocket);
		}
		else if (strcmp(command, "PUT") == 0) {

			while (1) {

				char filename[1024] = {0}; /* file-name max 1024 */
				memset(filename, 0, sizeof(filename));

				//1. recv file-path-name
				recv_file_name(filename, 1024, clientSocket);

				if (strcmp(filename, "END-FIL") == 0)
					break;

				char nfilename[1024] = {0};
				memset(nfilename, 0, sizeof(nfilename));
				strcat(nfilename, BASEDIR);
				strcat(nfilename, filename);

				dprintf("recv file-name:%s\n", nfilename);

				//2. make local-dir in server
				create_file_dir(nfilename);

				//3. recv file and write 
				recv_file(nfilename, clientSocket);

				break;
			}
		}
		else if (strcmp(command, "GET") == 0) {
			// send file to client
			send_get_file(clientSocket);
		}
	}

	/*
    char sendbuf[32] = "hello, I am server.";
    char recvbuf[32] = "";
    
    bytesRecv = recv( clientSocket, recvbuf, 32, 0 ); // receive content
    printf( "server receive : %s\n", recvbuf);
    
    bytesSent = send( clientSocket, sendbuf, strlen(sendbuf), 0 ); // send content
    printf( "server send : %s\n", sendbuf);

	*/

	closesocket(clientSocket);
	printf("finish connection.\n\n");
	return  0;     
}  


int main() {

    //initialize
    WSADATA wsaData;
    if( WSAStartup( MAKEWORD(2,2), &wsaData ) != NO_ERROR ){
        printf("Failed to load Winsock library!\n");
		exit (1);
	}

    //build socket
    SOCKET server;
    server = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if ( server == INVALID_SOCKET ) {
        printf( "socket error: %ld\n", WSAGetLastError());
        WSACleanup();
        exit (1);
    }

    //bind socket
    sockaddr_in service;

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(szServer);
    service.sin_port = htons(iPort);

    if (bind( server, (SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR ) {
        printf( "bind error: %ld.\n", WSAGetLastError());
        closesocket(server);
        exit (1);
    }
    
    //listen socket
    if ( listen( server, NUM_BACKLOG) == SOCKET_ERROR ){
		printf( "listen error: %ld.\n", WSAGetLastError());
        closesocket(server);
        exit (1);
	}

    
    SOCKET acceptSocket;
	DWORD  dwThreadId;   
	HANDLE  hThread;  

	while (1) {
		printf( "Waiting for a client to connect...\n" );
        acceptSocket = SOCKET_ERROR;
		// accept socket
        while ( acceptSocket == SOCKET_ERROR ) {
            acceptSocket = accept( server, NULL, NULL );
        }
        printf( "Client Connected.\n");
        hThread=CreateThread(NULL,NULL, AnswerThread, (LPVOID)acceptSocket,0,&dwThreadId);     
        if(hThread==NULL) {    
			printf("CreatThread  AnswerThread() failed.\n");     
		} 
		else     
		{  
			printf("CreateThread  OK.\n");     
        }   
    }
    

	closesocket(server);
	WSACleanup();

	destroy_list(head);

    return 0;
}

