/* builtin.c
 * contains the functions for builtin commands
 */

#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include    <errno.h>
#include    <stdlib.h>

#include 	<signal.h>
#include	"simsh.h"

int assign(char *);
int okname(char *);

void showcmdhistory();

int builtin_command(char **args, int *resultp)
/*
 * purpose: run a builtin command 
 * returns: 1 if args[0] is builtin, 0 if not
 * details: test args[0] against all known builtins.  Call functions
 */
{
	int rv = 0;
	char *statusString;
	struct job *job;
	int jobNum;

	if ( strcmp(args[0],"cd") == 0 ){	     /* 'cd' command? */
		*resultp = 0;

		char *newdir;

		if (args[1] != NULL) {
			if (strcmp(args[1], "-") == 0) { /* goto previous path */
				newdir = ppath;
				printf("%s\n", newdir);
			}
			else {
				newdir = args[1];
			}

			if (chdir(newdir) == -1) {
				fprintf(stderr, "%s:%d: chdir <%s> failed: %s\n",
						__FILE__, __LINE__, args[1], strerror(errno));
			}
			else {
				getprompt(newdir);

				sprintf(ppath, "%s", cpath);
				sprintf(cpath, "%s", newdir);
			}
		}

		rv = 1;
	}
	else if ( strcmp(args[0], "history") == 0 ){   /* history cmd */
		showcmdhistory();
		rv = 1;
	}
	else if ( strcmp(args[0], "exit") == 0 ){ /* exit command */
		rv = 1;
		exit(0); // exit shell
	}
	else if ( strcmp(args[0], "jobs") == 0) { /* job list */

		for (job = myJobSet.head; job; job = job->next) {

			if (job->stopped) statusString = "Stopped";
			else statusString = "Running";

			printf(JOB_STATUS_FORMAT, job->jobId, statusString);
		}

		rv = 1;
	}
	else if ( strcmp(args[0], "fg") == 0
			|| strcmp(args[0], "bg") == 0 ) {
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

		job->stopped = 0;

		/* restart */
		kill(-job->pgrp, SIGCONT);

		rv = 1;
	}

	return rv;
}

int assign(char *str)
/*
 * purpose: execute name=val AND ensure that name is legal
 * returns: -1 for illegal lval, or result of VLstore 
 * warning: modifies the string, but retores it to normal
 */
{
	char	*cp;
	int	rv ;

	cp = strchr(str,'=');
	*cp = '\0';
	//rv = ( okname(str) ? VLstore(str,cp+1) : -1 );
	*cp = '=';
	return rv;
}
int okname(char *str)
/*
 * purpose: determines if a string is a legal variable name
 * returns: 0 for no, 1 for yes
 */
{
	char	*cp;

	for(cp = str; *cp; cp++ ){
		if ( (isdigit(*cp) && cp==str) || !(isalnum(*cp) || *cp=='_' ))
			return 0;
	}
	return ( cp != str );	/* no empty strings, either */
}

/* print history command: one by line */
void showcmdhistory()
{
	int i = 0;
	char **p = cmdhistory;

	while (*p != NULL) {
		printf("%4d: %s\n", ++i, *p++);
	}
}
