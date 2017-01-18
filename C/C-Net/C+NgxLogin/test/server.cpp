#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <mysql.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "jsoncpp/json/json.h"

#include "sqlhelper.h"

#define MSGLEN	512
#define PORT 9090
#define LISTEN_QUEUE 5
#define MAXBUF 1024
#define MAXEPOLL_SIZE 10000
#define MAX_EVENT 10

#define NAME_AND_PASSWD_SIZE  31
#define LOGIN_MSG 0x0001

using namespace std;

SqlHelper mySql; /* db operator */

void set_socket_non_blocking( int sockfd);

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void str_handle(int connfd)
{
	std::string type, name, pwd, nickname, picname;

    char buffer[MSGLEN] ;
    memset(buffer, 0, MSGLEN);
    int n = read( connfd, buffer , MSGLEN);

    Json::Value val;
    Json::Reader reader;

    if (reader.parse(buffer, val)) {
    	type = val["type"].asString();
    	name = val["name"].asString();
    	pwd = val["pwd"].asString();
    	nickname = val["nickname"].asString();
    	picname = val["picname"].asString();
    }

	Json::Value ret;
	Json::StyledWriter writer;

	std::string sqlStr = "";

	std::string ok = "ok";
	std::string failed = "error";

    if (type == "get") {
    	sqlStr += "select * from userinfo where ui_uname='";
    	sqlStr += name;
    	sqlStr += "' and ui_passwd='";
    	sqlStr += pwd;
    	sqlStr += "'";

    	vector<vector<string> > retv = mySql.SqlRead(sqlStr.c_str());
    	if (retv.size() > 0) {
    		ret["status"] = ok;
    		ret["name"] = name;
    		ret["pwd"] = pwd;
    		ret["nickname"] = retv[0][2];
    		ret["picname"] = retv[0][3];
    	} else {
    		ret["status"] = failed; 
    		ret["name"] = name;
    		ret["pwd"] = pwd;
    		ret["nickname"] = "";
    		ret["picname"] = ""; 
    	}
    } else if (type == "set") {
    	sqlStr = "update userinfo set ui_nickname='";
    	sqlStr += nickname;
    	sqlStr += "', ui_picpath='";
    	sqlStr += picname;
    	sqlStr += "' where ui_uname='";
    	sqlStr += name;
    	sqlStr += "'";

    	bool res = mySql.SqlWrite(sqlStr.c_str());

    	if (res == true) {
    		ret["status"] = ok;
    		ret["name"] = name;
    		ret["pwd"] = pwd;
    		ret["nickname"] = nickname;
    		ret["picname"] = picname;
    	} else {
    		ret["status"] = failed;
    		ret["name"] = name;
    		ret["pwd"] = pwd;
    		ret["nickname"] = nickname;
    		ret["picname"] = picname;
    	}
    }

	std::string sendbuffer = writer.write(ret);
    memset( buffer, 0, MSGLEN);
	strcpy(buffer, sendbuffer.c_str());

	write(connfd, buffer, MSGLEN);
}

int main()
{
    int listenfd, connfd, nfds, epollfd, n;
    struct sockaddr_in server_addr, client_addr;
    struct epoll_event ev, events[MAX_EVENT];
    socklen_t server_len = sizeof(server_addr);
    socklen_t client_len = sizeof(server_addr);

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) <0)
    {
        perror("init:");
        exit(1);
    }

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_family = AF_INET;

    if (bind(listenfd, (struct sockaddr *)&server_addr, server_len) <0)
    {
        perror("bind:");
        exit(1);
    }

    if (listen(listenfd, LISTEN_QUEUE) < 0)
    {
        perror("listen:");
        exit(1);
    }

    printf("listenning ...\n");

    if((epollfd = epoll_create(MAXEPOLL_SIZE)) < 0)
    {
        perror("epoll create:");
        exit(1);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listenfd;  

    if( epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) <0)
    {
        perror("epoll_ctl:");
        exit(1);
    }

    while(1)
    {
        if((nfds = epoll_wait(epollfd, events, MAX_EVENT, -1)) <0)
        {
            perror("epoll_wait:");
            exit(1);
        }

        for (n = 0; n < nfds; ++n)
        {
            if(events[n].data.fd == listenfd)
            {
                if((connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_len)) < 0)
                {
                    perror("accept:");
                    exit(1);
                }
                set_socket_non_blocking(connfd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = connfd;
                if( epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) ==-1)
                {
                    perror("epoll_ctl:connfd");
                    exit(1);
                }

            }
            else str_handle(events[n].data.fd);
        }

    }
    return 0;
}

void set_socket_non_blocking( int sockfd )
{
    int ret;
    ret = fcntl( sockfd, F_GETFL );
    if ( ret < 0 )
    {
        perror("fcntl get");
        exit(1);
    }

    ret = ret|O_NONBLOCK;

    if( fcntl( sockfd, F_SETFL, ret)<0)
    {
        perror("fcntl set");
        exit(1);
    }
}
