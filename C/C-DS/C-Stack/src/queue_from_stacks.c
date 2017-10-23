#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"
#include "queue_from_stacks.h"

struct queue_from_stacks* queue_from_stacks_create() {
    struct queue_from_stacks *qfs = malloc(sizeof(struct queue_from_stacks));

    assert(qfs);

    qfs->s1 = stack_create();
    qfs->s2 = stack_create();

    return qfs;
}


/*
 * This function should free all of the memory allocated to a queue, including
 * the memory associated with each stack.
 *
 * Params:
 *   queue - the queue to be destroyed.  May not be NULL.  The function should
 *     exit the program with an error if queue is NULL.
 */
void queue_from_stacks_free(struct queue_from_stacks* queue) {
    assert(queue);

    stack_free(queue->s1);
    stack_free(queue->s2);

    free(queue);
}

int queue_from_stacks_isempty(struct queue_from_stacks* queue) {
    assert(queue);

    return stack_isempty(queue->s1) && stack_isempty(queue->s2);
}

void queue_from_stacks_enqueue(struct queue_from_stacks* queue, int value) {
    assert(queue);
    stack_push(queue->s1, value);
}

int queue_from_stacks_front(struct queue_from_stacks* queue) {
    assert(queue);

    while (!stack_isempty(queue->s1)) {
        stack_push(queue->s2, stack_pop(queue->s1));
    }

    int ret = stack_top(queue->s2);

    while (!stack_isempty(queue->s2)) {
        stack_push(queue->s1, stack_pop(queue->s2));
    }

    return ret;
}

int queue_from_stacks_dequeue(struct queue_from_stacks* queue) {
    assert(queue);

    while (!stack_isempty(queue->s1)) {
        stack_push(queue->s2, stack_pop(queue->s1));
    }

    int ret = stack_pop(queue->s2);

    while (!stack_isempty(queue->s2)) {
        stack_push(queue->s1, stack_pop(queue->s2));
    }

    return ret;
}

