#include "ssdc.h"
#include "base64.h"

#define BUFSIZE	1024
#define TOKDIR	"./token/"

char cmdline_buf[GETSIZE];
char filename[NSIZE];

void help();
void ssdc_cert_init(char *);
void ssdc_prompt();
int exists(char *);

#define dprintf	printf

int ssdc_cmd_args_check(char *cmd, char *args);

int sendfile_to_server(char *filename, int sockfd)
{
	int sendlen = 0, rlen = 0, slen = 0;
	char sendbuf[BUFSIZE];

	int fd = open(filename, O_RDONLY);
	if (fd < 0) return -1;

	while (1) {
		rlen = read(fd, sendbuf, BUFSIZE);

		if (rlen <= 0) break;

		do {
			slen = send(sockfd, sendbuf, rlen, 0);
		} while (slen != rlen);

		sendlen += slen;
	}

	close(fd);

	dprintf("Send file %s to server %d bytes.\n",
				filename, sendlen);

	return sendlen;
}

void sign_token(unsigned char* file, unsigned int uid) {
    char* out_file = (char*) malloc(strlen(file)+8);
    memset(out_file, '\0', strlen(file)+8);
    strcat(out_file, file);
    strcat(out_file, "_signed");

    char* prefix = (char*) get_prefix(uid);

    char* buffer = (char*) malloc (120);
    memset(buffer, '\0', 120);
    strcat(buffer, "openssl rsautl -sign -in ");
    strcat(buffer, file);
    strcat(buffer, " -inkey ");
    strcat(buffer, prefix);
    strcat(buffer, ".key -out ");
    strcat(buffer, out_file);
    printf("Command is %s and size is %d\n", buffer, (int)strlen(buffer));
    system(buffer);
    
    free(buffer);
    buffer = (unsigned char*) malloc (50);
    memset(buffer, '\0', 50);
    strcat(buffer, "sudo cp "); 
    strcat(buffer, out_file);
    strcat(buffer, " ../server/");
    strcat(buffer, out_file);
    system(buffer);
    free(buffer);

    free(prefix);
}

void ssdc_prompt()
{
    printf("Please Enter Your Command:\n");
}

int ssdc_cmd_args_check(char *cmd, char *args)
{
	int i;
	char *cmds[] = {"BYE", "GET", "PUT", "DG+", "DG-"};

	dprintf("array len=>%d\n", sizeof(cmds)/sizeof(char*));

	for (i = 0; i < sizeof(cmds)/sizeof(char*); i++) {
		if (strcmp(cmd, cmds[i]) == 0) {
			if (strlen(args) > strlen(cmd) + 1)
				return 0;
		}
	}

	if (strcmp(cmd, cmds[0]) == 0) return 0;

	return -1;
}

void cmd_parser(char *cmdline_buf, SSL* ssl, int sock)
{
    char cmd[4];
    int i;
    memcpy(cmd, cmdline_buf, 4);
    cmd[3] = '\0';

	if (ssdc_cmd_args_check(cmd, cmdline_buf) < 0) {
		printf("Command format error, Retry.\n");
		return ;
	}

    if (!strcmp(cmd, "GET"))
    {
        memset(filename, '\0', NSIZE);
        i = 4;


        while (cmdline_buf[i] != '\n')
            filename[i-4] = cmdline_buf[i++];

        GET(filename, ssl, sock);
    }
    else if (!strcmp(cmd, "PUT"))
    {
        memset(filename, '\0', NSIZE);
        i = 4;
        while (cmdline_buf[i] != '\n')
            filename[i-4] = cmdline_buf[i++];

        PUT(filename, ssl, sock);
    }
    else if (!strcmp(cmd, "DG-") || !strcmp(cmd, "DG+"))
    {
        int i, k, e;
        //DG- <filename> <host uid> <time> <option>
        char c_hostuid[NSIZE], c_time[NSIZE], buffer[LSIZE];
        char newcmd[NSIZE], path[20];
        int uid, hostuid, option;
        long int time;

        memset(filename, '\0', NSIZE);
        memset(c_hostuid, '\0', NSIZE);
        memset(c_time, '\0', NSIZE);
        memset(newcmd, '\0', NSIZE);

        pid_t pid = getpid(); //get client process pid
        struct stat usr_st;

        //get UID from pid
        sprintf(path, "/proc/%d", pid);
        stat(path, &usr_st);
        uid = usr_st.st_uid;

        i = 4;
        do {
            filename[i-4] = cmdline_buf[i];
            i++;
        }while(cmdline_buf[i] != ' ');

        i++;

        k = i;
        do {
            c_hostuid[i-k] = cmdline_buf[i];
            i++;
        }while(cmdline_buf[i] != ' ');
        hostuid = atoi((const char *)c_hostuid);

        i++;

        k = i;
        do {
            c_time[i-k] = cmdline_buf[i];
            i++;
        }while(cmdline_buf[i] != ' ');
        time = atol((const char *)c_time);

        i++;

        k = i;
        option = atoi((const char *)(&cmdline_buf[i])); 

        //********************CALL delegate, get token
        unsigned char* tokenfile = NULL;
        if (!strcmp(cmd, "DG-"))
            tokenfile = delegate(hostuid, filename, time, option);//option is the action (get/put/both)
        else
            tokenfile = delegate_plus(hostuid, filename, time, option);//option is the action (get/put/both)

        sprintf(newcmd, "DG- %s %d", tokenfile+strlen(TOKDIR), uid);
        printf("sending cmd to server... - %s\n", newcmd);
        
        e = SSL_write(ssl, newcmd, sizeof(newcmd));

        /* send token file to server */
        sendfile_to_server(tokenfile, sock);

        free(tokenfile);
    }
    else if (!strcmp(cmd, "BYE"))
    {
        end_session(ssl);
        exit(1);
    }
    else
    {
        printf("Unsupported Command Entered!\n");
        return;
    }
}

int main(int argc, char** argv)
{
    uid_t uid = getuid(); // get the uid of current user
    char* prefix = (char*)get_prefix(uid);

    /* prepare certs and keys */
    ssdc_cert_init(prefix);

    char* certfile = (char*)malloc(20);
    strcpy(certfile, prefix);
    strcat(certfile, ".crt");

    char* keyfile = (char*)malloc(20);
    strcpy(keyfile, prefix);
    strcat(keyfile, ".key");

    SSL_load_error_strings();
    SSL_library_init();

    const SSL_METHOD *meth;
    SSL_CTX *ctx;
    meth = SSLv23_method();
    ctx = SSL_CTX_new(meth);

    //Load cert
    printf("Loading Client Certificate...\n");
    SSL_CTX_use_certificate_file(ctx, certfile, SSL_FILETYPE_PEM);
    //Load private key
    printf("Loading Client Private Key...\n");
    SSL_CTX_use_PrivateKey_file(ctx, keyfile, SSL_FILETYPE_PEM);
    //Load CA cert
    printf("Loading CA Certificate...\n");
    SSL_CTX_load_verify_locations(ctx, "CA.crt", NULL);
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_set_verify_depth(ctx, 1);

	/* free buffer */
    free(prefix);
    free(certfile);
    free(keyfile);

    SSL* ssl = SSL_new(ctx);

    int sock = start_session(ssl);
    
    while(1)
    {
        ssdc_prompt();
        fgets(cmdline_buf, LSIZE, stdin);

        while (!strncmp("help", cmdline_buf, 4))
        {
            help();
            ssdc_prompt();
            memset(cmdline_buf, '\0', LSIZE);
            fgets(cmdline_buf, LSIZE, stdin);
        }

        cmd_parser(cmdline_buf, ssl, sock);
    }

    return 0;
}


// util
int exists(char* f) {   
    printf("Checking if file %s exists.\n", f);
    FILE* fp = fopen(f, "r");
    if(fp == NULL)
        return 0;
    else {
        fclose(fp);
        return 1;
    }
}

char* get_prefix(unsigned int uid) {

    char* key_str = (char*) malloc (sizeof(unsigned int)+21*sizeof(char));
    char* key = key_str;

    key_str += sprintf(key_str, "uid_");
    key_str += sprintf(key_str, "%d", uid);

    *(key_str + 1) = '\0';

    printf("Prefix UID %d is %s\n", uid, key);
    return key;
}

void ssdc_cert_init(char* prefix) {

    uid_t uid = getuid();   

    char* cert = (char*) malloc (20);
    strcpy(cert, prefix);
    strcat(cert, ".crt");

    char* key = (char*) malloc (20);
    strcpy(key, prefix);
    strcat(key, ".key");
    
    printf("Starting up program for user with UID %d and  \
    	key %s and cert %s. Prefix is %s.\n", uid, key, cert, prefix);
    if((exists(cert) == 0) || (exists(key) == 0)) {
        char* buffer = (char*) malloc (25);
        memset(buffer, '\0', 25);
        strcat(buffer, "./script "); // script is the file used to create key and certificate for the user
        strcat(buffer, prefix);
        printf("Command is %s and size is %d\n", buffer, (int)strlen(buffer));
        system(buffer); // call the script
        free(buffer);
    } else {
        printf("Keys and certificate have already been generated.\n");
    }   

    free(cert);
    free(key);
}




void err(char* msg)
{
    perror(msg);
    printf("\n");
    exit(1);
}


int start_session(SSL* ssl)
{
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == -1)
        err("socket error");
    int server_port = 9000;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;   /* loaclhost */
//  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    memset(&(server_addr.sin_zero), 0, 8);
    int e = connect(sock, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (e == -1)
        printf("Connection to Server Failed!\n");

    if (!SSL_set_fd(ssl, sock))
        ERR_print_errors_fp(stderr);
    
    e = SSL_connect(ssl);

    //SSL_get_peer_certificate(ssl);

    if(e != 1) {
		perror("SSL_connect error\n");
        printf("TLS/SSL Session Establishment Failed!\n");
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    else
        printf("TLS/SSL Session Established!\n");

    return sock;
}

int GET(char *filename, SSL* ssl, int sock)
{
    char buffer[GETSIZE], readbuf[GETSIZE];
    char path[20];
    pid_t pid = getpid(); //get client process pid
    char cmd[NSIZE];
    int e, len, wbytes, fd;
    struct stat usr_st;

    //get UID from pid
    sprintf(path, "/proc/%d", pid);
    stat(path, &usr_st);

    //send UID
    sprintf(cmd, "GET %s %d", filename, usr_st.st_uid);
    e = SSL_write(ssl, cmd, strlen(cmd));

    //get access code
    e = SSL_read(ssl, buffer, sizeof(buffer)-1);

    if (buffer[0] == '1')
    {
        printf("GET Access Granted!\n");
        if ((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC)) < 0)
        {
            err("Failed to open local file");
        }
        // receive file data
        while (1)
        {
            printf("Reading from server...\n");

            memset(readbuf, '\0', GETSIZE);
            len = recv(sock, readbuf, GETSIZE, 0);

            if(len <= 0)
                break;

            size_t output_length;

            char *base64_buf = base64_decode(readbuf, len, &output_length);
            printf("\tReceived %d bytes from server, decoded to %d bytes\n",
            			len, output_length);
//          wbytes = write(fd, readbuf, len);
            wbytes = write(fd, base64_buf, output_length);

            free(base64_buf);

            if (len < GETSIZE)
                break;
        }

        close(fd);

        printf("File <%s> written to local file system.\n", filename);
        return 0;
    }
    else
    {
        printf("GET Access Denied!\n");
        return -1;
    }
}

int PUT(char* filename, SSL* ssl, int sock)
{
    char buffer[PUTSIZE], readbuf[PUTSIZE];
    char path[20];
    pid_t pid = getpid(); //get client process pid
    char cmd[NSIZE];
    int e, fd, len, sdbytes;
    struct stat usr_st;

    //get UID from pid
    sprintf(path, "/proc/%d", pid);
    stat(path, &usr_st);

    //send UID
    sprintf(cmd, "PUT %s %d", filename, usr_st.st_uid);
    e = SSL_write(ssl, cmd, strlen(cmd));

    //get access code
    e = SSL_read(ssl, buffer, sizeof(buffer));

    if (buffer[0] == '1')
    {
        printf("PUT Granted!\n");
        if ((fd = open(filename, O_RDONLY)) < 0)
        {
            err("Failed to open local file");
        }
        // send file data
        while (1)
        {
            int size = PUTSIZE;

            len = read(fd, readbuf, PUTSIZE);
            size_t output_length;

            char *base64_buf = base64_encode(readbuf, len, &output_length);

            if (len <= 0)
                break;

            if (len < size)
                size = len;
            printf("Read %d bytes from file, encoded to %d bytes\n",
            					len, output_length);
//          sdbytes = send(sock, readbuf, size, 0);

            sdbytes = send(sock, base64_buf, output_length, 0);
            printf("\t%d bytes were sent to server\n", sdbytes);

            free(base64_buf);
        }

        close(fd);
        printf("File has already been written to server file system.\n");
        return 0;
    }
    else
    {
        printf("PUT Denied!\n");
        return -1;
    }

}

int end_session(SSL* ssl)
{
    int e = SSL_write(ssl, "END", strlen("END"));
}

int tok_num;

unsigned char* dump_token(token t, unsigned int from_uid) 
{
    unsigned char* tk = (unsigned char*)
    						malloc (sizeof(t) +
    						strlen(t.filename)*sizeof(char) + 5);

    unsigned char* data = tk;
    tk += sprintf(tk, "%d ", t.uid);
    tk += sprintf(tk, "%s ", t.filename);
    tk += sprintf(tk, "%d ", t.action);
    tk += sprintf(tk, "%d ", t.delegate);
    tk += sprintf(tk, "%ld", t.until_time);// tv->tv_sec);
    *(tk+1) = '\0';

    unsigned char* tk_file = (unsigned char*) malloc (128);
    unsigned char* file = tk_file;
    tk_file += sprintf(tk_file, "tk");
    tk_file += sprintf(tk_file, "%d", tok_num);
    tk_file += sprintf(tk_file, "_");
    tk_file += sprintf(tk_file, "%d", from_uid);
    *(tk_file +1) = '\0';
    tok_num++;

	char fullpath[128] = {0};
	sprintf(fullpath, TOKDIR"%s", file);
	
	memset(file, 0, 128);
	strcat(file, fullpath);

    printf("Writing token file %s.\n", file);
    FILE* fp = fopen(file, "w");
    printf("Token is %s\n", data);

    fwrite(data, strlen(data), 1, fp);

    free(data);

    fclose(fp);

    return file;
}

unsigned char* delegate_impl(unsigned int uid, unsigned char* filename, unsigned long until_time, int action, int delegate);

unsigned char* delegate(unsigned int uid, unsigned char* filename, unsigned long until_time, int action) {
    return delegate_impl(uid, filename, until_time, action, 1); //cannot further delegate
}

unsigned char* delegate_plus(unsigned int uid, unsigned char* filename, unsigned int until_time, int action) {
    return delegate_impl(uid, filename, until_time, action, 0); //can further delegate
}

unsigned char* delegate_impl(unsigned int uid, unsigned char* filename, unsigned long until_time, int action, int delegate) {
    token t;
    t.uid = uid;
    t.filename = filename;
    t.action = action;  
    t.delegate = delegate;
    t.until_time = until_time; //time from the epoch until when the token is valid
    
    unsigned int from_uid = getuid();
    unsigned char* file = dump_token(t, from_uid);

	// no sign token now
    //sign_token(file, from_uid);

    return file;
}

void help()
{
    printf("The following commands are supported now:\n\
    GET <filename>\n\
    PUT <filename>\n\
    DG- <filename> <host uid> <time> <option> (token cannot be relayed)\n\
    DG+ <filename> <host uid> <time> <option> (token can be relayed)\n\
    BYE\n");
}

int test_main() {

    char* prefix = (char*) get_prefix(getuid());

    ssdc_cert_init(prefix);

    tok_num = 0;
    struct timeval* tv = (struct timeval*) malloc (sizeof(struct timeval));
    gettimeofday(tv, NULL);
    unsigned long t = tv->tv_sec;
/*  delegate(2000, "token_data", t, 2);
    delegate(3000, "token_data", t-10, 2);
    delegate(3000, "token_data", t+10, 2);
*/
    delegate_plus(2000, "/home/asha/1", t, 2);
    delegate_plus(3000, "token_data", t-10, 2);
    delegate_plus(3000, "token_data", t+10, 2);

}
