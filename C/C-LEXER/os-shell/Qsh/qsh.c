/*
 * This program implement a cute-shell(qsh)
 * Date: 2015-03-20
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "qsh.h"

#define DEBUG 0

#define BUFSIZE 	1024
#define CMDNUMBER 	100
#define MAX			100000
#define JOB_STATUS_FORMAT "[%d] %-26s\n"

char *cmds[MAX];
int  cmdno = 0;

char infile[FILENAMELEN]; // for redirection
char outfile[FILENAMELEN];

int infd = 0, outfd = 1; // file desc

/*
 * Job control structure
 */
typedef struct job {
	int jobId;
	pid_t pgrp;
	int stop;
	struct job *next;
} job;

typedef struct jobSet {
	struct job *head;
	struct job *fg;
} jobSet;

struct jobSet myJobSet = {NULL, NULL};
void checkJobs(struct jobSet *jobList);
void removeJob(struct jobSet *jobList, struct job *job);
void addJob(int pid, int inBg);

int main(int argc, char *argv[])
{
	char arg[BUFSIZE];
	ssize_t readbytes;
	char str_prompt[BUFSIZE + 1];
	int status;
	
    signal(SIGTTOU, SIG_IGN);

	while(1){

		if (!myJobSet.fg) {

			checkJobs(&myJobSet);

			fprintf(stdout, "%s", shell_prompt(str_prompt));
			fflush(stdout);

			//void *memset(void *s, int c, size_t n);
			memset(arg, 0, sizeof(arg));

			//ssize_t read(int fd, void *buf, size_t count);
			if((readbytes = read(STDIN_FILENO, arg, BUFSIZE)) < 0){
				fprintf(stderr, "read failed: %s\n", strerror(errno));
				exit(1);
			}

			if(arg[readbytes - 1] == '\n'){ /* clear '\n' at end of arg */
				arg[readbytes - 1] = '\0';
			} 

			if (cmdno >= MAX-1) {
				printf("Warning: command too much to store.\n");
			}
			else {
				cmds[cmdno++] = strdup(arg);
				cmds[cmdno] = NULL;
			}

			if(qsh_exit(arg) == 1){ /* check exit or not */
				break;
			}

			qsh_process(arg);

		} else {

			waitpid(myJobSet.fg->pgrp, &status, WUNTRACED);

			if (WIFEXITED(status) || WIFSIGNALED(status)) {
				/* the child exited */
				removeJob(&myJobSet, myJobSet.fg);
				myJobSet.fg = NULL;

				/* move shell to foreground */
				if (tcsetpgrp(0, getpid()))
					perror("tcsetpgrp");

			} else {
				/* the child was stopped */
				myJobSet.fg->stop = 1;
				printf(JOB_STATUS_FORMAT, myJobSet.fg->jobId, "Stopped");
				myJobSet.fg = NULL;
			}

			if (!myJobSet.fg) {
				if (tcsetpgrp(0, getpid()))
					perror("tcsetpgrp");
			}
		}
	}

	free_cmd_history();

	return 0;
}

void free_cmd_history()
{
	int i;

	for (i = 0; i < MAX; i++)
	{
		if (cmds[i] == NULL) break;

		free(cmds[i]);
	}
}

/*
 * parse args
 * note that: args[n] and arg shared memory!
 * redirection support:
 *     < infile, > outfile
 */
int parse_cmdline_args(char *args[], char *arg)
{
	char *ifn, *ofn;

	char **p;
	char *q;
	int ch;
	int count;

	p = args;
	q = arg;

	infile[0] = '\0';
	outfile[0] = '\0';
	
	count = 0;
	while(*q != '\0'){
		
		//int read_char(char *arg);
		while((ch = read_char(q)) == ' '){ /* skip space */
			q++;
		}

		if (ch == '<') { /* infile redirection */
			*q++ = '\0';
			while ((ch = read_char(q)) == ' ') q++;
			ifn = infile; /* infile for STDIN */
			ch = read_char(q);
			while (ch != ' ' && ch != '>' && ch != '|' && ch != '\0') {
				*ifn = ch;
				ifn++;
				q++;
				ch = read_char(q);
			}

			*ifn = '\0';
		}

		while ((ch = read_char(q)) == ' ') q++;

		if (ch == '>') {
			*q++ = '\0';
			while ((ch = read_char(q)) == ' ') q++;
			ofn = outfile; /* outfile for STDOUT */
			ch = read_char(q);
			while (ch != ' ' && ch != '<' && ch != '\0') {
				*ofn = ch;
				ofn++;
				q++;
				ch = read_char(q);
			}

			*ofn = '\0';
		}

		if (ch == '\0') break;

		while ((ch = read_char(q)) == ' ') q++;
		
		*p++ = q++;
		count++;

		ch = read_char(q);

		while(ch != ' ' && ch != '<'
			&& ch != '>' && ch != '\0'){ /* find first space after word */
			q++;
			ch = read_char(q);
		}

		if(ch != '\0'){	
			*q++ = '\0';
			ch = read_char(q);
		}
	}

	return count;
}

/*
 * filter string
 */
int read_char(char *str)
{
	char filter[] = " \t\n";
	char *p;
	int flag;		/* flag 1 return ' ', 0 return *str */
	
	flag = 0;
	p = filter;
	while(*p != '\0'){
		if(*str == *p){
			flag = 1;
			break;
		}
		p++;
	}
	
	if(flag == 1){
		return ' ';
	}else{
		return *str;
	}
}

/*
 * print shell prompt
 */
char *shell_prompt(char *promptbuf)
{
	char tmpbuf[BUFSIZE + 1];

	//void *memset(void *s, int c, size_t n);
	memset(promptbuf, 0, BUFSIZE + 1);
	memset(tmpbuf, 0, sizeof(tmpbuf));

	//char *getcwd(char *buf, size_t size);
	if(getcwd(tmpbuf, sizeof(tmpbuf) - 1) == NULL){
		fprintf(stderr, "%s:%d: getcwd failed: %s\n", 
			__FILE__, __LINE__, strerror(errno));
		exit(1);
	}
	
	//int snprintf(char *str, size_t size, const char *format, ...);
	snprintf(promptbuf, BUFSIZE, "[OS Shell:%s]$", tmpbuf);

	return promptbuf;
}

/*
 * if args == quit
 */
int qsh_exit(char *arg)
{
	//int strcmp(const char *s1, const char *s2);
	if(strcmp(arg, "exit") == 0){
		return 1;
	}else{
		return 0;
	}
}

/*
 * Receive message and deal something
 */
int qsh_process(char *arg)
{
	int cmdnum;
	char *args[CMDNUMBER];
	int argnum;
	int inBg = 0;
	
	//int parse_cmdline_args(char *args[], char *arg); 
	argnum = parse_cmdline_args(args, arg);
	args[argnum] = NULL;

	if (argnum == 0) return -1;

	if (strcmp(args[argnum-1], "&") == 0) {
		inBg = 1;
		args[--argnum] = NULL;
	}

#if DEBUG
	int i;
	fprintf(stdout, "argnum = %d\n", argnum);
	for(i = 0; i < argnum; i++){
		fprintf(stdout, "[%d]%s\n", i, args[i]);
	}
#endif
	//int get_cmdnum(char *cmd)
	cmdnum = get_cmdnum(args[0]);
	
	switch(cmdnum){
	case SHELL_EMPTY:
		break;
	case SHELL_CD:
		qsh_builtin_cd(args);
		break;
	case SHELL_HISTORY:
		qsh_builtin_history(args);
		break;
	case SHELL_FORK:
		shell_fork(args, inBg);
		break;
	case SHELL_JOBS:
		shell_jobs(args);
		break;
	case SHELL_FG:
	case SHELL_BG:
		shell_fgbg(args);
		break;
	default:
		fprintf(stdout, "%s:%d: getcmd failed\n", __FILE__, __LINE__);
		break;
	}

	return 0;
}


/*
 * fork and called exec
 */
int shell_fork(char *args[], int inBg)
{
	pid_t pid[CMDNUMBER];
	int status;
	int fork_num;
	char **p;		/* point args */
	char *q;		/* point *args */

	/* get numbers of child process*/
	fork_num = 1;
	p = args;
	while(*p != NULL){
		q = *p;
		while(*q != '\0'){
			if(*q == '|'){
				fork_num++;
			}
			q++;
		}
		p++;
	}

#if DEBUG

	fprintf(stdout, "fork_num = %d\n", fork_num);

	fprintf(stdout, "infile = %s\n", infile);
	fprintf(stdout, "outfile = %s\n", outfile);

#endif

	/* case: child process number is one */
	if(fork_num < 2){
		//redirection support
		infd = 0;
		outfd = 1;

		if (infile[0] != '\0')
			infd = open(infile, O_RDONLY);
		if (outfile[0] != '\0')
			outfd = open(outfile, O_WRONLY|O_CREAT, 0666);

		//pid_t fork(void);
		if((pid[0] = fork()) < 0){
			/* error */
			perror("fork");
			exit(1);
		}else if(pid[0] == 0){
			signal(SIGTTOU, SIG_DFL);
			/* child process */
			//redirection support
			if (infd != 0) {
				close(0);
				dup2(infd, 0);
			}
			if (outfd != 1) {
				close(1);
				dup2(outfd, 1);
			}

			//int execvp(const char *file, char *const argv[]);
			if(execvp(args[0], args) < 0){
				perror("");
				exit(1);
			}

			if (infd != 0) close(infd);
			if (outfd != 1) close(outfd);

			exit(0);
		}

		setpgid(pid[0], pid[0]);
		addJob(pid[0], inBg);
	}

	/* parent process */
	//pid_t waitpid(pid_t pid, int *status, int options);
	if(fork_num < 2){

		if (infd != 0) close(infd);
		if (outfd != 1) close(outfd);

		//waitpid(pid[0], &status, 0);
	}else{
		//int mutifork(char *args[])
		status = mutifork(args, inBg);
	}

	return status;
}

/*
 * likes shell's cd
 */
int qsh_builtin_cd(char *args[])
{
	if (args[1] == NULL) return -1;

	char buf[BUFSIZE + 1];

	memset(buf, 0, BUFSIZE + 1);
	
	if(args[1][0] != '/' && args[1][0] != '.'){
		if(getcwd(buf, BUFSIZE) == NULL){
			fprintf(stderr, "%s:%d: getcwd failed: %s\n", __FILE__, 
				__LINE__, strerror(errno));
			return -1;
		}
		strncat(buf, "/", BUFSIZE - strlen(buf));
	}

	strncat(buf, args[1], BUFSIZE - strlen(buf));

#if DEBUG
	fprintf(stdout, "%s\n", buf);

#endif
	if(chdir(buf) == -1){
		fprintf(stderr, "%s:%d: chdir failed: %s\n", __FILE__, 
			__LINE__, strerror(errno));
	}

	return 0;
}

int qsh_builtin_history(char *args[])
{
	int i;

	for (i = 0; i < MAX; i++) {
		if (cmds[i] == NULL) break;

		printf("%4d:%s\n", i, cmds[i]);
	}
}

/*
 * Change cmd to int, according to shell.h
 */
int get_cmdnum(char *cmd)
{
	if(cmd == NULL) return SHELL_EMPTY;
	if(strcmp(cmd, "cd") == 0) return SHELL_CD;
	if (strcmp(cmd, "history") == 0) return SHELL_HISTORY;
	if (strcmp(cmd, "jobs") == 0) return SHELL_JOBS;
	if (strcmp(cmd, "fg") == 0) return SHELL_FG;
	if (strcmp(cmd, "bg") == 0) return SHELL_BG;

	return SHELL_FORK;
}

/*
 * multi-process
 */
int mutifork(char *args[], int inBg)
{
	int pipefd[CMDNUMBER][2];
	pid_t pid[CMDNUMBER];
	int i, j;
	int count;
	int status;
	char **arg_child[CMDNUMBER];
	char **p;
	char ***q;


	/* parse and split args to child arg */
	count = 0;
	p = args; q = arg_child;
	while(*p != NULL && p != NULL){
		*q++ = p;
		count++;
		while(*p != NULL && strcmp(*p, "|") != 0){
			p++;
		}
						
		if(*p != NULL){
			*p++ = NULL;
		}	
	}
	*q = NULL;

#if DEBUG			/* check child args */

	fprintf(stdout, "----------------------------------------\n");
	fprintf(stdout, "count = %d\n", count);
	q = arg_child; i = 0;
	while(*q != NULL){
		p = *q++;
		while(*p != NULL){
			fprintf(stdout, "[%d]%s\n", i, *p++);
		}
		i++;
	}
#endif
	
	/* fork count child process */
	for(i = 0; i < count; i++) {
		/* init pipe file descriptor */
		if(pipe(pipefd[i]) < 0){ /* FIXME: excess one */
			perror("pipe");
			return -1;
		}
		
		/* fork child i */
		if((pid[i] = fork()) < 0){
			fprintf(stderr, "%s:%d: fork() failed: %s\n", __FILE__,
				__LINE__, strerror(errno));
			return -1;
		}else if(pid[i] == 0){
			/* child i */
			signal(SIGTTOU, SIG_DFL);
			
			//int dup2(int oldfd, int newfd);
			if(i == 0){ /* the first child */
				close(pipefd[i][0]); /* close curr process read */

				if(dup2(pipefd[i][1], STDOUT_FILENO) < 0){
					perror("dup2 failed");
					return -1;
				}
			}else if(i == count - 1){ /* the last child */
				for(j = 0; j < i - 1; j++){ /* close unuse pipefd */
					close(pipefd[j][1]);
					close(pipefd[j][0]);
				}
				close(pipefd[j][1]); /* close prev process end of write */
				close(pipefd[i][0]); /* close curr process end of read */

				if(dup2(pipefd[j][0], STDIN_FILENO) < 0){
					perror("dup2 failed");
					return -1;
				}
			}else{
				for(j = 0; j < i - 1; j++){ /* close unuse pipefd */
					close(pipefd[j][1]);
					close(pipefd[j][0]);
				}
				close(pipefd[j][1]); /* close prev process end of write */
				close(pipefd[i][0]); /* close curr process end of read */

				if(dup2(pipefd[j][0], STDIN_FILENO) < 0){
					perror("dup2 failed");
					return -1;
				}
				if(dup2(pipefd[i][1], STDOUT_FILENO) < 0){
					perror("dup2 failed");
					return -1;
				}
			}
			//int execvp(const char *file, char *const argv[]);
			if(execvp(arg_child[i][0], arg_child[i]) < 0){
				fprintf(stderr, "%s:%d: fork() failed: %s\n", __FILE__,
					__LINE__, strerror(errno));

				exit(1);
			}
			
			exit(0);
		
			/* child process exit */
		}

		setpgid(pid[i], pid[0]); /* same group */
	}

	/* parent process */

	for(i = 0; i < count; i++){
		/* close all pipe file descriptor */
		close(pipefd[i][0]);
		close(pipefd[i][1]);	
	}

	/* when pipes command list, only add first command job */
	addJob(pid[0], inBg);

	for(i = 0; i < count; i++){
		//pid_t waitpid(pid_t pid, int *status, int options);

		waitpid(pid[i], &status, 0);
		if(status != 0){
			return status;
		}
	}

	return status;
}

void checkJobs(struct jobSet * jobList) {
    struct job * job;
    pid_t childpid;
    int status;
   
    while ((childpid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
        for (job = jobList->head; job; job = job->next) {
			if (childpid == job->pgrp) break;
        }

        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            /* child exited */
			printf(JOB_STATUS_FORMAT, job->jobId, "Done");
			removeJob(jobList, job);
        } else {
            /* child stopped */
			job->stop = 1;
			printf(JOB_STATUS_FORMAT, job->jobId, "Stopped");
        }
    }

    if (childpid == -1 && errno != ECHILD)
        perror("waitpid");
}

void removeJob(struct jobSet * jobList, struct job * job) {
    struct job * prevJob;

    if (job == jobList->head) {
        jobList->head = job->next;
    } else {
        prevJob = jobList->head;
        while (prevJob->next != job) prevJob = prevJob->next;
        prevJob->next = job->next;
    }

    free(job);
}

/* add one job a time */
void addJob(int pid, int inBg)
{
	/* add job to jobset */
	struct job* newJob = (struct job*) malloc(sizeof(struct job));
	newJob->jobId = 1;
	newJob->pgrp = pid;
	newJob->next = NULL;
	newJob->stop = 0;

	struct job* pjob;
	for (pjob = myJobSet.head; pjob; pjob = pjob->next)
		if (pjob->jobId >= newJob->jobId)
			newJob->jobId = pjob->jobId + 1;

	if (!myJobSet.head) myJobSet.head = newJob;
	else {
		for (pjob = myJobSet.head; pjob->next; pjob = pjob->next);
		/* add to tail */
		pjob->next = newJob;
	}

	if (inBg) {
		printf("[%d] %d\n", newJob->jobId, newJob->pgrp);
	} else {
		myJobSet.fg = newJob;

		if (tcsetpgrp(0, newJob->pgrp))
			perror("tcsetpgrp");
	}
}

void shell_jobs(char *args[])
{
	struct job *job;
	char *statusString;

	for (job = myJobSet.head; job; job = job->next) {

		if (job->stop) statusString = "Stopped";
		else statusString = "Running";

		printf(JOB_STATUS_FORMAT, job->jobId, statusString);
	}
}

int shell_fgbg(char *args[])
{
	struct job *job;
	int jobNum;

	if (!args[1] || sscanf(args[1], "%d", &jobNum) != 1) {
		fprintf(stderr, "%s: bad argument\n", args[0]);
		return 1;
	}

	for (job = myJobSet.head; job; job = job->next)
		if (job->jobId == jobNum) break;

	if (!job) {
		fprintf(stderr, "%s: unknown job %d\n", args[0], jobNum);
		return 1;
	}

	if (args[0][0] == 'f') {
		if (tcsetpgrp(0, job->pgrp))
			perror("tcsetpgrp");

		myJobSet.fg = job;
	}

	job->stop = 0;

	/* restart, send to group pid */
	kill(-job->pgrp, SIGCONT);

	return 0;
}
