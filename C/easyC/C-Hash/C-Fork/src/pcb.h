#ifndef _PCB_H
#define _PCB_H

#include "mlist.h"

typedef struct pcb {
    int pid;
    int priority;
    int hasMsg;
} PCB;

typedef struct msg {
    int from_pid;
    int to_pid;
    char mesg[41];
} MSG;

typedef struct sem {
    int val;
    LIST *semQ;
} SEM;


PCB *create_pcb(int prio);
PCB *fork_pcb(PCB *p);
void destroy_pcb(PCB *p);

MSG *create_msg(int from, int to, char *msg);
void destroy_msg(MSG *m);

SEM *get_sem(int sid);
LIST *get_semQ(int sid);
void free_sem();

#endif
