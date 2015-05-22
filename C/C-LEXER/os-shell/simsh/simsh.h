#ifndef _SIMSH_H
#define _SIMSH_H

#include <unistd.h>

#define	YES	1
#define	NO	0

#define MAXSIZ 256
#define JOB_STATUS_FORMAT "[%d] %-22s\n"

/* shell current path */
extern char *cpath;

/* shell previous path */
extern char *ppath;

extern char **cmdhistory;
extern int  cmdnum;
extern int  cmdspot;

char *getprompt(char *path);

char	*next_cmd();
char	**splitline(char *);
void	freelist(char **);
void	*emalloc(size_t);
void	*erealloc(void *, size_t);
int		execute(char **);
void	fatal(char *, char *, int );

void    addcmdhistory(char *cmd);
void    freecmdhistory(char **);

int	process(char **);

/*
 * Job control structure
 */
typedef struct job {
	int jobId;
	pid_t pgrp;
	struct job *next;
	int stopped;
} job;

typedef struct jobSet {
	struct job *head;
	struct job *fg;
} jobSet;

extern struct jobSet myJobSet;

#endif
