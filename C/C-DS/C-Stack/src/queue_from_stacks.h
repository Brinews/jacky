#ifndef __QUEUE_FROM_STACKS_H
#define __QUEUE_FROM_STACKS_H

#include "stack.h"
struct queue_from_stacks {
  struct stack* s1;
  struct stack* s2;
};




struct queue_from_stacks* queue_from_stacks_create();
void queue_from_stacks_free(struct queue_from_stacks* queue);
int queue_from_stacks_isempty(struct queue_from_stacks* queue);
void queue_from_stacks_enqueue(struct queue_from_stacks* queue, int value);
int queue_from_stacks_front(struct queue_from_stacks* queue);
int queue_from_stacks_dequeue(struct queue_from_stacks* queue);


#endif

