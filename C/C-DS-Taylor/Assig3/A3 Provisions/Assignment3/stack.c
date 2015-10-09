/*
 * Stack ADT
 * Author <<Your names here>>
 * Version September 2015

 * Implementation of a Stack using a linked-list of
 * nodes comprising "data" and "next" fields.
 *
 * YOU NEED TO COMPLETE THIS FILE. 
 */


//#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "assig_three215.h"
#include "node.h"
#include "stack.h"


// internals for the stack
struct stack_int {
	node tos;		// top-of-stack pointer
};


/*
	*	Stack
	*	initialiser function.
	*	Pre-condition: none
	*	Post-condition: the Stack variable's "tos" field is
	*					set to NULL 
	*	Informally: creates an empty stack
*/
void init_stack(stack *s)
{
    trace("stack: Initialiser starts");

	(*s) = (stack) malloc(sizeof(struct stack_int));
	(*s)->tos = NULL;

    trace("stack: Initialiser ends");
}

/*
	*	isEmptyS
	*	Emptiness test.
	*	Pre-condition: s is not NULL
	*	Post-condition: true is returned if the Stack variable is
	*					empty, false is returned otherwise
	*	Informally: indicate if the Stack contains no nodes
	*
	*	return boolean whether or not the stack is empty
*/
bool isEmptyS(stack s)
{
    trace("stack: isEmpty starts and ends");

	return s->tos == NULL;
}

/*
	*	top
	*	Find top item in stack
	*	Pre-condition: s is not NULL
	*	Post-condition: the value of the "data" field of the node
	*					at the top of the stack is returned, or
	*					the program exits with status 1 if there
	*					are no values in the stack
	*	Informally: get the value at the top of the stack
	*
	*	return (void *) the value at the top of the stack
*/
void *top(stack s)
{
    trace("stack: top starts");

	if (isEmptyS(s)) {
		fprintf(stderr,"top: empty stack");
		exit(1);
	}

    trace("stack: top ends");

	if (!isEmptyS(s)) return getDataN(s->tos);
}

/*
	*	pop
	*	Remove top item from stack
	*	Pre-condition: s is not NULL
	*	Post-condition: the node at the head of the linked-list
	*					has been deleted and the second node (if
	*					it exists) is now the head of the list,
	*					or the program exits with status 1 if
	*					there are no values in the stack
	*	Informally:	delete the value at the top of the stack
*/
void pop(stack s)
{
    trace("stack: pop starts");

	if (isEmptyS(s))
	{
		fprintf(stderr,"pop: empty stack");
		exit(1);
	}	

	trace("stack: pop ends");

	if (!isEmptyS(s)) {
		node next = getNextN(s->tos);
		s->tos = next;
	}
}

/*
	*	push
	*	Add item to top of stack
	*	Pre-condition: s is not NULL
	*	Post-condition: a new Node is created containing the
	*					the given parameter (o) and added to
	*					the start of the linked-list
	*	Informally: add a value onto the top of the stack
	*
	*	param (void *) o the value to push
*/
void push(stack s,void *o)
{
	node n;

    trace("stack: push starts");

	init_node(&n, o);

	setNextN(n, s->tos);
	s->tos = n;

    trace("stack: push ends");
}

/*
	*	toStringS
	*	Find printable form of stack
	*	Pre-condition: s is not NULL
	*	Post-condition: a character string of the printable
	*					version of the stack's contents is
	*					created and returned
	*	Informally: find a printable form for the stack
	*
	*	return (char *) a string representation of the stack's contents
 */
char *toStringS(stack k,char *f)
{
	node c;											// current node
	char *fmt = (char *)malloc(10 * sizeof(char));	// format string for display
	char *s=(char *)malloc(100*10*sizeof(char));	// string comprising the result
	
    trace("stack: toStringS starts");
	if (isEmptyS(k))	// empty stack
	{
		s = "<>";
	}
	else				// not empty -- build up string with contents
	{
		s[0] = '\0';	// string is initially empty
		sprintf(fmt, "%%s%s", f);
		c = k->tos;
		while (c != NULL)	// for each node
		{
			sprintf(s, fmt, s,*(int *)(getDataN(c)));	// add it to the string
			if (getNextN(c) != NULL)
			{
				sprintf(s, "%s, ", s);	// add a comma if not the last one
			}
			c = getNextN(c);
		}
	}

	trace("stack: toStringS ends");

	return s;
}


