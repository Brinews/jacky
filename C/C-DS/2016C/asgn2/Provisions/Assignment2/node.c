// KIT107 Assignment 2: Node
/*
 * Implementation for Node
 * Author Julian Dermoudy and <<INSERT YOUR NAME AND STUDENT ID HERE>>
 * Version 15/8/16
 */

//#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "node.h"


struct node_int {
    void *data;
    node next;
} node_int;

/*
 * 'Constructor' for node
 */
void init_node(node *n,void *o)
{
    *n = (node)malloc(sizeof(struct node_int));
    if (*n == NULL) 
    {
        fprintf(stderr, "malloc node error\n");
        return;
    }

    /* init node's data with o */
    (*n)->data = o;
    (*n)->next = NULL;
}

/*
 * Getter for data
 * Return data field
 */
void *get_data(node n)
{
    if (n != NULL)
    {
        /* get data field */
        return n->data;
    }
    else 
    {
        /* NULL if node is not exist */
        return NULL;
    }
}

/*
 * Getter for next
 * Return next field
 */
node get_next(node n)
{
    if (n != NULL) 
    {
        return n->next;
    }

    return NULL;
}

/*
 * Setter for data
 * Param o value to be placed into the node's data field
 */
void set_data(node n,void *o)
{
    if (n != NULL) 
    {
        n->data = o;
    }
}

/*
 * Setter for next
 * Param x value to be placed into the node's next field
 */
void set_next(node n, node x)
{
    if (n != NULL)
    {
        n->next = x;
    }
}


