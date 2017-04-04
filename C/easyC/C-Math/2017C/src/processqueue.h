#ifndef _PROCESSQUEUE_H
#define _PROCESSQUEUE_H

#include <stdbool.h>

struct pcb {
    int pid;
    int starttime;
    int memsize;
    int jobtime;
    int timeslice;
    int memaddr;
};

struct queue;

struct pcb* create_pcb(int, int , int, int);
void destroy_pcb(struct pcb *p);

struct queue* queue_create(int num);
void queue_destroy(struct queue *q);

bool enqueue(struct queue* q, struct pcb *p);
struct pcb dequeue(struct queue *q);

struct pcb* queue_front(struct queue *q);

/* swap out the pcb with max starttitme */
struct pcb queue_swapout(struct queue *q, int ticker);
/* insert pcb into queue order by starttime */
bool queue_insert(struct queue *q, struct pcb p);

void sort_queue(struct queue *q);

bool isempty(struct queue *q);
bool isfull(struct queue *q);

void print_pcb(struct pcb *p);
void print_queue(struct queue *q);

int queue_size();

#endif
