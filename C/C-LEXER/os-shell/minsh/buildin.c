#include "buildin.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <unistd.h>
#include "externs.h"
#include "parse.h"

void do_exit(void);
void do_cd(void);
void do_history(void);
void do_jobs(void);
void do_fg(void);
void do_bg(void);

typedef void (*CMD_HANDLER)(void);

typedef struct buildin_cmd {
    char *name;
    CMD_HANDLER handler;

} BUILDIN_CMD;

BUILDIN_CMD buildins[] = {
    {"exit", do_exit},
    {"cd", do_cd},
    {"history", do_history},
    {"jobs", do_jobs},
    {"fg", do_fg},
    {"bg", do_bg},
    {NULL, NULL}
};

/**
 * build-in command parse
 */
int buildin() {

	int i = 0;
    while (buildins[i].name != NULL) {
        if (check(buildins[i].name)) {
            buildins[i].handler();
            return 1;
        }
        i++;
    }
    return 0;
}

void do_exit(void) {
    printf("exit\n");
    exit(EXIT_SUCCESS);
}

void do_cd(void) {
    //printf("do cd ..\n");
	get_command(0);

	//printf("newdir=%s.\n", cmd[0].args[0]);

	if (chdir(cmd[0].args[0]) == -1) {
		printf("chdir failed.\n");
	}
	else 
		printf("change dir success.\n");
}

void do_history(void) {
    //printf("do history ..\n");

	int i = 0;
	char **p = cmdall;

	while (*p != NULL) {
		printf("%4d:%s", i++, *p++);
	}
}

void do_jobs(void)
{
	struct job *job;
	char *statusString;

	for (job = myJobSet.head; job; job = job->next) {

		if (job->stop) statusString = "Stopped";
		else statusString = "Running";

		printf(JOB_STATUS_FORMAT, job->jobId, statusString);
	}
}

void do_fg(void)
{
	struct job *job;
	int jobNum;

	get_command(0);

	if (!cmd[0].args[0] || sscanf(cmd[0].args[0], "%d", &jobNum) != 1) {
		fprintf(stderr, "fg: bad argument %s\n", cmd[0].args[0]);
		return;
	}

	for (job = myJobSet.head; job; job = job->next)
		if (job->jobId == jobNum) break;

	if (!job) {
		fprintf(stderr, "fg: unknown job %d\n", jobNum);
		return;
	}

	if (tcsetpgrp(0, job->pgrp))
		perror("tcsetpgrp");

	myJobSet.fg = job;

	job->stop = 0;

	/* restart, send to group pid */
	kill(-job->pgrp, SIGCONT);
}

void do_bg(void)
{
	struct job *job;
	int jobNum;

	get_command(0);

	if (!cmd[0].args[0] || sscanf(cmd[0].args[0], "%d", &jobNum) != 1) {
		fprintf(stderr, "bg: bad argument %s\n", cmd[0].args[0]);
		return;
	}

	for (job = myJobSet.head; job; job = job->next)
		if (job->jobId == jobNum) break;

	if (!job) {
		fprintf(stderr, "bg: unknown job %d\n", jobNum);
		return;
	}

	job->stop = 0;

	/* restart, send to group pid */
	kill(-job->pgrp, SIGCONT);
}
