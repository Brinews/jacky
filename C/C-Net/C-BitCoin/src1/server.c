#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  

#include <fcntl.h>
#include <time.h>
#include <pthread.h>

#include "uint256.h"
#include "sha256.h"

#include <inttypes.h>

#define BACKLOG 100    // how many pending connections queue will hold  
#define BUF_SIZE 256  

#ifdef DEBUG
#define debug	printf
#else
#define debug
#endif

int fd_clients[BACKLOG];    // accepted connection fd  
int conn_amount;      // current connection amount  
char client_ip[BACKLOG][16];

char *logfile = "log.txt";
int logfd;

typedef struct soln {
	int fd;
	char params[256];
} soln_t;

/*
 * work queue
 */
#define QSIZE	10

typedef struct job {
	int fd;
	char params[256];
}job_t;

job_t jobqueue[QSIZE];
job_t *now;
int head, tail;

pthread_mutex_t m_mutex;
pthread_cond_t m_condv;
pthread_cond_t m_condv1;

int is_full();
void add_work(job_t item);
job_t poll_work();
void* process_work_thread(void *arg);

void logmsg(int fd, int sfd, char *ip, char *msg);
void handle_request(int fd, char *msg, char *clientip);
void *verify_soln(void *args);
void *search_soln(void *args);

BYTE hex_to_byte(unsigned char c)
{
	BYTE ret = 0;
	if (c >= 'a') {
		ret = c - 'a' + 10;
	} else {
		ret = c - '0';
	}

	return ret;
}

int proof_of_work(char *diff, char *seed, char *nonce)
{
	/* calculate target */
	BYTE base[32];
	BYTE temp[32];
	BYTE beta[32];
	BYTE target[32];

	uint32_t difficulty;
	uint32_t alpha;

	debug("diff=%s\n", diff);
	sscanf(diff, "%x", &difficulty);

	uint256_init(target);
	uint256_init(base);
	uint256_init(temp);
	uint256_init(beta);

	base[31] = 0x2;

	alpha = difficulty >> 24;

	beta[31] = difficulty & 0xFF;
	beta[30] = (difficulty>>8) & 0xFF;
	beta[29] = (difficulty>>16) & 0xFF;

	debug("0x%x\n", alpha);

	uint256_exp(temp, base, 8*(alpha-3));
	uint256_mul(target, temp, beta);

	/* calculate the hash of x|nonce */
	BYTE y[32];
	uint256_init(y);

	BYTE x[40+1] = {0};

	int i, idx = 0;
	for (i = 0; i < strlen(seed); i += 2) {
		BYTE b = hex_to_byte(seed[i]);
		BYTE c = hex_to_byte(seed[i+1]);
		x[idx++] = b*16+c;
	}

	for (i = 0; i < strlen(nonce); i += 2) {
		BYTE b = hex_to_byte(nonce[i]);
		BYTE c = hex_to_byte(nonce[i+1]);
		x[idx++] = b*16+c;
	}

	debug("s=%s\n", seed);
	debug("n=%s\n", nonce);

	for (i = 0; i < idx; i++) {
		debug("%02x", x[i]);
	}
	debug("\n");

	BYTE res[SHA256_BLOCK_SIZE];

	/* hash function */
	SHA256_CTX ctx;
	sha256_init(&ctx);
	sha256_update(&ctx, x, 40);
	sha256_final(&ctx, res);

	debug("len(x)=%d, len(res)=%d\n", idx, strlen(res));

	sha256_init(&ctx);
	sha256_update(&ctx, res, 32);
	sha256_final(&ctx, y);

	return sha256_compare(target, y);
}

void acc_hex_str(char *nonce)
{
	int i;
	int acc = 1;

	for (i = strlen(nonce)-1; i >= 0; i--) {
		if (nonce[i] == '9') {
			nonce[i] = 'a';
			acc = 0;
		}
		else if (nonce[i] == 'f') {
			nonce[i] = '0';
			acc = 1;
		} else {
			nonce[i] = nonce[i] + acc;
			acc = 0;
		}

		if (acc == 0) break;
	}
}

void *search_soln(void *args)
{
	char buffer[BUF_SIZE];
	bzero(buffer, BUF_SIZE);

	soln_t *sn = (soln_t *)args;

	char *diff, *seed, *nonce;
	diff = sn->params;
	
	seed = diff;
	while (*seed != ' ' && *seed != '\0' 
			&& *seed != '\r' && *seed != '\n') {
		seed++;
	}
	*seed = '\0';
	do {
		seed++;
	} while (*seed == ' ');

	nonce = seed;

	while (*nonce != ' ' && *nonce != '\0' 
			&& *nonce != '\r' && *nonce != '\n') {
		nonce++;
	}
	*nonce = '\0';
	do {
		nonce++;
	} while (*nonce == ' ');

	char *tail = nonce;
	while (*tail != '\0' && *tail != '\r'
			&& *tail != '\n' && *tail != ' ') {
		tail++;
	}
	*tail = '\0';
	do {
		tail++;
	} while (*nonce == ' ');

	int num = 0;
	sscanf(tail, "%d", &num);
	debug("num=%d\n", num);

	char nonce_str[17]= {0};
	int found = 0;

	strncpy(nonce_str, nonce, 16);
	debug("non=%s\n", nonce_str);

	do {
		debug("start=%s\n", nonce);
		if (proof_of_work(diff, seed, nonce) > 0) {
			found = 1;
			break;
		}
		acc_hex_str(nonce);
	} while (strcmp(nonce, nonce_str) != 0);

	if (found == 0) { /* no found */
		strcpy(buffer, "ERRO no solution can be found\r\n");
	} else {
		sprintf(buffer, "SOLN %s %s %s\r\n", diff, seed, nonce);
	}

	write(sn->fd, buffer, strlen(buffer));
	logmsg(logfd, sn->fd, "0.0.0.0", buffer);
}

void *verify_soln(void *args)
{
	char buffer[BUF_SIZE];
	bzero(buffer, BUF_SIZE);

	soln_t *sn = (soln_t *)args;
	char *diff, *seed, *nonce;
	diff = sn->params;
	
	seed = diff;
	while (*seed != ' ' && *seed != '\0' 
			&& *seed != '\r' && *seed != '\n') {
		seed++;
	}
	*seed = '\0';
	do {
		seed++;
	} while (*seed == ' ');

	nonce = seed;

	while (*nonce != ' ' && *nonce != '\0' 
			&& *nonce != '\r' && *nonce != '\n') {
		nonce++;
	}
	*nonce = '\0';
	do {
		nonce++;
	} while (*nonce == ' ');

	char *tail = nonce;
	while (*tail != '\0' && *tail != '\r'
			&& *tail != '\n') {
		tail++;
	}
	*tail = '\0';

	int res = proof_of_work(diff, seed, nonce);

	if (res < 0) {
		strcpy(buffer, "OKAY\r\n");
	} else if (res == 0) {
		strcpy(buffer, "ERRO H(H(x)) == y\r\n");
	} else {
		strcpy(buffer, "ERRO H(H(x)) > y\r\n");
	}

	write(sn->fd, buffer, strlen(buffer));
	logmsg(logfd, sn->fd, "0.0.0.0", buffer);
}

void logmsg(int fd, int sfd, char *ip, char *msg)
{
	char logbuf[BUF_SIZE];
	time_t now;
	time(&now);

	char timebuf[128];
	strcpy(timebuf, ctime(&now));
	timebuf[strlen(timebuf)-1] = '\0';

	sprintf(logbuf, "%26s %16s [%2d]: %s", timebuf, ip, sfd, msg);

	write(fd, logbuf, strlen(logbuf));
}


void handle_request(int fd, char *msg, char *ip)
{
	char buffer[BUF_SIZE];
	bzero(buffer, BUF_SIZE);

	if (strncmp(msg, "PING", 4) == 0) {
		strcpy(buffer, "PONG\r\n");
		write(fd, buffer, strlen(buffer));
	} else if (strncmp(msg, "PONG", 4) == 0) {
		strcpy(buffer, "ERRO\r\n");
		write(fd, buffer, strlen(buffer));
	} else if (strncmp(msg, "OKAY", 4) == 0) {
		strcpy(buffer, "ERRO\r\n");
		write(fd, buffer, strlen(buffer));
	} else if (strncmp(msg, "ERRO ", 5) == 0) {
		strcpy(buffer, "ERRO\r\n");
		write(fd, buffer, strlen(buffer));
	} else if (strncmp(msg, "SOLN ", 5) == 0) {
		soln_t arg;
		arg.fd = fd;
		strncpy(arg.params, msg+5, 255);
		pthread_t pid;
		pthread_create(&pid, NULL, &verify_soln, (void *)&arg);
		//verify_soln((void *)&arg);
		return;
	} else if (strncmp(msg, "WORK ", 5) == 0) {
		soln_t arg;
		arg.fd = fd;
		strncpy(arg.params, msg+5, 255);
		job_t item;
		item.fd = fd;
		strncpy(item.params, msg+5, 255);
		if (!is_full()) {
			add_work(item);
		} else {
			strcpy(buffer, "ERRO pending jobs full\r\n");
			write(fd, buffer, strlen(buffer));
		}
		//pthread_t pid;
		//pthread_create(&pid, NULL, &search_soln, (void *)&arg);
		return;
		//search_soln((void*)&arg);
	} else {
		strcpy(buffer, msg);
		write(fd, buffer, strlen(buffer));
	}

	logmsg(logfd, fd, ip, buffer);
}

void showclient()  
{  
	int i;  
	printf("client amount: %d\n", conn_amount);
	for (i = 0; i < BACKLOG; i++) {
		if (fd_clients[i] > 0) {
			printf("[%d]:%d  ", i, fd_clients[i]);
		}
	}
	printf("\n\n");
}  

int is_full()
{
	int full = 0;
	pthread_mutex_lock(&m_mutex);
	if ((tail+1)%QSIZE == head) full = 1;
	pthread_mutex_unlock(&m_mutex);

	return full;
}

void add_work(job_t item)
{
	pthread_mutex_lock(&m_mutex);
	/*
	while ((tail+1)%QSIZE == head) {
		pthread_cond_wait(&m_condv1, &mutex);
	}
	*/
	tail = (tail+1)%QSIZE;
	jobqueue[tail++] = item;
	pthread_cond_signal(&m_condv);
	pthread_mutex_unlock(&m_mutex);
}

job_t poll_work()
{
	pthread_mutex_lock(&m_mutex);
	while (head == tail) {
		pthread_cond_wait(&m_condv, &m_mutex);
	}
	job_t job = jobqueue[head];
	head = (head+1)%QSIZE;
	//pthread_cond_signal(&m_condv1);
	pthread_mutex_unlock(&m_mutex);

	return job;
}

void* process_work_thread(void *arg)
{
	while (1) {
		job_t cur = poll_work();
		search_soln((void*)&cur);
	}
}

int main(int argc, char **argv)  
{  
	// listen on sock_fd, new connection on new_fd  
	int sock_fd, new_fd;             
	// server address information  
	struct sockaddr_in server_addr;  
	// connector's address information  
	struct sockaddr_in client_addr;  

	socklen_t sin_size;  
	int yes = 1;  
	char buf[BUF_SIZE];  
	int ret;  
	int i, j;  

	int port = 12345; //default port number
	if (argc > 1) {
		port = atoi(argv[1]);
	}

	head = tail = 0;
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_condv, NULL);
	pthread_cond_init(&m_condv1, NULL);

	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
		perror("socket");  
		exit(1);  
	}  

	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, 
				&yes, sizeof(int)) == -1) {  
		perror("setsockopt");
		exit(1);
	}  

	server_addr.sin_family = AF_INET;         // host byte order  
	server_addr.sin_port = htons(port);     // short, network byte order  
	server_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP  

	memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));  

	if (bind(sock_fd, (struct sockaddr *)&server_addr, 
				sizeof(server_addr)) == -1) {  
		perror("bind");  
		exit(1);  
	}  

	if (listen(sock_fd, BACKLOG) == -1) {  
		perror("listen");  
		exit(1);  
	}  

	printf("Listen port %d\n", port);  

	logfd = open(logfile, O_RDWR | O_CREAT | O_APPEND);
	if (logfd < 0) {
		perror("open log file");
	}

	fd_set fdsr;  
	int maxsock;  
	struct timeval tv;  

	conn_amount = 0;  
	sin_size = sizeof(client_addr);  
	maxsock = sock_fd;  

	memset(fd_clients, 0, sizeof(fd_clients));

	pthread_t pid;
	pthread_create(&pid, NULL, &process_work_thread, NULL);

	while (1) {  
		// initialize file descriptor set  
		FD_ZERO(&fdsr);  
		FD_SET(sock_fd, &fdsr);  // add fd  

		maxsock = sock_fd;

		// timeout setting  
		tv.tv_sec = 30;  
		tv.tv_usec = 0;  
		// add active connection to fd set  
		for (i = 0; i < BACKLOG; i++) {  
			if (fd_clients[i] != 0) {  
				FD_SET(fd_clients[i], &fdsr);  
				if (fd_clients[i] > maxsock) {
					maxsock = fd_clients[i];
				}
			}  
		}  

		ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);  

		if (ret < 0) {          // error  
			perror("select");  
			break;  
		} else if (ret == 0) {  // time out  
			debug("timeout\n");  
			continue;  
		}  

		// check every fd in the set  
		for (i = 0; i < BACKLOG; i++) {  
			// check which fd is ready  
			if (fd_clients[i] != 0 && FD_ISSET(fd_clients[i], &fdsr)){  
				ret = recv(fd_clients[i], buf, sizeof(buf), 0);  

				if (ret <= 0) {
					// client close  
					debug("END : %d and client[%d] close\n", ret, i);  
					close(fd_clients[i]);  
					FD_CLR(fd_clients[i], &fdsr);  // delete fd   
					fd_clients[i] = 0;  
					conn_amount--;  
				} else { // receive data  
					if (ret < BUF_SIZE) 
						memset(&buf[ret], '\0', 1); // add NULL('/0')  

					debug("client[%d] send:%s\n", i, buf);  
					logmsg(logfd, fd_clients[i], client_ip[i], buf);

					handle_request(fd_clients[i], buf, "0.0.0.0");

					if (strcmp(buf, "exit") == 0) {
					}
				}
			}
		}

		// check whether a new connection comes  
		if (FD_ISSET(sock_fd, &fdsr))  // accept new connection   
		{  
			new_fd = accept(sock_fd, 
					(struct sockaddr *)&client_addr, &sin_size);  
			if (new_fd <= 0) {  
				perror("accept");  
				continue;  
			}  

			// add to fd queue  
			if (conn_amount < BACKLOG)   
			{  
				for (j = 0; j < BACKLOG; j++) {
					if (fd_clients[j] == 0) {
						fd_clients[j] = new_fd;

						debug("new connection client[%d] %s:%d\n", j,  
								inet_ntoa(client_addr.sin_addr), 
								ntohs(client_addr.sin_port));  

						strncpy(client_ip[j], inet_ntoa(client_addr.sin_addr), 16);

						if (new_fd > maxsock) {
							// update the maxsock fd for select function  
							maxsock = new_fd;  
						}

						conn_amount++;
						break;
					}
				}
			}  
			else   
			{  
				printf("max connections arrive, exit\n");  
				send(new_fd, "bye", 4, 0);  
				close(new_fd);  
			}
		}

		//showclient();  
	}  

	// close other connections  
	for (i = 0; i < BACKLOG; i++)
	{
		if (fd_clients[i] != 0)
		{
			close(fd_clients[i]);
		}
	}

	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_condv);
	pthread_cond_destroy(&m_condv1);

	close(logfd);

	return 0;
}

