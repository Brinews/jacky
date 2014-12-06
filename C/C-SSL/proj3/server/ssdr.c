#include "ssdr.h"
#include <fcntl.h>

#define IDSIZE 11

#define CERTF	"srv.crt"
#define	KEYF	"srv.key"
#define CACERT	"CA.crt"
#define	REPODIR	"./repo/"
#define TOKDIR	"./token/"

#define	PORT	9000
#define BUFLEN	1024

#define dprintf	printf

token* tokens;

extern int putting;
extern int totalLen;

/* return: file size, -1 means error */
int recvfile_from_client(char *filename, int sockfd)
{
	int fd = open(filename, O_RDWR|O_CREAT|O_TRUNC, 0666);

	if (fd < 0) return -1;

	int recvlen = 0, rlen, wlen;
	char buf[BUFLEN];

	while (1) {

		memset(buf, 0, BUFLEN);

		do {
			rlen = recv(sockfd, buf, BUFLEN, 0);
		} while (rlen < 0);

		if (rlen == 0) break; /* read nothing */

		wlen = write(fd, buf, rlen);

		recvlen += rlen;

		if (rlen < BUFLEN) break; /* read over */
	}

	dprintf("write file %s bytes %d\n", filename, recvlen);
	close(fd);

	return recvlen;
}

int get_repofile_uid(char *filename)
{
	int uid = -1;
	char idbuf[IDSIZE] = {0};
	struct stat filest;

	char *pathfile = (char*) malloc(strlen(REPODIR) + strlen(filename) + 1);

	sprintf(pathfile, REPODIR"%s", filename);

	/* to delegate file exists */
	if (stat(pathfile, &filest) >= 0) {
		FILE* fp = fopen(pathfile, "rb");
		if (fp != NULL) {
			fread(idbuf, IDSIZE, 1, fp);
			fclose(fp);

			uid = atoi(idbuf);
		}
	}

	free(pathfile);

	dprintf("Delegate File Uid:%d\n", uid);

	return uid;
}


int ssdr_verify_sign(unsigned char* tokenfile, unsigned char* signature, unsigned int uid) 
{
    char *prefix, *mykey, *buffer;

    prefix = (char*) malloc (20);
    memset(prefix, 0, 20);  
    sprintf(prefix, "uid_%d", uid);

    mykey = (char*) malloc (25);
    memset(mykey, '\0', 25);
    strcat(mykey, prefix);
    strcat(mykey, "_pub.pem");

    buffer = (char*) malloc (200);
    memset(buffer, '\0', 200);
    strcat(buffer, "openssl rsautl -verify -in ");
    strcat(buffer, signature);
    strcat(buffer, " -pubin -inmykey ");
    strcat(buffer, mykey);
    strcat(buffer, " > extracted");

    system(buffer); 

    free(buffer);
    free(mykey);

    buffer = (char*) malloc(30);
    memset(buffer, '\0', 30);
    strcat(buffer, "diff ");
    strcat(buffer, tokenfile);
    strcat(buffer, " ./extracted");

    int verified = system(buffer);

    free(buffer);
    free(prefix);

    return verified;
}

void accept_token(token* newt) 
{
    if(newt == NULL)
        return;/* tokens; */

    newt->next = NULL;

    if(tokens == NULL) {
        tokens = newt;
        return;/* tokens; */
    }
    
    token* temp = tokens;
    while(temp->next != NULL) {
        printf("uid = %d\n", temp->uid);
        temp = temp->next;
    }
    temp->next = newt;
}

void printall() {
    if (tokens == NULL) {
        printf("No tokens!!\n");
        return;
    }

    printf("Print all tokens:\n");
    token* temp = tokens;
    while(temp->next != NULL) {
        printf("uid = %d, filename = %s\n", temp->uid, temp->filename);
        temp = temp->next;
    }

    if (temp != NULL)
        printf("uid = %d, filename = %s\n", temp->uid, temp->filename);
}

void delete_tokens()
{
	token *tmp = tokens;
	while (tmp != NULL) {
		token* next = tmp->next;
		free(tmp);
		tmp = next;
	}
}

int check_further_delegate(unsigned char* filename, int action, unsigned long until_time, unsigned int uid) {
    /*check if the user has delegation rights */
    token* temp = tokens;
    while(temp != NULL) {
        int cmp = strcmp(temp->filename, filename);
        if(cmp == 0 && uid == temp->uid) {
            if(action == temp->action || temp->action == 2) { 
                if(temp->delegate == 0) {
                    if(until_time <= temp->until_time)
                        return 0; /*has access */
                    else {
                        printf("Expired.\n");
                        return 1;
                    }
                }
                else { 
                    printf("Only access, no delegation.\n");
                    return 1;
                }
            }
        }
        temp = temp->next;
    }
    /*deny access */
    return 1; 
}

void verify_and_store(unsigned char* tokenfile, unsigned char* signature, unsigned int uid) {

	/* no signed token 
    int verified = ssdr_verify_sign(tokenfile, signature, uid);
    
    if(verified != 0) {
        printf("Verification of signature %s for token %s is failed\n", signature, tokenfile);
        return;
    } else {
        printf("Signature on token verified.\n");
    }
    */

    /*read token */
    FILE* fp = fopen(tokenfile, "r");
    if (fp == NULL) {
    	dprintf("token file %s open failed.\n", tokenfile);
        return;
	}

    token* t = (token*) malloc (sizeof(token));

    t->filename = (unsigned char*) malloc (20);
    t->next = NULL;

    fscanf(fp, "%d %s %d %d %ld\n", &t->uid, t->filename, &t->action, &t->delegate, &t->until_time);

    printf("Read token: %d %s %d %d %ld\n", t->uid, t->filename, t->action, t->delegate, t->until_time);
    fseek(fp, 0, SEEK_SET);

    fclose(fp);

    /*check if user can further delegate for specified file */
    int ret = check_further_delegate(t->filename, 
    									t->action, t->until_time, uid);

	/* file owner delegate */
    if(uid == get_repofile_uid(t->filename)
    		&& uid != t->uid) 
		ret = 0; /* can delegate */

    if(ret == 0) {
        printf("Delegation token-%s accepted.\n\n", tokenfile);
        accept_token(t);
    }
    else {
        printf("No delegation permission\n");
    }
}

int verify_access(unsigned int client_uid, unsigned char* filename, int action) {
    /*check token list */
    token* temp = tokens;
    dprintf("begin compare..tokens.\n");

    while(temp != NULL) {
        int cmp = strcmp(temp->filename, filename);
        
        dprintf("tmp->name:%s,name:%s, cmp=%d\n", temp->filename, filename, cmp);
        dprintf("tmp->uid:%u,uid:%u\n", temp->uid, client_uid);

        if(cmp == 0 && client_uid == temp->uid) {
            if(action == temp->action || temp->action == 2) {
                return 0;
            }
        }
        temp = temp->next;
    }
    return 1;
}

char* getfromclient(int sock)
{
    char buffer[NSIZE][LSIZE];
    char *retbuf;
    int len, i = 0, totalsize = 0;
    memset(buffer, '\0', NSIZE*LSIZE);

    /* recv the file */
    while (1)
    {
        len = recv(sock, buffer[i*LSIZE], LSIZE, 0);

        if (len <= 0 || len < LSIZE)
        {
            /*printf("File received\n"); */
            break;
        }
        i++;

        totalsize += len;
    }

    if (len > 0) totalsize += len;

    char *pbuf = (char *)buffer;

    len = (strlen(pbuf)+1)*sizeof(char);
    //len  = totalsize + 1;

    retbuf = (char *)malloc(len);
    memset(retbuf, '\0', len);

    /* copy to dest buffer */
    strcat(retbuf, pbuf);

    return retbuf;
}

void sendtoclient(int sock, char* fname, char* sendbuf)
{
    char buffer[LSIZE];
    int sbytes;
    int i, k = 0, len = totalLen;
    /*char *buffer = malloc(len + 1); */
    /*memset(buffer, '\0', len + 1); */
    memset(buffer, '\0', LSIZE);
    printf("Send file size:%d\n", len);

    while(1)
    {
        for (i = 0; i < LSIZE && (k*LSIZE+i)<len; i++)
        {
            buffer[i] = sendbuf[k*LSIZE + i];
        }
        k++;

        if (i < LSIZE)
        {
            sbytes = send(sock, buffer, i, 0);
            break;
        }
        else
            sbytes = send(sock, buffer, LSIZE, 0);

    }
}

void ssdr_session(int *sock)
{
    int client_sockfd = *sock;
    char buffer[20];

    SSL_load_error_strings();
    SSL_library_init();
    const SSL_METHOD *meth;
    SSL_CTX *ctx;
    meth = SSLv23_method();
    ctx = SSL_CTX_new(meth);
    /*Load cert */
    SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM);
    //SSL_CTX_use_certificate_file(ctx, "server_pub.pem", SSL_FILETYPE_PEM);
    /*Load private key */
    SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM);
    /*Load CA cert */
    SSL_CTX_load_verify_locations(ctx, CACERT, NULL);
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_set_verify_depth(ctx, 1);

    /*mutual authentication */
    SSL* ssl = SSL_new(ctx);

    SSL_set_fd(ssl, client_sockfd);
    int e = SSL_accept(ssl);

    if(e != 1) {
        printf("TLS/SSL handshake failed!  %d\n", e);
        exit(1);
    }
    else
        printf("TLS/SSL handshake success!\n");

    while (1)
    {
        /*wait for command */
        memset(buffer, '\0', 20);
        e = SSL_read(ssl, buffer, sizeof(buffer)-1);

        char cmd[4];
        cmd[0] = buffer[0]; cmd[1] = buffer[1];
        cmd[2] = buffer[2]; cmd[3] = '\0';

        /*enumerate the commands */
        if (!strcmp(cmd, "GET"))
        {
            char filename[NSIZE], c_uid[NSIZE];
            char retbuf[1];
            struct stat file_st;
            int i, k, clientid;
            memset(filename, '\0', NSIZE);
            memset(c_uid, '\0', NSIZE);

			/* get filename */
            i = 4;
            do
            {
                filename[i-4] = buffer[i];
                i++;
            }while(buffer[i] != ' ');
            filename[i] = '\0';

            /* space */
            i++;
            
            /* get uid */
            k = i;
            do
            {
                c_uid[i-k] = buffer[i];
                i++;
            }while(buffer[i] != '\0');

            clientid = atoi((const char *)c_uid);

            printf("\n--------GET------\n \
            			file: %s, client uid: %d\n", filename, clientid);

            char *actfile=(char *)malloc((strlen(REPODIR)+strlen(filename)+1));
            sprintf(actfile, REPODIR"%s", filename);

            if (stat(actfile, &file_st) >= 0)
            {
                FILE *fpr = fopen(actfile, "rb");
                if (fpr == NULL){ return; }
                char* str_uid = (unsigned char*) malloc (IDSIZE);
                fread(str_uid, IDSIZE, 1, fpr);
                fclose(fpr);

                file_st.st_uid = atoi(str_uid);
                free(str_uid);

                printf("Owner uid: %d; User uid: %d\n", file_st.st_uid, clientid);
                if (clientid == file_st.st_uid)
                {
                    memset(buffer, '\0', 20);
                    retbuf[0] = '1';
                }
                else
                {
                    /*check delegation first */
                    int ret = verify_access(clientid, filename, 0);
                    /*if failed delegation check */
                    dprintf("ret=%d\n", ret);
                    if(ret == 1) /*deny access */
                        retbuf[0] = '0';
                    else retbuf[0] = '1';
                }

            }
            else /*file doesn't exist */
            {
                printf("Cannot find the requested file\n");
                retbuf[0] = '0';
            }
            /*send back access */
            e = SSL_write(ssl, retbuf, strlen(retbuf));

            if (retbuf[0] == '0')
                continue;

            /*Extract the file */
            char* sendbuf = (char*) get(actfile);

            sendtoclient(client_sockfd, actfile, sendbuf);

			free(actfile);
            free(sendbuf);

            continue;
        }
        else if (!strcmp(cmd, "PUT"))
        {
            int i, k, clientid;
            char filename[NSIZE], c_uid[IDSIZE];
            char retbuf[1];
            struct stat file_st;

            memset(filename, '\0', NSIZE);
            memset(c_uid, '\0', IDSIZE);

			/* filename get */
            i = 4;
            do
            {
                filename[i-4] = buffer[i];
                i++;
            }while(buffer[i] != ' ');
            filename[i] = '\0';

			/* space omit */
            i++;

            /* get uid */
            k = i;
            do
            {
                c_uid[i-k] = buffer[i];
                i++;
            }while(buffer[i] != '\0');

            clientid = atoi((const char *)c_uid);

            printf("\n------------PUT------------\n \
            			file: %s, client uid:%d\n", filename, clientid);

            char *actfile = (char *)malloc((strlen(REPODIR)+strlen(filename)+1)*sizeof(char));
            sprintf(actfile, REPODIR"%s", filename);

            if (stat(actfile, &file_st) >= 0)
            {
                /*get the owner id */
                FILE *fpr = fopen(actfile, "rb");
                if(fpr == NULL){
                	free(actfile);
					return;
                }

                char* str_uid = (unsigned char*) malloc (IDSIZE);
                fread(str_uid, IDSIZE, 1, fpr);
                fclose(fpr);

                file_st.st_uid = atoi(str_uid);


                /*file_st.st_uid = atoi(c_uid); */
                printf("Owner uid: %d; User uid: %d\n", file_st.st_uid, clientid);
                if (clientid == file_st.st_uid)
                {
                    memset(buffer, '\0', 20);
                    retbuf[0] = '1';
                }
                else
                {
                    /*check delegation first */
                    int ret = verify_access(clientid, filename, 1);
                    /*if failed delegation check */
                    if(ret == 1) /*deny access */
                        /*if failed delegation check */
                        retbuf[0] = '0';
                    else {
                        memcpy(c_uid, str_uid, IDSIZE);
                        retbuf[0] = '1';
					}
                }
                free(str_uid);
            }
            else
            {
                printf("Cannot find the requested file\n");
                retbuf[0] = '1';
            }

            /*send back access code */
            e = SSL_write(ssl, retbuf, strlen(retbuf));
            if (retbuf[0] == '0') {
                printf("Waiting...\n");
                continue;
            }

            /*get file from client */
			sprintf(actfile, REPODIR"%s", filename);

            char* recvbuf = NULL;
            putting = 0;
            do{
            	if (recvbuf != NULL) free(recvbuf);

                recvbuf = getfromclient(client_sockfd);
                printf("Putting buffer size: %d\n", strlen(recvbuf));
                putting += 1;

                put(recvbuf, actfile, c_uid);

            } while (strlen(recvbuf) == LSIZE);

            if (recvbuf != NULL) free(recvbuf);

            /*encrypt the whole received buffer */
            putting = 0;

            encrypt_put(actfile, c_uid);

        }
        else if (!strcmp(cmd, "DG-") || !strcmp(cmd, "DG+"))
        {
            int i, k;
            char tokenfile[NSIZE], c_uid[IDSIZE];
            int uid;
            memset(tokenfile, '\0', NSIZE);
            memset(c_uid, '\0', IDSIZE);

            i = 4;
            do {
                tokenfile[i-4] = buffer[i];
                i++;
            }while(buffer[i] != ' ');

			/* space */
            i++;

            k = i;
            do {
                c_uid[i-k] = buffer[i];
                i++;
            }while(buffer[i] != '\0');

            uid = atoi((const char *)c_uid);

			char fullpath[NSIZE] = {0};
			sprintf(fullpath, TOKDIR"%s", tokenfile);
			memset(tokenfile, 0, NSIZE);
			strcat(tokenfile, fullpath);

            printf("\nDelegate: %s %d\n", tokenfile, uid);

            /*verify and add token   */
            int len = strlen(tokenfile)*sizeof(char)+8;
            unsigned char* signedtokenfile = (unsigned char*) malloc (len);

            memset(signedtokenfile, '\0', len);
            strcat(signedtokenfile, tokenfile);
            strcat(signedtokenfile, "_signed");
			
			recvfile_from_client(tokenfile, client_sockfd);

            verify_and_store(tokenfile, signedtokenfile, uid);

            free(signedtokenfile);

            printall();
        }
        else if (!strcmp(cmd, "BYE"))
        {
            SSL_shutdown(ssl);
            /*debug */
            printf("\nEnd thread\n");
            pthread_exit(NULL);

            close(*sock);
        }
    }
}


int main()
{
    /*initialization */

    pid_t pid;

    uid_t uid = getuid();   

    printf("Starting for UID: %d.\n", uid);
    struct stat file_st;
    if(stat(CERTF, &file_st) < 0 || stat(KEYF, &file_st) < 0) {   
        char* buffer = (char*) malloc (25);
        memset(buffer, '\0', 25);
        strcat(buffer, "./script ");
        strcat(buffer, "srv");
        system(buffer);
        free(buffer);
    } 

    printf("Listening...\n");
    pthread_t client[NUM_CLIENTS]; 
    int conn = 0;

    int listen_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(listen_sock == -1)
        err("listen_sock error\n");
    int server_port = 9000;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;/*Local IP address */
    /*server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); */

    memset(&(server_addr.sin_zero), 0, 8);

    int e = bind(listen_sock, (struct sockaddr*)&server_addr, 
    				sizeof(server_addr));

    e = listen(listen_sock, 3);

    while(1)
    {

        struct sockaddr_in client_addr;
        socklen_t addr_size;
        int sock = accept(listen_sock, (struct sockaddr*) &client_addr, 
        					&addr_size);

        if(sock == -1)
        {
            perror("accept() error!\n");
            continue;
        }

		/*
        pid = fork();
        if (pid == 0) {
            ssdr_session(&sock);
            close(listen_sock);
        }
        else if (pid > 0) {
            close(sock);
        }
        */
        pthread_create(&client[conn], NULL, (void *(*)(void *)) &ssdr_session, &sock);

        ++conn;
        if (conn == NUM_CLIENTS)
        {
            fprintf(stderr, "Reach Max connections\n");
            exit(1);
        }
    }

    delete_tokens();

    return 0;
}
