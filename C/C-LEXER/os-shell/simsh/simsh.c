#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/wait.h>
#include 	<string.h>
#include 	<errno.h>

#include	"simsh.h"

/**
 **	simple-shell
 ** Main functions:
 ** 1. parse command line looply;
 ** 2. use fork, exec, wait and signals to process command;
 ** 3. can dispose internal command;
 ** 4. pipe support;
 ** 5. redirection support;
 ** 6. job control support;
 **/

#define	DFL_PROMPT	"[OS shell:%s]$"

char *ppath; /* previous path */
char *cpath; /* current path */

char prompt[MAXSIZ+16];

char **cmdhistory;
int cmdnum = 0;
int cmdspot = 0;

struct jobSet myJobSet = {NULL, NULL};

void setup();
void checkJobs(struct jobSet *jobList);
void removeJob(struct jobSet *jobList, struct job *job);

int main()
{
	char	*cmdline , **arglist;
	int	result;
	int status;

	/* get current path for prompt */
	cpath = (char *) malloc(MAXSIZ);
	ppath = (char *) malloc(MAXSIZ);

	if (cpath == NULL) return -1;

	memset(cpath, 0, MAXSIZ);
	if (getcwd(cpath, MAXSIZ-1) == NULL) {
		fprintf(stderr, "%s:%d: getcwd failed: %s\n",
				__FILE__, __LINE__, strerror(errno));
		return -1;
	}
	sprintf(ppath, "%s", cpath);

	getprompt(cpath);

	/* signal handler */
	setup();

	/*
	 * recv command from consol, parse it and execute
	 */
	//while ( (cmdline = next_cmd(prompt, stdin)) != NULL ){
	while (1) {

		if (!myJobSet.fg) {

			/* see if any background processes have exited */
			checkJobs(&myJobSet);

			cmdline = next_cmd(prompt, stdin);

			if (cmdline == NULL) break;

			/* save command history */
			addcmdhistory(cmdline);

			if ( (arglist = splitline(cmdline)) != NULL  ){

				/* process command */
				result = process(arglist);
				freelist(arglist);
			}
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
				myJobSet.fg->stopped = 1;
				printf(JOB_STATUS_FORMAT, myJobSet.fg->jobId, "Stopped");
				myJobSet.fg = NULL;
			}

			if (!myJobSet.fg) {
				if (tcsetpgrp(0, getpid()))
					perror("tcsetpgrp");
			}
		}
	}

	freecmdhistory(cmdhistory);
	free(cpath);
	free(ppath);

	return 0;
}

char* getprompt(char *path)
{
	sprintf(prompt, DFL_PROMPT, path);

	return prompt;
}

void setup()
/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 */
{
	//signal(SIGINT,  SIG_IGN);
	//signal(SIGQUIT, SIG_IGN);
    //signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
	fprintf(stderr,"Error: %s,%s\n", s1, s2);
	exit(n);
}

void addcmdhistory(char *cmd)
{
	if (cmdspot == 0) {
		cmdhistory = emalloc(BUFSIZ);
		cmdspot = BUFSIZ / (sizeof(char*));
	}

	if (cmdnum + 1 > cmdspot) 
	{
		cmdhistory = erealloc(cmdhistory, cmdspot*sizeof(char*)+BUFSIZ);
		cmdspot += (BUFSIZ/sizeof(char*));
	}

	cmdhistory[cmdnum++] = cmd;
	cmdhistory[cmdnum] = NULL;
}

void freecmdhistory(char **cmdhis)
{
	char **p = cmdhis;

	while (*p != NULL) {
		free(*p++);
	}

	free(cmdhis);
}

/* Checks to see if any background processes have exited -- if they 
   have, figure out why and see if a job has completed */
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
            job->stopped = 1;
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
