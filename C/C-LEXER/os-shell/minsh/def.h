#ifndef _DEF_H_
#define _DEF_H_

#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(m) \
    do { \
        perror(m); \
        exit(EXIT_FAILURE); \
    } \
    while (0)

#define MAXLINE 256 // max chars in input line
#define MAXARG 20 // max args in each command
#define PIPELINE 5 // max command in a pipe
#define MAXNAME 100 // max length of filename in IO redirct

#define JOB_STATUS_FORMAT "[%d] %-26s\n"

typedef struct command {
    // the command args after parse
    char *args[MAXARG+1];
    int infd;
    int outfd;
} COMMAND;

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


extern struct jobSet myJobSet;

#endif /* _DEF_H_ */
