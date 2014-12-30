// Function: client

#include <stdio.h>
#include <winsock2.h>

#include <string>
#include <io.h>
#include <direct.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT 5150
#define DEFAULT_BUFFER 2048

#define dprintf	printf

char szServer[128]="127.0.0.1", szbuf[1024];

int iPort = DEFAULT_PORT;

void send_file_dir (string path, int baselen, SOCKET sockfd);
int  send_file (string filename, int baselen, SOCKET sockfd);

void send_end_tag(char *tag, SOCKET sockfd)
{
	memset(szbuf, '\0', sizeof(szbuf));
	strcat(szbuf, tag);

	send(sockfd, szbuf, sizeof(szbuf), 0);
}

//recursive send file dir
void send_file_dir(string path, int baselen, SOCKET sockfd)
{
	struct _finddata_t filefind;
	string curr = path + "\\*.*";
	int done = 0, i, handle;

	dprintf("[%s]\n", path.c_str());

	if ((handle = _findfirst(curr.c_str(), &filefind)) == -1)
		return ; // no exist file in folder

	while (!(done = _findnext(handle, &filefind))) {
		if (!strcmp(filefind.name, ".."))
			continue;

		// is a folder
		if ((_A_SUBDIR == filefind.attrib)) {
			curr = path + "\\" + filefind.name;

			send_file_dir(curr, baselen, sockfd);
		}
		else {
			// send file content to server
			send_file(path+"\\"+filefind.name, baselen+1, sockfd);
		}
	}

	_findclose(handle);
}

// send file to server, return send size
int send_file(string filename, int baselen, SOCKET sockfd)
{
	int bytesSent;

	//1. send file-path to server
	string relpath = filename.substr(baselen);

	char path[1024];
	
	memset(path, 0, sizeof(path));
	strcat(path, relpath.c_str());

	dprintf("send file-name %s to server.\n", path);
    bytesSent = send(sockfd, path, 1024, 0);

	if (strcmp(path, "END-FIL") == 0) return -1;

	//2. send file to server
	int sendlen = 0, rlen = 0, slen = 0;
	char sendbuf[BUFSIZ];

	FILE* fp = fopen(filename.c_str(), "rb");
	if (fp == NULL) return -1;

	while (1) {
		memset(sendbuf, 0, sizeof(sendbuf));

		rlen = fread(sendbuf, 1, BUFSIZ, fp);

		if (rlen <= 0) break;

		do {
			slen = send(sockfd, sendbuf, rlen, 0);
		} while (slen != rlen);

		sendlen += slen;
	}

	//send_end_tag("END-FILE", sockfd);

	dprintf("send file %s to server %d bytes\n",
			filename.c_str(), sendlen);

	fclose(fp);

	return sendlen;
}

// send user info
int send_user_info(int opt, char *name, char *pwd, SOCKET clientfd)
{
	char sndbuf[1024] = {0};
	int ret = 0, slen, rlen;

	memset(sndbuf, '\0', sizeof(sndbuf));
	sprintf(sndbuf, "%d %s %s", opt, name, pwd); 

	slen = send(clientfd, sndbuf, strlen(sndbuf), 0);

	dprintf("Send to Server:%s.\n", sndbuf);

	rlen = SOCKET_ERROR;

	while (rlen == SOCKET_ERROR) {
		rlen = recv(clientfd, sndbuf, 1024, 0);
		if (rlen == 0 || rlen == WSAECONNRESET) {
			printf("Connection Closed.\n");
			break;
		}

		if (rlen < 0) return 0;

		dprintf("Client Recv:%s.\n", sndbuf);
	}

	if (sndbuf[0] == '0') {
		printf("User Already Exist.\n");
	}
	else if (sndbuf[0] == '1') {
		printf("User Name Non-Exist.\n");
	}
	else if (sndbuf[0] == '2') {
		printf("User Password Wrong.\n");
	}
	else if (sndbuf[0] == '3') {
		printf("User Signed Success.\n");
		ret = 1;
	}
	else if (sndbuf[0] == '4') {
		printf("User Login Success.\n");
		ret = 1;
	}

	return ret;
}

/* recv dir list and display */
void recv_dir_list(SOCKET sockfd)
{
	int rlen;

	printf("\nRemote Dir-List:\n");

	do {
		rlen = recv(sockfd, szbuf, 1024, 0);

		if (rlen == 0) break;

		if (strcmp(szbuf, "END-LST") == 0) break;

		printf("%s\n", szbuf);

	} while (1);
}

/* 
 * is a dir : 0
 * is a file: 1
 * not exist: -1
 */
int valid_path_file(char *path)
{
	if (path == NULL) return -1;

	int r = _access(path, 0);

	if (r != 0) return -1;

	FILE *fp = fopen(path, "r+");
	if (fp != NULL) return 1;

	return 0;
}

// receive file from server, return file size
int recv_file(char *filename, SOCKET sockfd)
{
	FILE* fp = fopen(filename, "wb+");

	if (fp == NULL) return -1;

	int recvlen = 0, rlen, wlen;
	char buf[BUFSIZ];

	while (1) {
		memset(buf, 0, BUFSIZ);

		do {
			rlen = recv(sockfd, buf, BUFSIZ, 0);
		} while (rlen < 0);

		if (rlen == 0) break;

		wlen = fwrite(buf, 1, rlen, fp);

		recvlen += wlen;

		// recv ends
		if (rlen < BUFSIZ) break;
	}

	dprintf("Write file %s bytes %d\n", filename, recvlen);
	fclose(fp);

	return recvlen;
}

void recv_get_file(char *filename, SOCKET sockfd)
{
	// 1. send filename to server
	memset(szbuf, '\0', sizeof(szbuf));
	strcat(szbuf, filename);

	send(sockfd, szbuf, sizeof(szbuf), 0);

	// 2. recv file from server
	memset(szbuf, '\0', sizeof(szbuf));
	recv(sockfd, szbuf, 7, 0);

	if (strcmp(szbuf, "GET-ERR") == 0) {
		printf("File <%s> Not Found.\n", filename);
	}
	else {

		memset(szbuf, '\0', sizeof(szbuf));
		strcat(szbuf, filename);

		// recv and store file 
		char *barename = szbuf+strlen(szbuf) - 1;
		while (*barename != '/' && *barename != '\\'
				&& barename != szbuf)
			barename--;

		if (*barename == '/' || *barename == '\\') barename++;

		dprintf("BareFileName:%s.\n", barename);

		// create and store
		recv_file(barename, sockfd);
	}
}

int main() {

    //initialize
    WSADATA wsaData;
    if( WSAStartup( MAKEWORD(2,2), &wsaData ) != NO_ERROR ){
        printf("Failed to load Winsock library!\n");
		exit (1);
	}

    //build socket.
    SOCKET client;
    client = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( client == INVALID_SOCKET ) {
        printf( "socket error: %ld\n", WSAGetLastError());
        WSACleanup();
        exit (1);
    }

    //connect to server
    sockaddr_in clientService;

    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(szServer);
    clientService.sin_port = htons(DEFAULT_PORT);

    if ( connect( client, (SOCKADDR*) &clientService, sizeof(clientService) ) == SOCKET_ERROR) {
		printf( "connect error: %ld\n", WSAGetLastError());
        WSACleanup();
        exit (1);
    }

    //send and receive data
	//
	char buf[256], name[256], pwd[256];
	char *cmd_args[2];
	string path;
	int opt, ret;
	int i;

	while (1) {
		printf("Choices(1: sign in; 2: login; 0: close) >> ");

		scanf("%d", &opt);

		if (opt == 0) {
			ret = 0;
			break;
		}
		else if (opt == 1 || opt == 2) {
			printf("Please Input User Name(No Space Allowed):");
			scanf("%255s", name);

			printf("Please Input User Password(No Space Allowed):");
			scanf("%255s", pwd);

			// send user information to server and receive authification
			ret = send_user_info(opt, name, pwd, client);

			if (ret == 1) break;
		}
		else {
			printf("Invalid Choices, Retry.\n");
		}
	}

	getchar();

	while (ret) {

		printf("Commands (LST; PUT <file>; GET <file>; EXI;) >>");
		//scanf("%s", buf);
		fgets(buf, sizeof(buf), stdin);

		if (strlen(buf) < 3) continue;

		buf[strlen(buf)-1] = '\0'; //replace '\n'

		cmd_args[0] = buf;
		cmd_args[1] = NULL;

		for (i = 0; i < strlen(buf); i++) {
			if (buf[i] == ' ') {
				buf[i] = '\0';

				cmd_args[1] = buf+i+1;
				break;
			}
		}

		if (strcmp(cmd_args[0], "LST") == 0) {
			/* get server directory list */

			send(client, cmd_args[0], 3, 0);

			recv_dir_list(client);
		}
		else if (strcmp(cmd_args[0], "PUT") == 0) {
			/* send file to server */

			int type = valid_path_file(cmd_args[1]);

			dprintf("dir:%s.\n", cmd_args[1]);

			if (type == -1 || type == 0) {
				printf("<file> Not Exist, Retry.\n");
				continue;
			}

			send(client, cmd_args[0], 3, 0);

			path = cmd_args[1];

			if (type == 0) { /* send dir */
				send_file_dir(path, strlen(cmd_args[1]), client);
			}
			else if (type == 1) { /* send file */
				int blen = path.length();

				while (blen > 0 && path.at(blen-1) != '/'
						&& path.at(blen-1) != '\\') blen--;

				send_file(path, blen, client);
			}

			//send_file("END-FIL", 0, client);
		}
		else if (strcmp(cmd_args[0], "GET") == 0) {
			/* get single file from server, store it in local dir */

			if (cmd_args[1] == NULL) {
				printf("<file> should not be null, retry.\n");
				continue;
			}

			send (client, cmd_args[0], 3, 0);

			recv_get_file(cmd_args[1], client);
		}
		else if (strcmp(cmd_args[0], "EXI") == 0) {
			break;
		}
		else {
			printf("Invalid Command, retry..\n");
		}
	}

	/*
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
    char sendbuf[32] = "hello, I am client.";
    char recvbuf[32] = "";

    bytesSent = send( client, sendbuf, strlen(sendbuf), 0 );
    printf( "client sent : %s\n",  sendbuf);

    while( bytesRecv == SOCKET_ERROR ) {
        bytesRecv = recv( client, recvbuf, 32, 0 );
        if ( bytesRecv == 0 || bytesRecv == WSAECONNRESET ) {
            printf( "Connection Closed.\n");
            break;
        }
        if (bytesRecv < 0)
            return 0;
        printf( "client receive : %s\n", recvbuf);
    }
	*/

	closesocket(client);
	WSACleanup();

    return 0;
}
