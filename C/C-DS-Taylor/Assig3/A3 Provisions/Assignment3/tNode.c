/*
 *	Tree Node ADT
 *	author <<Your names here>>
 *	version September 2015
 *	
 *	This file holds the Tree Node ADT which represents
 *	the nodes in a doubly-linked general tree.  Tree
 *	nodes consist of a "data" field, a level number
 *	("level"), and two references to other nodes
 *	(these being the child node ("child") and the
 *	next-eldest sibling node ("sibling")).
 *	
 *	YOU NEED TO COMPLETE THIS FILE.
*/


//#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "assig_three215.h"
#include "tNode.h"


// the internals of the tree node
struct tNode_int
{
	void *data;		// the value stored in the node
	int level;		// the level of the current node
	tNode child;	// the eldest child node of the current node
	tNode sibling;	// the next eldest node of the current node
};


/*
	*	TNode
	*	Initialiser function
	*	Pre-condition: none
	*	Post-condition: the TNode variable holds the parameter value
	*					(o) within its "data" field, the parameter
	*					value (l) within its "level" field, and its
	*					"child", and "sibling" fields are null
	*	Informally: intialises the fields of the newly created TNode
	*				variable to hold the given parameters and to
	*				terminate the "child" and "sibling" fields
	*
	*	param (void *) o the value to store in the data field of the node
	*	param (int) l the level number of the created node
*/
void init_TNode(tNode *tp, void *o, int l)
{
    trace("TNode: Initialiser starts");

	*tp = (tNode) malloc(sizeof(struct tNode_int));
	(*tp)->data = o;
	(*tp)->level = l;
	(*tp)->child = NULL;
	(*tp)->sibling = NULL;

    trace("TNode: Initialiser ends");
}


/*
	*	setTNData
	*	Set function for "data" field.
	*	Pre-condition: t is not NULL
	*	Post-condition: the TNode varable's data field is altered to
	*					hold the given (o) value
	*	Informally: assign the value of the parameter to the TNode
	*				variable's "data" field
	*
	*	param (void *) o the variable to store in the data field of the node
*/
void setTNData(tNode t,void *o)
{
    trace("setTNData: setTNData starts");

	t->data = o;

    trace("setTNData: setTNData ends");
}
	
	
/*
	*	setTNLevel
	*	Set function for "level" field.
	*	Pre-condition: t is not NULL
	*	Post-condition: the TNode variable's level field is altered to
	*					hold the given (l) value
	*	Informally: assign the value of the parameter to the TNode
	*				variable's "level" field
	*
	*	param int l the level number of the node
*/
void setTNLevel(tNode t,int l)
{
    trace("setTNLevel: setTNLevel starts");

	t->level = l;

    trace("setTNLevel: setTNLevel ends");
}
	
	
/*
	*	setTNChild
	*	Set function for "child" field.
	*	Pre-condition: t is not NULL
	*	Post-condition: the TNode variable's child field is altered
	*					to hold the given (n) value
	*	Informally: assign the value of the parameter to the TNode
	*				variable's "child" field
	*
	*	param n the node to set as the child of the current node
*/
void setTNChild(tNode t,tNode n)
{
    trace("setTNChild: setTNChild starts");

	t->child = n;

    trace("setTNChild: setTNChild ends");
}
	
	
/*
	*	setTNSibling
	*	Set function for "sibling" field.
	*	Pre-condition: t is not NULL
	*	Post-condition: the TNode variable's sibling field is altered
	*					to hold the given (n) value
	*	Informally: assign the value of the parameter to the TNode
	*				variable's "sibling" field
	*
	*	param tNode n the node to set as the sibling of the current node
*/
void setTNSibling(tNode t,tNode n)
{
    trace("setTNSibling: setTNSibling starts");

	t->sibling = n;

    trace("setTNSibling: setTNSibling ends");
}
	
	
/*
	*	getTNData
	*	Get function for "data" field.
	*	Pre-condition: t is not NULL
	*	Post-condition: the TNode variable's data field is returned
	*	Informally: examine the TNode variable's "data" field
	*				returning its value
	*
	*	return (void *) the item in the node
*/
void *getTNData(tNode t)
{
    trace("getTNData: getTNData starts and ends");

	return t->data;
}
	
	
/*
	*	getTNLevel
	*	Get function for "level" field.
	*	Pre-condition: t is not NULL
	*	Post-condition: the TNode variable's level field is
	*				returned
	*	Informally: examine the TNode variable's "level"
	*				field returning its value
	*
	*	return int the level number of the current node
*/
int getTNLevel(tNode t)
{
    trace("getTNLevel: getTNLevel starts and ends");

	return t->level;
}
	
	
/*
	*	getTNChild
	*	Get function for "child" field.
	*	Pre-condition: t is not NULL
	*	Post-condition: the TNode variable's child field is
	*					returned
	*	Informally: examine the TNode variable's "child"
	*				field returning its value
	*
	*	return tNode the child of the current node
*/
tNode getTNChild(tNode t)
{
    trace("getTNChild: getTNChild starts and ends");

	return t->child;
}
	
	
/*
	*	getTNSibling
	*	Get function for "sibling" field.
	*	Pre-condition: t is not NULL
	*	Post-condition: the TNode variable's "sibling" field is
	*					returned
	*	Informally: examine the TNode variable's "sibling"
	*				field returning its value
	*
	*	return tNode the sibling of the current node
*/
tNode getTNSibling(tNode t)
{
    trace("getTNSibling: getTNSibling starts and ends");

	return t->sibling;
}


