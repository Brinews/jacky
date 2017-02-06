#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <mysql/mysql.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "threadpool.h"

#define LISTEN_MAX 10000

typedef struct worker1
{
	char * buff;
	int fd;
}WORKER;

typedef struct msghead
{
	int len;
	int msgtype;
}MSGHEAD;

typedef struct regist//客人注册信息
{
	char name[20];
	char passwd[20];
	char tel[20];
	char born[20];
}REGI;

typedef struct film {
	char name[20];
	char cinema[20];
	char date[20];
	char price[8];
	char seat[32];
	char select[8]; // 选择的座位号:0-31
} FILM;

void my_result(int fd, int result, int msgtype)
{
	char buff[1024]; //发送数据 msghead + result
	MSGHEAD msghead;
	int len;//发送消息的总长度

	len = sizeof(msghead) + 4;
	msghead.len = len;
	msghead.msgtype = msgtype;

	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的buff
	result = htonl(result);
	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &result, sizeof(int));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{
		perror("write");
	}
	return;
}

void myregister(char * buff, int fd)
{
	REGI myRe;
	MYSQL db;
	MYSQL *pdb;
	int ret;

	memset(&myRe, 0, sizeof(myRe));
	memcpy(&myRe, buff + sizeof(MSGHEAD), sizeof(REGI));

	mysql_init(&db);
	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "china111", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect error\n");
		my_result(fd, 1, 2);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	char sql[1024] = {0};
sprintf(sql, "insert into account values(\'%s\', \'%s\',\'%s\',\'%s\');",myRe.name, myRe.passwd,myRe.tel,myRe.born);

	ret = mysql_query(pdb, sql);
	if(ret == 0)
	{
		printf("insert ok\n");
		my_result(fd, 0, 2);
	}
	else
	{
		printf("insert error: sql = %s\n", sql);
		my_result(fd, 1, 2);
	}

	mysql_close(pdb);

	return;


}

void mylogin(char * buff, int fd)
{
	REGI myRe;
	MYSQL db;
	MYSQL *pdb;
	int ret;

	memset(&myRe, 0, sizeof(myRe));
	memcpy(&myRe, buff + sizeof(MSGHEAD), sizeof(REGI));

	mysql_init(&db);
	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "china111", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect error\n");
		my_result(fd, 1, 4);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	char sql[1024] = {0};
	sprintf(sql, "select * from account where name = \'%s\' and passwd = \'%s\';", myRe.name, myRe.passwd);

	ret = mysql_query(pdb, sql);
	if(ret == 0)
	{
		printf("select ok\n");
		//my_result(fd, 0, 2);
	}
	else
	{
		printf("select  error: sql = %s\n", sql);
		my_result(fd, 1, 4);
	}

	MYSQL_RES* res;

	res = mysql_store_result(pdb);

	int row;

	row = mysql_num_rows(res);
	if(row == 0)
	{
		my_result(fd, 1, 4);
	}
	else
	{
		my_result(fd, 0, 4);
	}

	mysql_free_result(res);
	mysql_close(pdb);

	return;
}

void mySerch_result_nodata(int fd, int msgtype)
{
	MSGHEAD msghead;
	int len = sizeof(msghead);
	msghead.len = len;
	msghead.msgtype = msgtype;

	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	int ret;

	ret = write(fd, &msghead, len);
	if(ret < 0)
	{}
	return;
}

void mySerch_result(int fd, int msgtype, char *msg)
{
	char buff[1024] ={0};//发送消息
	MSGHEAD msghead;
	int len;//发送消息的总长度

	len = sizeof(MSGHEAD) + strlen(msg);

	msghead.len = len;
	msghead.msgtype = msgtype;

	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), msg, strlen(msg));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{}

	return;

}

void mySerchAccount(char * buff, int fd)
{
	MYSQL db;
	MYSQL *pdb;
	int ret;
	MSGHEAD msghead;


	mysql_init(&db);
	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "china111", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect error\n");
		mySerch_result_nodata(fd, 6);
		//my_result(fd, 1, 4);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	char sql[1024] = {0};
	sprintf(sql, "select * from account;");

	ret = mysql_query(pdb, sql);
	if(ret == 0)
	{
		printf("select ok\n");
		//my_result(fd, 0, 2);
	}
	else
	{
		printf("select  error: sql = %s\n", sql);
		mySerch_result_nodata(fd, 6);
		//my_result(fd, 1, 4);
	}

	MYSQL_RES* res;

	res = mysql_store_result(pdb);

	int row;

	row = mysql_num_rows(res);
	if(row == 0)
	{
		mySerch_result_nodata(fd, 6);
		return;
	}

	int col;
	col = mysql_num_fields(res);

	int i,j;
	char msg[1024] = {0};
	MYSQL_ROW result;
	for(i = 0; i < row; i++)
	{
		result = mysql_fetch_row(res);
		for(j = 0; j< col; j++)
		{
			sprintf(msg,"%s%20s", msg, result[j]);
		}
		sprintf(msg,"%s%s", msg, "\n");

	}

	mySerch_result(fd, 6, msg);

	mysql_free_result(res);
	mysql_close(pdb);

	return;
}

void myQueryFilm(char * buff, int fd)
{
	MYSQL db;
	MYSQL *pdb;
	
	FILM myRe;

	int ret;
	MSGHEAD msghead;

	memset(&myRe, 0, sizeof(myRe));
	memcpy(&myRe, buff+sizeof(MSGHEAD), sizeof(FILM));

	if (strcmp(myRe.name, "null") == 0 &&
			strcmp(myRe.cinema, "null") == 0) {
		// query invalid
		mySerch_result_nodata(fd, 8);
		return;
	}

	mysql_init(&db);
	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "china111", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect error\n");
		mySerch_result_nodata(fd, 8);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int pos = 0;
	char sql[1024] = {0};
	pos += sprintf(sql+pos, "select name,cinema,datetime,price from film where ");

	if (strcmp(myRe.name, "null") != 0 &&
			strcmp(myRe.cinema, "null") != 0) {
		pos += sprintf(sql+pos, " name = \'%s\' and cinema = \'%s\';",
				myRe.name, myRe.cinema);
	} else {
		if (strcmp(myRe.name, "null") != 0) {
			pos += sprintf(sql+pos, " name = \'%s\' ;", myRe.name);
		} else {
			pos += sprintf(sql+pos, " cinema = \'%s\';", myRe.cinema);
		}
	}

	ret = mysql_query(pdb, sql);
	if(ret == 0)
	{
		printf("select ok\n");
	}
	else
	{
		printf("select  error: sql = %s\n", sql);
		mySerch_result_nodata(fd, 8);
	}

	MYSQL_RES* res;

	res = mysql_store_result(pdb);

	int row;

	row = mysql_num_rows(res);
	if(row == 0)
	{
		mySerch_result_nodata(fd, 8);
		return;
	}

	int col;
	col = mysql_num_fields(res);

	int i,j;
	char msg[1024] = {0};
	MYSQL_ROW result;
	for(i = 0; i < row; i++)
	{
		result = mysql_fetch_row(res);
		for(j = 0; j< col; j++)
		{
			sprintf(msg,"%s%20s", msg, result[j]);
		}
		sprintf(msg,"%s%s", msg, "\n");
	}

	mySerch_result(fd, 8, msg);

	mysql_free_result(res);
	mysql_close(pdb);

	return;
}

void myQuerySeat(char * buff, int fd)
{
	MYSQL db;
	MYSQL *pdb;
	
	FILM myRe;

	int ret;
	MSGHEAD msghead;

	memset(&myRe, 0, sizeof(myRe));
	memcpy(&myRe, buff+sizeof(MSGHEAD), sizeof(FILM));

	mysql_init(&db);
	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "china111", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect error\n");
		mySerch_result_nodata(fd, 10);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	int pos = 0;
	char sql[1024] = {0};
	pos += sprintf(sql+pos, "select name,cinema,datetime,price,seat from film where ");

	pos += sprintf(sql+pos, " name = \'%s\' and cinema = \'%s\' and datetime = \'%s\' ;",
			myRe.name, myRe.cinema, myRe.date);

	ret = mysql_query(pdb, sql);
	if(ret == 0)
	{
		printf("select ok\n");
	}
	else
	{
		printf("select  error: sql = %s\n", sql);
		mySerch_result_nodata(fd, 10);
	}

	MYSQL_RES* res;

	res = mysql_store_result(pdb);

	int row;

	row = mysql_num_rows(res);
	if(row == 0)
	{
		mySerch_result_nodata(fd, 10);
		return;
	}

	int col;
	col = mysql_num_fields(res);

	int i,j;
	char msg[1024] = {0};
	MYSQL_ROW result;
	for(i = 0; i < row; i++)
	{
		result = mysql_fetch_row(res);
		for(j = 0; j< col-1; j++)
		{
			sprintf(msg,"%s%20s", msg, result[j]);
		}

		sprintf(msg, "%s%32s", msg, result[j]);
		sprintf(msg,"%s%s", msg, "\n");
	}

	mySerch_result(fd, 10, msg);

	mysql_free_result(res);
	mysql_close(pdb);

	return;
}

void myGetSeat(char * buff, int fd)
{
	int pos = 0;
	FILM myRe;

	MYSQL db;
	MYSQL *pdb;
	int ret;

	memset(&myRe, 0, sizeof(myRe));
	memcpy(&myRe, buff + sizeof(MSGHEAD), sizeof(FILM));

	mysql_init(&db);
	pdb = mysql_real_connect(&db, "127.0.0.1", "root", "123", "china111", 3306, NULL, 0);
	if(pdb == NULL)
	{
		printf("mysql connect error\n");
		my_result(fd, 1, 12);
		return;
	}
	else
	{
		printf("mysql connect ok\n");
	}

	sscanf(myRe.select, "%d", &pos);

	char sql[1024] = {0};
	sprintf(sql, "update film set seat=concat(substring(seat, 1, %d), '1', substring(seat, %d, %d)) where name=\'%s\' and cinema=\'%s\' and datetime=\'%s\' and substring(seat, %d, 1) =\'0\';", pos-1, pos+1, 32-pos, myRe.name, myRe.cinema,myRe.date, pos);

	//printf("update sql = %s\n", sql);

	ret = mysql_query(pdb, sql);
	if (ret == 0)
	{
		if (mysql_affected_rows(pdb) > 0) {
			printf("update ok\n");
			my_result(fd, 0, 12);
		} else {
			my_result(fd, 1, 12);
		}
	}
	else
	{
		printf("update error: sql = %s\n", sql);
		my_result(fd, 1, 12);
	}

	mysql_close(pdb);

	return;
}

void * handlerWork(void * arg)
{
	WORKER *pw = (WORKER *)arg;

	MSGHEAD msghead;
	char * buffer = pw->buff;
	int sockFd = pw->fd;

	memset(&msghead, 0, sizeof(msghead));
	memcpy(&msghead, buffer, sizeof(msghead));
	//转换字节序
	msghead.len = ntohl(msghead.len);
	msghead.msgtype = ntohl(msghead.msgtype);

	if(msghead.msgtype == 1)//注册
	{
		myregister(buffer, sockFd);
	}
	else if(msghead.msgtype == 3)//登录
	{
		mylogin(buffer, sockFd);
	}
	else if(msghead.msgtype == 5)//查询account
	{
		mySerchAccount(buffer, sockFd);
	}
	else if (msghead.msgtype == 7) //查询电影信息
	{
		myQueryFilm(buffer, sockFd);
	}
	else if (msghead.msgtype == 9) // 查询座位信息
	{
		myQuerySeat(buffer, sockFd);
	}
	else if (msghead.msgtype == 11) //下单信息
	{
		myGetSeat(buffer, sockFd);
	}

	free(buffer);
	free(arg);
	return NULL;
}

int main(int argc, char** argv)
{
	int ret;
	int epFd;//epoll的描述符
	int listenFd;//监听第一次链接的FD
	struct epoll_event ev, events[20];//ev 注册事件使用的结构体
	//events 通知事件的数组

	if(argc != 2)
	{
		printf("para error\n");
		return -1;
	}

	pool_init(5);//线程池的初始化

	listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenFd < 0)
	{
		perror("socket");
		return -1;
	}
	epFd = epoll_create(LISTEN_MAX);//LISTEN_MAX限制监听连接数的上限；
	if(epFd < 0)
	{
		perror("epoll_create");
		return -1;
	}
	ev.events = EPOLLIN | EPOLLET;//事件属性
	ev.data.fd = listenFd;//事件描述符

	epoll_ctl(epFd, EPOLL_CTL_ADD, listenFd, &ev);

	struct sockaddr_in serverAddr, clientAddr;
	int cliLen = sizeof(clientAddr);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(listenFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		perror("bind");
		return -1;
	}

	ret = listen(listenFd, LISTEN_MAX);
	if(ret < 0)
	{
		perror("listen");
		return -1;
	}

	int nfds;//保存通知事件的个数
	int i;
	int clientFd;
	char buffer[1024];
	int sockFd;
	int fdCli[20] = {0};
	int j;
	MSGHEAD msghead;
	REGI myRe;

	for(; ;)
	{
		nfds = epoll_wait(epFd, events, 20, 500);
		for(i = 0; i < nfds; i++)//遍历整个通知队列
		{
			if(events[i].data.fd == listenFd)//第一次链接
			{
				clientFd = accept(listenFd, (struct sockaddr *)&clientAddr, 
					(socklen_t*)&cliLen);

				char buff[48];
				inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, buff, sizeof(buff));
				printf("connnect ok:clinet ip = %s port =%d\n", buff, ntohs(clientAddr.sin_port));

				ev.events = EPOLLIN | EPOLLET;//ET边缘触发
				ev.data.fd = clientFd;
				epoll_ctl(epFd, EPOLL_CTL_ADD, clientFd, &ev);

			}
			else if(events[i].events & EPOLLIN)//socket有数据可以读取
			{

				memset(buffer, 0, 1024);
				sockFd = events[i].data.fd; 
				ret = read(sockFd, buffer, 1024);
				if(ret < 0)
				{
					perror("read");

					ev.data.fd = sockFd;
					epoll_ctl(epFd, EPOLL_CTL_DEL, sockFd, &ev);
					continue;
				}
				else if(ret == 0)
				{
					ev.data.fd = sockFd;
					epoll_ctl(epFd, EPOLL_CTL_DEL, sockFd, &ev);
					continue;
				}
			    
				WORKER *pw = (WORKER *)malloc(sizeof(WORKER));
				char *buf1 = (char *)malloc(ret);
                pw->buff = buf1;
				pw->fd = sockFd;

				memcpy(buf1, buffer, ret);
				pool_add_worker(handlerWork, pw);

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = sockFd;
				epoll_ctl(epFd, EPOLL_CTL_MOD, sockFd, &ev);

			}
			else if(events[i].events & EPOLLOUT)//写通知
			{
				sockFd = events[i].data.fd;
				ret = write(sockFd,"hello", 5);

				ev.events = EPOLLOUT | EPOLLET;
				ev.data.fd = sockFd;
				epoll_ctl(epFd, EPOLL_CTL_MOD, sockFd, &ev);
			}
		}
	}

	pool_destroy();
}
