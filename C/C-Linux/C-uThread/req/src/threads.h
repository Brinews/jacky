#ifndef _THREAD_H
#define _THREAD_H

#include "q.h"

extern Queue *RunQ;

void start_thread(void (*function)(void))
{ 
	//allocate a stack (via malloc) of a certain size (choose 8192)
	char *stack = (char *) malloc(sizeof(char)*8192);

	//allocate a TCB (via malloc)
	TCB_t *tcb = (TCB_t *) malloc(sizeof(TCB_t));

	//call init_TCB with appropriate arguments
	init_TCB(tcb, function, stack, 8192);

	//call addQ to add this TCB into the "RunQ" which is a global header pointer
	Queue *item = newItem();
	item->val = tcb;

	AddQueue(&RunQ, item);
}

void run()
{
	ucontext_t parent;     // get a place to store the main context, for faking
    getcontext(&parent);   // magic sauce
    swapcontext(&parent, &(RunQ->val->context));  // start the first thread
}
 
void yield() // similar to run
{
	//rotate the run Q;
	ucontext_t *current = &(RunQ->val->context);
	RotateQ(&RunQ);

	//swap the context, from previous thread to the thread pointed to by RunQ
	swapcontext(current, &(RunQ->val->context));
}

#endif
