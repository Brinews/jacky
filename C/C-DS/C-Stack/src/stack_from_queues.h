#ifndef __STACK_FROM_QUEUES_H
#define __STACK_FROM_QUEUES_H

#include "queue.h"
struct stack_from_queues {
  struct queue* q1;
  struct queue* q2;
};


struct stack_from_queues* stack_from_queues_create();
void stack_from_queues_free(struct stack_from_queues* stack);
int stack_from_queues_isempty(struct stack_from_queues* stack);
void stack_from_queues_push(struct stack_from_queues* stack, int value);
int stack_from_queues_top(struct stack_from_queues* stack);
int stack_from_queues_pop(struct stack_from_queues* stack);


#endif

