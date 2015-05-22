#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/wait.h>
#include 	<fcntl.h>

#include 	"simsh.h"

/* execute.c - execute the command */

#define MAXARGS	10	// each command with max 10 args, including redirection
#define MAXCMDS	5	// max commands number

typedef struct cmdlist {
	char *args[MAXARGS]; // command list
	int ifd;
	int ofd;
} cmdslist;

struct cmdlist cmd_list[MAXCMDS];

void clear_cmdlist()
{
	int i, j;

	for (i = 0; i < MAXCMDS; i++) {
		for (j = 0; j < MAXARGS; j++)
			cmd_list[i].args[j] = NULL;
		cmd_list[i].ifd = 0;
		cmd_list[i].ofd = 1;
	}
}

/*
 * commands &
 * @return int 0: no &, 1: background
 */
int command_bg(char *argv[]) 
{
	int bg = 0;

	char **p = argv;
	char **last;

	while (*p != NULL) {
		last = p;
		p++;
	}

	if (strcmp(*last, "&") == 0) {
		bg = 1;
		*last = NULL; //remove the '&'
	}

	return bg;
}

/*
 * complex commands:(space should exist before and after "|")
 *  cat < 1.c | grep "main" > 2.c
 */
int command_pipe(char *argv[]) 
{
	int cmd_num = 0, i = 0;

	char **p = argv;

	clear_cmdlist();

	while (*p != NULL) {
		if (strcmp(*p, "|") == 0) {
			cmd_num++;
			i = 0;
			p++;
			if (cmd_num >= MAXCMDS) break;
		}

		while (*p != NULL && strcmp(*p, "|") != 0) {
			cmd_list[cmd_num].args[i++] = *p++;

			if (i >= MAXARGS) {
				fprintf(stdout, "too many args of command.\n");
				return -1;
			}

			if (*p != NULL && strcmp(*p, "<") == 0) {
				p++;
				if (*p != NULL) {
					cmd_list[cmd_num].ifd = open(*p, O_RDONLY);
					p++;
				}
			}

			if (*p != NULL && strcmp(*p, ">") == 0) {
				p++;
				if (*p != NULL) {
					cmd_list[cmd_num].ofd = open(*p, O_WRONLY|O_CREAT, 0666);
					p++;
				}
			}
		}

		cmd_list[cmd_num].args[i] = NULL;
	}

	return cmd_num;
}

void show_command(int cmd_num)
{
	int i;
	for (i = 0; i <= cmd_num; i++) {
		char **p = cmd_list[i].args;
		while (*p != NULL) {
			printf("[%s],", *p++);
		}
		printf("\n");
	}
}

int execute(char *argv[])
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 */
{
	int *pid;
	int	child_info = -1;

	if ( argv[0] == NULL )		/* nothing succeeds	*/
		return 0;

	int inBg = command_bg(argv);

	int cmd_num = command_pipe(argv);
	int fds[2], fd, i;

	pid = (int *) calloc(cmd_num+1, sizeof(int));

	//show_command(cmd_num);

	for (i = 0; i <= cmd_num; i++) {
		if (i < cmd_num) {
			pipe(fds);
			cmd_list[i].ofd = fds[1];
			cmd_list[i+1].ifd = fds[0];
		}
		
		/* fork begin */

		if ( (pid[i] = fork())  == -1 )
			perror("fork");
		else if ( pid[i] == 0 ) {
			//signal(SIGINT, SIG_DFL);
			//signal(SIGQUIT, SIG_DFL);
            //signal(SIGTSTP, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

			//if (i == 0) setpgid(0, 0);

			if (cmd_list[i].ifd != 0) {
				close(0);
				dup(cmd_list[i].ifd);
			}

			if (cmd_list[i].ofd != 1) {
				close(1);
				dup(cmd_list[i].ofd);
			}

			//execvp(argv[0], argv);
			execvp(cmd_list[i].args[0], cmd_list[i].args);

			perror("cannot execute command");

			exit(1);
		}
		else {
			/*
			if ( wait(&child_info) == -1 )
				perror("wait");
			*/
		}

		setpgid(pid[i], pid[0]);
		/* fork end */

		if ((fd = cmd_list[i].ifd) != 0) close(fd);
		if ((fd = cmd_list[i].ofd) != 1) close(fd);
	}

	/* add job to jobset */
	struct job* newJob = (struct job*) malloc(sizeof(struct job));
	newJob->jobId = 1;
	newJob->pgrp = pid[0];
	newJob->next = NULL;
	newJob->stopped = 0;

	struct job* pjob;
	for (pjob = myJobSet.head; pjob; pjob = pjob->next)
		if (pjob->jobId >= newJob->jobId)
			newJob->jobId = pjob->jobId + 1;

	if (!myJobSet.head) myJobSet.head = newJob;
	else {
		for (pjob = myJobSet.head; pjob->next; pjob = pjob->next);
		pjob->next = newJob;
	}

	if (inBg) {
		printf("[%d] %d\n", newJob->jobId, newJob->pgrp);
	} else {
		myJobSet.fg = newJob;

		if (tcsetpgrp(0, newJob->pgrp))
			perror("tcsetpgrp");
	}

	for (i = 1; i <= cmd_num; i++) {
		waitpid(pid[i], &child_info, WUNTRACED);
	}

	free(pid); /* free pid array */

	return child_info;
}
