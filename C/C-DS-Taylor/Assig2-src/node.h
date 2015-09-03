// KIT107 Assignment 2
/*
 * Specification for the Node ADT
 * Author Julian Dermoudy
 * Version 20/8/15
 */

#ifndef NODE_H
#define NODE_H

#include "sample.h"

struct node_int;
typedef struct node_int *node;

void init_node(node *n, sample s);
void setData(node n, sample s);
void setNextIndividual(node n1, node n2);
void setNextLocation(node n1, node n2);
sample getData(node n);
node getNextIndividual(node n);
node getNextLocation(node n);

#endif