

#ifndef __QUEUE_H
#define __QUEUE_H

struct queue;
struct queue* queue_create();
void queue_free(struct queue* queue);


int queue_isempty(struct queue* queue);


void queue_enqueue(struct queue* queue, int value);
int queue_front(struct queue* queue);
int queue_dequeue(struct queue* queue);


#endif

