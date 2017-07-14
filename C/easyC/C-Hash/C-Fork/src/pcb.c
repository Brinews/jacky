#include "pcb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mlist.h"

static int gpid = 0;

SEM *gsem[5] = {NULL}; // total 5 semphores

PCB *create_pcb(int prio)
{
    PCB *p = (PCB *) malloc(sizeof(PCB));
    p->pid = gpid++;
    p->priority = prio;
    p->hasMsg = 0;

    return p;
}

PCB *fork_pcb(PCB *q)
{
    PCB *p = (PCB *) malloc(sizeof(PCB));
    p->pid = gpid++;
    p->priority = q->priority;
    p->hasMsg = 0;

    return p;
}

void destroy_pcb(PCB *p)
{
    if (p != NULL) free(p);
}

MSG *create_msg(int from, int to, char *msg)
{
    MSG *m = (MSG *) malloc(sizeof(MSG));
    m->from_pid = from;
    m->to_pid = to;
    strncpy(m->mesg, msg, 40);

	return m;
}

void destroy_msg(MSG *m)
{
    if (m) free(m);
}

SEM *get_sem(int sid)
{
    if (gsem[sid] == NULL) {
        gsem[sid] = (SEM *) malloc(sizeof(SEM));
        gsem[sid]->val = 0;
        gsem[sid]->semQ = ListCreate();
    }

    return gsem[sid];
}

LIST *get_semQ(int sid)
{
    if (gsem[sid] != NULL) return gsem[sid]->semQ;
    return NULL;
}

void free_sem()
{
    int i = 0;
    PCB *p;
    for (i = 0; i < 5; i++) {
        if (gsem[i] != NULL) {
            while (ListFirst(gsem[i]->semQ)) {
                p = ListRemove(gsem[i]->semQ);
                destroy_pcb(p);
            }
        }

        free(gsem[i]);
    }
}
