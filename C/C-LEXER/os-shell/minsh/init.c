#include "init.h"
#include "externs.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

char workpath[PSIZ];

void sigint_handler(int sig);

void setup(void) {
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);

	cmdcap = cmdsiz = 0;
	cmdall = NULL;
}

void sigint_handler(int sig) {
    printf("\n[minishell]$ ");
    fflush(stdout);
}

void init(void) {
    memset(cmd, 0, sizeof(cmd));
    int i;
    for (i=0; i<PIPELINE; ++i) {
        cmd[i].infd = 0;
        cmd[i].outfd = 1;
    }
    memset(cmdline, 0, sizeof(cmdline));
    memset(avline, 0, sizeof(avline));
    lineptr = cmdline;
    avptr = avline;

    memset(infile, 0, sizeof(infile));
    memset(outfile, 0, sizeof(outfile));
    
    cmd_count = 0;
    backgnd = 0;
    append = 0;
    lastpid = 0;

    printf("[OS shell:%s]$", getpath());
    fflush(stdout);

	if (cmdcap == 0) {
		cmdall = (char **) malloc(BUFSIZ);
		cmdcap = BUFSIZ / sizeof(char*);
	}
}

char *getpath()
{
	if (getcwd(workpath, PSIZ-1) == NULL) {
		printf("get work path error\n");
	}

	return workpath;
}

/* free all command lines history */
void free_commands()
{
	char **p = cmdall;

	while (*p != NULL)
		free(*p++);

	free(cmdall);
}
