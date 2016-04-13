/*
 * Author: cod
 * Date : 2016-03-30
 */

#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#define DELIMITERS " \n\t"
#define MAX_ARGS 64
#define MAX_BUFFER_SIZE 1024
#define MAX_PATH 255

/* Custom data types */ 
typedef struct process /* Process control block */
{
    struct process * next;      /* next process in pipeline */
    char ** argv;               /* for exec */
    pid_t pid;                  /* process ID */
    char completed;             /* true if process has completed */
    char stopped;               /* true if process has stopped */
    int status;                 /* reported status value */
} process;

typedef struct job     /* Job control block */
{
    struct job * next;          /* next active job */
    struct job * prev;          /* previous active job */
    char * command;             /* command line, used for messages */
    process * first_process;    /* list of processes in this job */
    pid_t pgid;                 /* process group ID */
    char notified;              /* true if user told about stopped job */
    struct termios tmodes;      /* saved terminal modes */
    int stdin, stdout, stderr;  /* standard i/o channels */
} job;

/* Global variables */ 
int fg_flag; /* Foreground execution flag */
pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;
job * job_list = NULL;

char path[255];
char hpath[255];
char tpath[255];

struct termios saved_attributes;

/* Function prototypes*/
job * add_job(char *);
process * add_process(job *, process *);
void cmd_parser(char **, job *);
void do_job_notification(void);
job * find_job(pid_t);
void format_job_info(job *, const char *);
void free_job(job *);
void init_shell(int);
int job_is_stopped(job *);
int job_is_completed(job *);
void launch_job(job *, int);
void launch_process(process *, pid_t, int, int, int, int);
int mark_process_status(pid_t, int);
void put_job_in_background(job *, int);
void put_job_in_foreground(job *, int);
void update_status(void);
void wait_for_job(job *);

/* shell internal functions */
void nsh_process_hist(char args[][MAX_BUFFER_SIZE], int, int);
void nsh_process_cd(char **cmd_args);
void nsh_process_envset(char **cmd_args);
void nsh_process_envunset(char **cmd_args);
void nsh_process_bg(char **cmd_args);
void nsh_process_fg(char **cmd_args);

void nsh_process_bg(char **cmd_args)
{
	if (cmd_args[1] == NULL) {
		printf("bg <pgid>\n");
	} else {
		int id = atoi(cmd_args[1]);
		job *fj = find_job(id);
		if (fj != NULL)
			put_job_in_background(fj, 1);
	}
}

void nsh_process_fg(char **cmd_args)
{
	if (cmd_args[1] == NULL) {
		printf("fg <pgid>\n");
	} else {
		int id = atoi(cmd_args[1]);
		job *fj = find_job(id);
		if (fj != NULL)
			put_job_in_foreground(fj, 1);
	}
}

void nsh_process_envunset(char **cmd_args)
{
	if(getenv(cmd_args[1])==NULL)
		printf("variable does not exist！\n");
	unsetenv(cmd_args[1]);
}

void nsh_process_envset(char **cmd_args)
{
	if(!cmd_args[1])
	{
		printf("Please input the variable name!\n");
	}
	else
	{
		setenv(cmd_args[1],cmd_args[2],1);
	}
}

void nsh_process_cd(char **cmd_args)
{
	if(!cmd_args[1])
	{
		getcwd(path,254);
		printf("%s\n",path);
	}
	else
	{
		if (!chdir(cmd_args[1]))
		{
			getcwd(path,254);
			setenv("PWD",path,1);
		}
		else 
		{
			printf("This direcrtory is unvalid\n");
		}
	}
}

void nsh_process_hist(char args[][MAX_BUFFER_SIZE], int idx, int overflow)
{
	int i;
	if (overflow == 0) {
		for (i = 0; i < idx; i++) {
			printf("%s", args[i]);
		}
	} else {
		for (i = idx; i < MAX_BUFFER_SIZE; i++) {
			printf("%s", args[i]);
		}
		for (i = 0; i <= idx; i++) printf("%s", args[i]);
	}
}

/* Main function */
int main (int argc, char ** argv)
{
    char ** argp;
    char * cmd_args[MAX_ARGS];
    char buf[MAX_BUFFER_SIZE];
	char cmdlist[MAX_BUFFER_SIZE][MAX_BUFFER_SIZE];
    char * prompt = " $ ";
	int idx = 0;
	int overflow = 0;

    init_shell(argc);

    getcwd(hpath,254);
    strcat(hpath,"/cshell");
    setenv("SHELL",hpath,1);
    getcwd(hpath,254);
    /* keep reading commands until the "exit" command or EOF is triggered */

    while (1)
    {
        /* get command line from input */

        printf("%s",getcwd(tpath,254));
		printf("%s", prompt); 
        if (fgets(buf, MAX_BUFFER_SIZE, stdin))                 // get next command line from user or batchfile
        {
			memcpy(cmdlist[idx], buf, MAX_BUFFER_SIZE);
			idx = (idx + 1)%MAX_BUFFER_SIZE;
			if (idx == 0) overflow = 1;

            do_job_notification();                              // ensure zombie processes are reaped

            /* extract tokens from command line */

            argp = cmd_args;
            *argp++ = strtok(buf, DELIMITERS);              // fetch the first token
            while ((*argp = strtok(NULL, DELIMITERS)))      // keep fetching tokens until reaching the NULL token
            {
                argp++;
            }

            if (cmd_args[0])                                // run if and only if a command has been entered
            {
                /* check and respond to command */
				if (!strcmp(cmd_args[0], "history")) {
					nsh_process_hist(cmdlist, idx, overflow);
				}
				else if (!strcmp(cmd_args[0], "cd"))
                {
					nsh_process_cd(cmd_args);
                }
                else if (!strcmp(cmd_args[0], "envset"))
                {
					nsh_process_envset(cmd_args);
                }
                else if (!strcmp(cmd_args[0], "envunset"))
                {
					nsh_process_envunset(cmd_args);
				}
                else if (!strcmp(cmd_args[0], "exit"))
                {
                   exit(0);
                }
				else if (!strcmp(cmd_args[0], "bg"))
                {
					nsh_process_bg(cmd_args);
				}
                else if (!strcmp(cmd_args[0], "fg"))
                {
					nsh_process_fg(cmd_args);
				}
                else
                {
                    struct job *newJob = add_job(cmd_args[0]);
					cmd_parser(cmd_args, newJob);
                    launch_job(newJob, fg_flag);
                }
            }
        }

        else                            // user presses control-D or EOF has been reached
        {
            puts("");                   // ensure that there is a newline after shell exits
            break;                      // break out of "while" loop to quit
        }
    }
    exit(EXIT_SUCCESS);
}

/* Create a job and add it to the job list. Return the job */
job * add_job(char * cmd)
{
    job * j = job_list; /* Pointer to global job list */
    job * k = NULL;
    
    /* Initialise a new job control block */
    k = (job *)malloc(sizeof(job));
    k->next = NULL;
    k->command = strdup(cmd);
    k->first_process = NULL;
    k->pgid = 0;
    k->notified = 0;
    k->stdin = STDIN_FILENO;
    k->stdout = STDOUT_FILENO;
    k->stderr = STDERR_FILENO;
	tcgetattr(shell_terminal, &k->tmodes);
    /* Add the new job control block to the global job list */

	while(j != NULL && j->next!=NULL)
	{
		j=j->next;
	}

    if (j == NULL) {
        k->prev = NULL;
        job_list = k;
    } else {
        j->next=k;
        k->prev=j;
    }
	return k;
}

/* Add a pre-initialised process control block and add it to the specified job list. */
/* Return the process control block */
process * add_process(job * j, process * p)
{
	process * k=j->first_process;

	while(k != NULL && k->next!=NULL) {
		k=k->next;
	}

    if (k == NULL) j->first_process = p;
    else k->next = p;

	return p; 
}

/* Command line parser */
void cmd_parser(char ** cmd_args, job * j)
{
    char ** argp = cmd_args; // Working variable for command line tokens
    char ** argvp = NULL;    // Working variable for array of program arguments for process control block */

    fg_flag = 1;
    for (; ; argp++)
    {
        /* Initialise a new process control block */
        process * p = (process *)malloc(sizeof(process));
        p->next = NULL;
        p->completed = 0;
        p->stopped = 0;
        p->status = 0;
        p->argv = (char **)malloc(sizeof(char *)*MAX_ARGS);
        argvp = p->argv;

        for(;*argp && strcmp(*argp,"&") && strcmp(*argp,"|");argp++) {
		    *argvp = strdup(*argp);
            argvp++;
        }

        *argvp = NULL;

        add_process(j,p);

        if (!*argp) {
            break; // No more tokens to process
        }

        if (!strcmp(*argp,"&")) {
            fg_flag = 0; // Flag that the program should run in the background
            break;
        }
    }
}

/* Notify the user about stopped or terminated jobs.
 Delete terminated jobs from the active job list. */
void do_job_notification(void)
{
    job *j, *jlast, *jnext;
    
    /* Update status information for child processes. */
    update_status();
    
    jlast = NULL;
    for (j = job_list; j; j = jnext)
    {
        jnext = j->next;
        
        if (job_is_completed(j))
        {
            format_job_info(j, "finished");
            if (jlast) {
                jlast->next = jnext;
            }
            else {
                job_list = jnext;
            }
            free_job(j);
        } else if (job_is_stopped(j) && !j->notified) {
            format_job_info(j, "stopped");
            j->notified = 1;
            jlast = j;
        } else {
            jlast = j;
        }
    }
}

/* Find the active job with the indicated pgid. */
job * find_job(pid_t pgid)
{
    job * j;

    for (j = job_list; j; j = j->next)
    {
        if (j->pgid == pgid)
        {
            return j;
        }
    }
    return NULL;
}

/* Format information about job status for the user to look at. */
void format_job_info(job *j, const char *status)
{
    fprintf(stderr, "%ld (%s): %s\n", (long)j->pgid, status, j->command);
}

void free_job(job * j)
{
    process *p;

    if (!j)
    {
        return;
    }
    if (j->next)
    {
        j->next->prev = j->prev;
    }
    if (j->prev)
    {
        j->prev->next = j->next;
    }
    else
    {
        job_list = j->next;
    }

    for (p = j->first_process; p;)
    {
        process * q = NULL;
        char **argvp;
        for (argvp = p->argv; *argvp; argvp++)
        {
            free(*argvp);
        }
        free(p->argv);
        q = p;
        p = p->next;
        free(q);
    }
    if (j->command)
    {
        free(j->command);
    }
    else
    {
        fprintf(stderr, "WARNING: NULL pointer encountered for job name while trying to free job.\n");
    }
    free(j);
}

void init_shell(int argc)
{
    /* Set command input source */
    if (argc > 1)
    {
        fprintf(stderr, "cShell: Arguments are not supported.\n");
        exit(EXIT_FAILURE);
    }
    else if (argc != 1)
    {
        fprintf(stderr, "FATAL: Unable to process arguments\n");
        fprintf(stderr, "cShell: Unknown error\n");
        exit(EXIT_FAILURE);
    }
    
    /* See if we are running interactively. */
    shell_terminal = STDIN_FILENO;
    shell_is_interactive = isatty(shell_terminal);
    
    if (shell_is_interactive)
    {
        /* Loop until we are in the foreground. */
        while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
        {
            kill(-shell_pgid, SIGTTIN);
        }
        
        /* Ignore interactive and job-control signals. */
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        
        /* Put ourselves in our own process group. */
        shell_pgid = getpid();
        if (setpgid(shell_pgid, shell_pgid) < 0)
        {
            fprintf(stderr, "FATAL: Unable to put the shell in its own process group");
            perror("cShell");
            exit(EXIT_FAILURE);
        }
        
        /* Grab control of the terminal. */
        tcsetpgrp(shell_terminal, shell_pgid);
    }
    else
    {
        fprintf(stderr, "Sorry, this shell does not support non-interactive mode.\n");
        exit(EXIT_FAILURE);
    }
    
    /* Save default terminal attributes for shell. */
    tcgetattr(shell_terminal, &shell_tmodes);
}

/* Return true if all processes in the job have completed. */
int job_is_completed(job * j)
{
    process *p;
    
    for (p = j->first_process; p; p = p->next)
    {
        if (!p->completed)
        {
            return 0;
        }
    }
    return 1;
}

/* Return true if all processes in the job have stopped or completed. */
int job_is_stopped(job * j)
{
    process *p;
    
    for (p = j->first_process; p; p = p->next)
    {
        if (!p->completed && !p->stopped)
        {
            return 0;
        }
    }
    return 1;
}

void launch_job(job * j, int foreground)
{
    process *p;
    pid_t pid;
    int mypipe[2], infile, outfile;
    
    infile = j->stdin;
    for (p = j->first_process; p; p = p->next)
    {
        /* Set up pipes, if necessary.  */
        if (p->next)
        {
            if (pipe(mypipe) < 0)
            {
                fprintf(stderr, "FATAL: Unable to establish pipe\n");
                perror("External command");
                exit(EXIT_FAILURE);
            }
            outfile = mypipe[1];
        }
        else
            outfile = j->stdout;
        
        /* Fork the child processes.  */
        pid = fork();
        if (pid == 0)
        /* This is the child process.  */
            launch_process(p, j->pgid, infile,
                           outfile, j->stderr, foreground);
        else if (pid < 0)
        {
            /* The fork failed.  */
            fprintf(stderr, "FATAL: Unable to fork process\n");
            perror("External command");
            exit(EXIT_FAILURE);
        }
        else
        {
            /* This is the parent process.  */
            p->pid = pid;
            if (shell_is_interactive)
            {
                if (!j->pgid)
                {
                    j->pgid = pid;
                }
                setpgid(pid, j->pgid);
            }
            else
            {
                j->pgid = getpid();
            }
        }
        
        /* Clean up after pipes.  */
        if (infile != j->stdin)
        {
            close(infile);
        }
        if (outfile != j->stdout)
        {
            close(outfile);
        }
        infile = mypipe[0];
    }
    
    format_job_info(j, "started");
    
    if (!shell_is_interactive)
    {
        wait_for_job(j);
    }
    else if (foreground)
    {
        put_job_in_foreground(j, 0);
    }
    else
    {
        put_job_in_background(j, 0);
    }
}

void launch_process(process *p, pid_t pgid,
                    int infile, int outfile, int errfile,
                    int foreground)
{
    pid_t pid;
    
    if (shell_is_interactive)
    {
        /* Put the process into the process group and give the process group
         the terminal, if appropriate.
         This has to be done both by the shell and in the individual
         child processes because of potential race conditions.  */
        pid = getpid();
        if (pgid == 0)
        {
            pgid = pid;
        }
        setpgid(pid, pgid);
        if (foreground)
        {
            tcsetpgrp(shell_terminal, pgid);
        }
        
        /* Set the handling for job control signals back to the default.  */
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGCHLD, SIG_DFL);
    }
    
    /* Set the standard input/output channels of the new process. */
    if (infile != STDIN_FILENO)
    {
        dup2(infile, STDIN_FILENO);
        close(infile);
    }
    if (outfile != STDOUT_FILENO)
    {
        dup2(outfile, STDOUT_FILENO);
        close(outfile);
    }
    if (errfile != STDERR_FILENO)
    {
        dup2(errfile, STDERR_FILENO);
        close(errfile);
    }
    
    /* Exec the new process.  Make sure we exit. */
    execvp(p->argv[0], p->argv);
    fprintf(stderr, "ERROR: Unable to run external command\n");
    perror(p->argv[0]);
    exit(EXIT_FAILURE);
}

/* Store the status of the process pid that was returned by waitpid.
 Return 0 if all went well, nonzero otherwise. */
int mark_process_status(pid_t pid, int status)
{
    job *j;
    process *p;
    if (pid > 0)
    {
        /* Update the record for the process. */
        for (j = job_list; j; j = j->next) 
            for (p = j->first_process; p; p = p->next) 
                if (p->pid == pid)
                {
                    p->status = status;
                    if (WIFSTOPPED(status))
                    {
                        p->stopped = 1;
                    }
                    else
                    {
                        p->completed = 1;
                        if (WIFSIGNALED(status))
                        {
                            fprintf(stderr, "%d: Terminated by signal %d.\n",
                                    (int) pid, WTERMSIG (p->status));
                        }
                    }
                    return 0;
                }
        fprintf(stderr, "No child process %d.\n", pid);
        return -1;
    }
    else if (pid == 0 || errno == ECHILD)
    {
        /* No processes ready to report. */
        return -1;
    }
    else
    {
        /* Other weird errors. */
        perror("waitpid");
        return -1;
    }
}

/* Put a job in the background.  If the cont argument is true, send
 the process group a SIGCONT signal to wake it up. */
void put_job_in_background(job *j, int cont)
{
    process *p;
    j->notified = 0; // Enable job suspension notifications
    for (p = j->first_process; p; p = p->next)
    {
        p->completed = 0;
        p->stopped = 0;
        p->status = 0;
    }
    /* Send the job a continue signal, if necessary. */
    if (cont)
    {
        if (kill(-j->pgid, SIGCONT) < 0)
        {
            perror("kill (SIGCONT)");
        }
    }
}

/* Put job j in the foreground.  If cont is nonzero,
 restore the saved terminal modes and send the process group a
 SIGCONT signal to wake it up before we block. */
void put_job_in_foreground(job * j, int cont)
{
    process *p;
    j->notified = 0; // Enable job suspension notifications
    for (p = j->first_process; p; p = p->next)
    {
        p->completed = 0;
        p->stopped = 0;
        p->status = 0;
    }
    /* Put the job into the foreground. */
    tcsetpgrp(shell_terminal, j->pgid);
    /* Send the job a continue signal, if necessary. */
    if (cont)
    {
        tcsetattr(shell_terminal, TCSADRAIN, &j->tmodes);
        if (kill(-j->pgid, SIGCONT) < 0)
        {
            perror("kill (SIGCONT)");
        }
    }
    /* Wait for it to report. */
    wait_for_job(j);
    
    /* Put the shell back in the foreground. */
    tcsetpgrp(shell_terminal, shell_pgid);
    
    /* Restore the shell's terminal modes. */
    tcgetattr(shell_terminal, &j->tmodes);
    tcsetattr(shell_terminal, TCSADRAIN, &shell_tmodes);
}

void update_status(void)
{
    int status;
    pid_t pid;
    
    do
    {
        pid = waitpid(WAIT_ANY, &status, WUNTRACED|WNOHANG);
    } while (!mark_process_status(pid, status));
}

void wait_for_job(job * j)
{
    int status;
    pid_t pid;
    
    do
    {
        pid = waitpid(-j->pgid, &status, WUNTRACED);
    } while (!mark_process_status(pid, status)
           && !job_is_stopped(j)
           && !job_is_completed(j));
}
