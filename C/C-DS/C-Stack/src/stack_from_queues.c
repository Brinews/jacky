#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "stack.h"
#include "stack_from_queues.h"

struct stack_from_queues* stack_from_queues_create() {
    struct stack_from_queues *sfq = malloc(sizeof(struct stack_from_queues));

    assert(sfq);
    sfq->q1 = queue_create();
    sfq->q2 = queue_create();

    return sfq;
}


void stack_from_queues_free(struct stack_from_queues* stack) {
    assert(stack);

    queue_free(stack->q1);
    queue_free(stack->q2);

    free(stack);
}

int stack_from_queues_isempty(struct stack_from_queues* stack) {
    assert(stack);

    return queue_isempty(stack->q1) && queue_isempty(stack->q2);
}

void stack_from_queues_push(struct stack_from_queues* stack, int value) {
    assert(stack);
    queue_enqueue(stack->q1, value);

    while (!queue_isempty(stack->q2)) {
        queue_enqueue(stack->q1, queue_dequeue(stack->q2));
    }

    while (!queue_isempty(stack->q1)) {
        queue_enqueue(stack->q2, queue_dequeue(stack->q1));
    }
}

int stack_from_queues_top(struct stack_from_queues* stack) {
    assert(stack);

    return queue_front(stack->q2);
}

int stack_from_queues_pop(struct stack_from_queues* stack) {
    assert(stack);

    return queue_dequeue(stack->q2);
}

