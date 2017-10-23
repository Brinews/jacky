#ifndef __STACK_H
#define __STACK_H
struct stack;
struct stack* stack_create();
void stack_free(struct stack* stack);
int stack_isempty(struct stack* stack);
void stack_push(struct stack* stack, int value);
int stack_top(struct stack* stack);
int stack_pop(struct stack* stack);


#endif

