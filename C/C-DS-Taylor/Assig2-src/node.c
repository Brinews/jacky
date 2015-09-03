// KIT107 Assignment 2
/*
 * Implementation for Node ADT
 * Author Julian Dermoudy and <<YOUR NAME>>
 * Version 2015-09-01
 */

#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "sample.h"

/*
 * Definition of node_int type
 */
struct node_int {
	sample data;
	node nextIndividual;
	node nextLocation;
};


/*
 * init_node()
 * Precondition: s is defined
 * Postcondition: n points to a new node with clone(s) in the data field and NULL in the other fields
 * Informally: 'Constructor' for node
 */
void init_node(node *n,sample s)
{
	*n = (node)malloc(sizeof(struct node_int));
	(*n)->data = clone(s);
	(*n)->nextIndividual = NULL;
	(*n)->nextLocation = NULL;
}


/*
 * getData()
 * Precondition: n is defined and not NULL
 * Postcondition: the data field's value is returned
 * Informally: Return data field
 */
sample getData(node n)
{
	return n->data;
}


/*
 * getNextIndividual()
 * Precondition: n is defined and not NULL
 * Postcondition: the nextIndividual field's value is returned
 * Informally: Return nextIndividual field
 */
node getNextIndividual(node n)
{
	return n->nextIndividual;
}


/*
 * getNextLocation()
 * Precondition: n is defined and not NULL
 * Postcondition: the nextLocation field's value is returned
 * Informally: Return nextLocation field
 */
node getNextLocation(node n)
{
	return n->nextLocation;
}


/*
 * setData()
 * Precondition: n and s are defined and not NULL
 * Postcondition: n's data field contains a clone of s
 * Informally: Set the data field
 */
void setData(node n,sample s)
{
	n->data = clone(s);
}


/*
 * setNextIndividual()
 * Precondition: n1 is defined and not NULL
 * Postcondition: n1's nextIndividual field contains n2
 * Informally: Set the nextIndividual field
 */
void setNextIndividual(node n1, node n2)
{
	n1->nextIndividual = n2;
}


/*
 * setNextLocation()
 * Precondition: n1 is defined and not NULL
 * Postcondition: n1's nextLocation field contains n2
 * Informally: Set the nextLocation field
 */
void setNextLocation(node n1, node n2)
{
	n1->nextLocation = n2;
}


