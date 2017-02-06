#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>

//消息类型 1.注册  2.注册的返回消息 3.登录 4.登录返回消息  5.查询account
// 	       6.查询account返回信息 7. 电影信息查询 8. 电影信息查询返回
//         9.座位信息查询 10. 座位信息查询返回
//         11.下单购票 12.下单购票返回
typedef struct msghead
{
	int len;//发送信息的总长度
	int msgtype;//消息类型
}MSGHEAD;

typedef struct regi
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
	char select[8]; // 选择的座位号:1-32
} FILM;

int flag = 0;// 控制菜单的打印

void regist(int fd)
{
	REGI myRe;
	MSGHEAD msghead;
	int len;
	char buff[1024] = {0};//发送数据  MSGHEAD + REGI

	memset(&myRe, 0, sizeof(myRe));
	memset(&msghead, 0, sizeof(msghead));

	printf("please input a name:");
	scanf("%s", myRe.name);
	printf("please input a passwd:");
	scanf("%s",myRe.passwd);
	printf("please input a tel num:");
	scanf("%s",myRe.tel);
	printf("please input your born according to year-mooth-day:");
	scanf("%s",myRe.born);

	len = sizeof(MSGHEAD) + sizeof(REGI);//发送消息的总长度
	msghead.len = len;
	msghead.msgtype = 1;
	//转换字节序
	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);
	//准备发送数据的buff
	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &myRe, sizeof(myRe));

	int ret;

	ret = write(fd, buff, len);//发送数据
	if(ret < 0)
	{
		perror("write");
		return;
	}
}

void register_result(char * buff)//msghead + result(int)
{
	int result;
	memcpy(&result, buff+sizeof(MSGHEAD), 4);
	result = ntohl(result);
	if(result == 0)
	{
		printf("register ok\n");
	}
	else if(result == 1)
	{
		printf("register fault\n");
	}

	return;
}

void login_result(char * buff)//msghead + result(int)
{
	int result;
	memcpy(&result, buff+sizeof(MSGHEAD), 4);
	result = ntohl(result);
	if(result == 0)
	{
		printf("login ok\n");
		flag = 1;
	}
	else if(result == 1)
	{
		printf("login fault\n");
		flag = -1;
	}

	return;
}

void serch_account_result(char * buff)
{
	//printf("")列名
	MSGHEAD msghead;

	memcpy(&msghead, buff, sizeof(msghead));
	msghead.len = ntohl(msghead.len);

	if(msghead.len == sizeof(msghead))
	{
		printf("no data\n");
		return;
	}
	printf("%s\n", buff + sizeof(MSGHEAD));

	return;
}

void query_film_result(char * buff)
{
	printf("Film Name\tCinema\tDate\tPrice\n"); //列名
	MSGHEAD msghead;

	memcpy(&msghead, buff, sizeof(msghead));
	msghead.len = ntohl(msghead.len);

	if(msghead.len == sizeof(msghead))
	{
		printf("no qualified film information\n");
		return;
	}

	printf("%s\n", buff + sizeof(MSGHEAD));

	return;
}

void query_seat_result(char * buff)
{
	printf("Film Name\tCinema\tDate\tPrice\tSeat\n"); //列名
	MSGHEAD msghead;

	memcpy(&msghead, buff, sizeof(msghead));
	msghead.len = ntohl(msghead.len);

	if(msghead.len == sizeof(msghead))
	{
		printf("no qualified film seat information\n");
		return;
	}

	printf("%s\n", buff + sizeof(MSGHEAD));

	return;
}

void get_seat_result(char * buff)//msghead + result(int)
{
	int result;
	memcpy(&result, buff+sizeof(MSGHEAD), 4);
	result = ntohl(result);

	if(result == 0)
	{
		printf("Seat Reserved ok\n");
		flag = 1;
	}
	else if(result == 1)
	{
		printf("Seat Reserved failed\n");
		flag = -1;
	}

	return;
}


void *handler(void * arg)
{
	int *p = (int *)arg;
	int fd = *p;

	int ret;
	MSGHEAD msghead;
	char name[20];
	char buff[1024];
	while (1)
	{
		//20 + buf1
		memset(buff, 0, 1024);
		ret = read(fd, buff, 1024);
		if(ret < 0)
		{
			perror("read");
			return NULL;
		}
		else if(ret == 0)
		{
			printf("close fd\n");
			return NULL;
		}
		else
		{
			memset(&msghead, 0, sizeof(msghead));
			memcpy(&msghead, buff, sizeof(msghead));
			//字节序转换
			msghead.len = ntohl(msghead.len);
			msghead.msgtype = ntohl(msghead.msgtype);

			if(msghead.msgtype == 2)//注册的返回消息
			{
				register_result(buff);
			}
			else if(msghead.msgtype == 4)//登录的返回消息
			{
				login_result(buff);
			}
			else if(msghead.msgtype == 6)//查询account表的返回消息
			{
				serch_account_result(buff);
				flag = 1;
			} 
			else if (msghead.msgtype == 8) // 查询电影信息的返回消息
			{
				query_film_result(buff);
				flag = 1;
			}
			else if (msghead.msgtype == 10) // 查询座位信息的返回信息
			{
				query_seat_result(buff);
				flag = 1;
			}
			else if (msghead.msgtype == 12) // 下单的返回信息
			{
				get_seat_result(buff);
				flag = 1;
			}
		}
	}

	return NULL;
}

void login(int fd)
{
	char buff[1024];//发送数据
	MSGHEAD msghead;
	REGI myRe;

	memset(&msghead, 0, sizeof(msghead));
	memset(&myRe, 0, sizeof(myRe));

	printf("please input a name:");
	scanf("%s", myRe.name);
	printf("please input a passwd:");
	scanf("%s", myRe.passwd);
	int len;
	len = sizeof(msghead) + sizeof(myRe);//发送数据的总长度

	msghead.len = len;
	msghead.msgtype = 3;// 登录消息

	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的buff
	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &myRe, sizeof(myRe));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{}

	return;
}

void query_film(int fd)
{
	char buff[1024];//发送数据
	MSGHEAD msghead;
	FILM myRe;

	memset(&msghead, 0, sizeof(msghead));
	memset(&myRe, 0, sizeof(myRe));

	printf("please input the film name or 'null':");
	//scanf("%s", myRe.name);
	fgets(myRe.name, 20-1, stdin);
	myRe.name[strlen(myRe.name)-1] = '\0';

	printf("please input the cinema name or 'null':");
	//scanf("%s", myRe.cinema);
	fgets(myRe.cinema, 20-1, stdin);
	myRe.cinema[strlen(myRe.cinema)-1] = '\0';

	int len;
	len = sizeof(msghead) + sizeof(myRe);//发送数据的总长度

	msghead.len = len;
	msghead.msgtype = 7;// 查询电影信息消息

	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的buff
	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &myRe, sizeof(myRe));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{}

	return;
}

void query_seat(int fd)
{
	char buff[1024];//发送数据
	MSGHEAD msghead;
	FILM myRe;

	memset(&msghead, 0, sizeof(msghead));
	memset(&myRe, 0, sizeof(myRe));

	printf("please input the film name:");
	//scanf("%s", myRe.name);
	fgets(myRe.name, 20-1, stdin);
	myRe.name[strlen(myRe.name)-1] = '\0';

	printf("please input the cinema name:");
	//scanf("%s", myRe.cinema);
	fgets(myRe.cinema, 20-1, stdin);
	myRe.cinema[strlen(myRe.cinema)-1] = '\0';

	printf("please input the film date:");
	//scanf("%s", myRe.date);
	fgets(myRe.date, 20-1, stdin);
	myRe.date[strlen(myRe.date)-1] = '\0';

	int len;
	len = sizeof(msghead) + sizeof(myRe);//发送数据的总长度

	msghead.len = len;
	msghead.msgtype = 9;// 查询电影座位消息

	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的buff
	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &myRe, sizeof(myRe));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{}

	return;
}

void get_seat(int fd)
{
	char buff[1024];//发送数据
	MSGHEAD msghead;
	FILM myRe;
	int seatno;

	memset(&msghead, 0, sizeof(msghead));
	memset(&myRe, 0, sizeof(myRe));

	printf("please input the film name:");
	//scanf("%s", myRe.name);
	fgets(myRe.name, 20-1, stdin);
	myRe.name[strlen(myRe.name)-1] = '\0';

	printf("please input the cinema name:");
	//scanf("%s", myRe.cinema);
	fgets(myRe.cinema, 20-1, stdin);
	myRe.cinema[strlen(myRe.cinema)-1] = '\0';

	printf("please input the film date:");
	//scanf("%s", myRe.date);
	fgets(myRe.date, 20-1, stdin);
	myRe.date[strlen(myRe.date)-1] = '\0';

	do {
		printf("please input the seat no(1-32):");
		fgets(myRe.select, 3, stdin);
		myRe.select[strlen(myRe.select)-1] = '\0';
		sscanf(myRe.select, "%d", &seatno);
	} while (seatno < 0 || seatno > 31);

	int len;
	len = sizeof(msghead) + sizeof(myRe);//发送数据的总长度

	msghead.len = len;
	msghead.msgtype = 11;// 下单消息

	msghead.len = htonl(msghead.len);
	msghead.msgtype = htonl(msghead.msgtype);

	//组装发送的buff
	memcpy(buff, &msghead, sizeof(msghead));
	memcpy(buff + sizeof(msghead), &myRe, sizeof(myRe));

	int ret;

	ret = write(fd, buff, len);
	if(ret < 0)
	{}

	return;
}

void printf_menu1()
{
	printf("1.register\n");
	printf("2.login\n");
	printf("0.exit\n");
	return;
}

void printf_menu2()
{
	printf("1.select account\n");
	printf("2.query film\n");
	printf("3.query seats\n");
	printf("4.get a seat\n");
	printf("0.exit\n");

	return;
}

void serch_account(int fd)//消息类型 5
{
	char buff[1024] = {0};//发送消息的buff
	MSGHEAD msghead;

	int len = sizeof(msghead);//发送数据的总长度
	msghead.len = len;
	msghead.msgtype = 5;

	msghead.len = htonl(len);
	msghead.msgtype = htonl(msghead.msgtype);

	int ret;

	memcpy(buff, &msghead, len);
	ret = write(fd, buff, len);
	if(ret < 0)
	{}

	return;
}

int main(int argc, char **argv)
{
	int fd;
	int ret;
	struct sockaddr_in serverAddr;

	if(argc != 3)
	{
		printf("para erro\n");
		return -1;
	}

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0)
	{
		perror("socket");
		return -1;
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family  =  AF_INET;
	serverAddr.sin_port = htons(atoi(argv[2]));  //"8080" -->8080
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]); //"127.0.0.1"

	ret = connect(fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		perror("connet");
		return -1;
	}

	pthread_t pid;
	ret = pthread_create(&pid, NULL, handler, &fd);

	char buf1[1000];//从键盘接收数据
	char buff[1024];//发送数据
	int num;
	while(1)//发送数据
	{
		printf_menu1();
		scanf("%d", &num);
		if(num == 1)//注册
		{
			regist(fd);
		}
		else if(num == 2)//登录
		{
			login(fd);
			while(flag == 0);

			if(flag == 1)//登录成功
			{
				break;
			}

			if(flag == -1)//登录失败
			{
				flag = 0;
				continue;
			}
		}
		else //退出
		{
			pthread_cancel(pid);
			close(fd);
			//pthread_join(pid, NULL);
			return 0;
		}
	}

	getchar();

	flag = 0;
	while(1) // login success
	{
		flag = 0;
		printf_menu2();

		char buf[10];
		fgets(buf, 9, stdin);

		sscanf(buf, "%d", &num);

		if(num == 1) //查询
		{
			serch_account(fd);
			while(flag == 0);
		} else if (num == 0) {
			// quit
			break;
		} else if (num == 2) {
			// query film
			query_film(fd);
			while (flag == 0);
		} else if (num == 3) {
			// query seat
			query_seat(fd);
			while (flag == 0);
		} else if (num == 4) {
			// get ticket
			get_seat(fd);
			while (flag == 0);
		}
	}

	pthread_cancel(pid);
	close(fd);
	//pthread_join(pid, NULL);

	return 0;

}
