#ifndef SHELL_H  
#define SHELL_H  
  
#define SHELL_CD 		1  
#define SHELL_FORK 		2  
#define SHELL_EMPTY 	3  
#define SHELL_HISTORY 	4
  
  
int read_char(char *str);  
int parse_cmdline_args(char *args[], char *arg);  

int qsh_exit(char *args);  

char *shell_prompt(char *promptbuf);  

int qsh_process(char *arg);  

int qsh_builtin_cd(char *args[]);  
int qsh_builtin_history(char *args[]);  
void free_cmd_history();

int shell_fork(char *args[]);  

int get_cmdnum(char *cmd);  
int mutifork(char *args[]);  
  
#endif  

