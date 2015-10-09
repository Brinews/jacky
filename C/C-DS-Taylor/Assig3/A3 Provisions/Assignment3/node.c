/*
 * node ADT
 * Author <<YOUR NAMES HERE>>
 * Version September 2015
 * 
 * This file holds the Node ADT used by the Stack
 *
 * YOU NEED TO COMPLETE THIS FILE. 
*/


//#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "assig_three215.h"
#include "node.h"


// the internals of the node
struct node_int {
	void *data;		// the value to be stored in the node
	node next;		// a link to the next node in the list
};


/*
	*	Node
	*	initialiser function.
	*	Pre-condition: none
	*	Post-condition: the Node variable's "data" field ise
	*					set to the given parameter (o) and the
	*					Node variable's "next" field is set
	*					to NULL
	*	Informally: creates a linked-list node
*/
void init_node(node *n,void *o)
{
    trace("Node: Initialiser starts");

	*n = (node) malloc(sizeof(struct node_int));

	(*n)->data = o;
	(*n)->next = NULL;

    trace("Node: Initialiser ends");
}

/*
	*	getDataN
	*	Get function for variable's "data" value.
	*	Pre-condition: n is not NULL
	*	Post-condition: the value of the Node variable's "data"
	*					field is returned
	*	Informally: return the value within the node
	*
	*	return (void *) the data field of the node
*/
void *getDataN(node n)
{
    trace("TNode: getDataN starts and ends");

	return n->data;
}

/*
	*	getNextN
	*	Get function for variable's "next" value.
	*	Pre-condition: n is not NULL
	*	Post-condition: the value of the Node variable's "next"
	*					field is returned
	*	Informally: return the pointer to the next node
	*
	*	return node the next field of the node
*/
node getNextN(node n)
{
    trace("TNode: getNextN starts and ends");

	return n->next;
}

/*
	*	setDataN
	*	Set function for the variable's "data" field.
	*	Pre-condition: n is not NULL
	*	Post-condition: the variable's "data" field is altered
	*					to hold the given (o) value
	*	Informally: assign the given value to the node variable
	*
	*	param (void *) o the value to place in the node
*/
void setDataN(node n,void *o)
{
    trace("TNode: setDataN starts");

	n->data = o;

    trace("TNode: setDataN ends");
}

/*
	*	setNextN
	*	Set function for the variable's "next" field.
	*	Pre-condition: v is not NULL
	*	Post-condition: the variable's "next" field is altered
	*					to hold the given (o) value
	*	Informally: assign the given value to the node variable's
	*				next field
	*
	*	param (node) n the value to place in the node's next field
*/
void setNextN(node v, node n)
{
    trace("TNode: setNextN starts");

	v->next = n;

    trace("TNode: setNextN ends");
}
