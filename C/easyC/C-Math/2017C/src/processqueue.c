#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "processqueue.h"

struct queue {
    int count;
    int size;
    struct pcb *array;
    int head;
    int tail;
};

struct pcb* create_pcb(int pid, int st, int ms, int jt)
{
    struct pcb *p = (struct pcb *) malloc(sizeof(struct pcb));

    p->pid = pid;
    p->starttime = st;
    p->memsize = ms;
    p->jobtime = jt;
    p->timeslice = 0;
    p->memaddr = NULL;

    return p;
}

void destroy_pcb(struct pcb *p)
{
    free(p);
}

struct queue* queue_create(int num)
{
    struct queue *q = (struct queue *) malloc(sizeof(struct queue));

    q->array = (struct pcb *)malloc(sizeof(struct pcb) * num);
    q->count = 0;
    q->size = num;
    q->head = q->tail = 0;

    return q;
}

void queue_destroy(struct queue *q)
{
    free(q->array);
    free(q);
}

bool isfull(struct queue *q)
{
    return q->count == q->size;
}

bool isempty(struct queue *q)
{
    return q->head == q->tail;
}


bool enqueue(struct queue *q, struct pcb *p)
{
    if (isfull(q)) {
        printf("queue is full.\n");
        return false;
    }

    q->array[q->tail] = *p;
    q->tail = (q->tail+1)%q->size;
    q->count++;

    return true;
}

struct pcb dequeue(struct queue *q)
{
    struct pcb ret;

    if (isempty(q)) {
        printf("Queue is empty.");
        return ret;
    }

    ret = q->array[q->head];
    q->head = (q->head+1)%q->size;
    q->count--;

    return ret;
}

struct pcb *queue_front(struct queue *q)
{
    if (isempty(q)) return NULL;
    return &q->array[q->head];
}

void print_pcb(struct pcb *p)
{
    printf("pid:%d, start:%d, jobtime:%d, mem:%d, slice=%d\n", 
            p->pid, p->starttime,
            p->jobtime, p->memsize, p->timeslice);
}

void print_queue(struct queue *q)
{
    int cnt = q->count;
    int idx = q->head;
    while (cnt > 0) {
        print_pcb(&q->array[idx]);
        idx = (idx+1)%q->size;
        cnt--;
    }
}

/*
 * sort the jobs by starttime and priority
 */
void sort_queue(struct queue *q)
{
    for (int i = 0; i < q->count; i++) {
        for (int j = i+1; j < q->count; j++) {
            if (q->array[j].starttime <= q->array[i].starttime 
                    && q->array[j].pid < q->array[i].pid) {
                // swap
                struct pcb temp = q->array[i];
                q->array[i] = q->array[j];
                q->array[j] = temp;
            }
        }
    }
}

int queue_size(struct queue *q)
{
    return q->count;
}

/* swap out the pcb with max starttitme */
struct pcb queue_swapout(struct queue *q, int ticker)
{
    int mintime = ticker+1, pos = q->head;
    int ptr = q->head;
    while (ptr != q->tail) {
        if (q->array[ptr].starttime < mintime) {
            mintime = q->array[ptr].starttime;
            pos = ptr;
        }
        ptr = (ptr+1)%q->size;
    }

    struct pcb ret = q->array[pos];

    int next;
    while (pos != q->tail) {
        next = (pos+1)%q->size;
        q->array[pos] = q->array[next];
        pos = next;
    }

    q->tail = (q->tail-1+q->size)%q->size;
    q->count--;

    return ret;
}

/* insert pcb into queue order by starttime */
bool queue_insert(struct queue *q, struct pcb p)
{
    int ptr = (q->tail-1+q->size)%q->size;
    int next;
    while (ptr != ((q->head-1+q->size)%q->size)) {
        if (p.starttime < q->array[ptr].starttime
                || (p.starttime == q->array[ptr].starttime 
                    && p.pid < q->array[ptr].pid)) {
            next = (ptr+1)%q->size;
            q->array[next] = q->array[ptr];
        } else {
            next = (ptr+1)%q->size;
            q->array[next] = p;
            break;
        }
        ptr = (ptr-1+q->size)%q->size;
    }

    if (ptr == (q->head-1+q->size)%q->size) {
        q->array[q->head] = p; // insert at head
    } 

    q->tail = (q->tail+1)%q->size;
    q->count++;

    return true;
}

