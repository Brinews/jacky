/*
 * Node
 * Specification for the Node ADT
 * Author Julian Dermoudy
 * Version September 2015
 *
 * This file is complete.
*/

#ifndef NODE_H
#define NODE_H

//#include <stdbool.h>

struct node_int;
typedef struct node_int *node;

void init_node(node *v, void *o);
void setDataN(node v, void *o);
void setNextN(node v, node n);
void *getDataN(node v);
node getNextN(node v);

#endif