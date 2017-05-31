#include "mlist.h"

#include <stdio.h>

#define MAXHEADER   10
#define MAXSIZE     500

LIST header[MAXHEADER];
ListNode nodes[MAXSIZE];

int ptr_header = 0, ptr_node = 0;

LIST *ListCreate()
{
    if (ptr_header >= MAXHEADER) {
        return NULL;
    }

    header[ptr_header].head = ptr_node;
    header[ptr_header].tail = ptr_node+1;
    header[ptr_header].current = ptr_node;
    header[ptr_header].count = 0;

    nodes[ptr_node].next = ptr_node+1;
    nodes[ptr_node].prev = ptr_node;
    nodes[ptr_node].data = NULL;

    nodes[ptr_node+1].prev = ptr_node;
    nodes[ptr_node+1].next = ptr_node+1;
    nodes[ptr_node+1].data = NULL;

    ptr_node += 2; // dummy node

    return &(header[ptr_header++]);
}

int ListCount(LIST *list)
{
    return list->count;
}

void *ListFirst(LIST *list)
{
    if (list->count > 0) {
        list->current = nodes[list->head].next;
        return nodes[list->current].data;
    } else {
        return NULL;
    }
}

void *ListLast(LIST *list)
{
    if (list->count > 0) {
        list->current = nodes[list->tail].prev;
        return nodes[list->current].data;
    } else {
        return NULL;
    }
}

void *ListNext(LIST *list)
{
    list->current = nodes[list->current].next;

    if (list->current == list->tail) return NULL;

    return nodes[list->current].data;
}

void *ListPrev(LIST *list)
{
    list->current = nodes[list->current].prev;

    if (list->current == list->head) return NULL;

    return nodes[list->current].data;
}

void *ListCurr(LIST *list)
{
    if (list->count > 0) {
        return nodes[list->current].data;
    }
    return NULL;
}

int ListAdd(LIST *list, void *item)
{
    if (ptr_node >= MAXSIZE) return -1;

    nodes[ptr_node].prev = list->current;
    nodes[ptr_node].next = nodes[list->current].next;

    nodes[nodes[list->current].next].prev = ptr_node;
    nodes[list->current].next = ptr_node;

	nodes[ptr_node].data = item;

    list->current = ptr_node;

    list->count++;
    ptr_node++;

    return 0;
}

int ListInsert(LIST *list, void *item)
{
    if (ptr_node >= MAXSIZE) return -1;

    nodes[ptr_node].next = list->current;
    nodes[ptr_node].prev = nodes[list->current].prev;
	nodes[ptr_node].data = item;

    nodes[nodes[list->current].prev].next = ptr_node;
    nodes[list->current].prev = ptr_node;

    list->current = ptr_node;

    list->count++;
    ptr_node++;

    return 0;
}

int ListAppend(LIST *list, void *item)
{
    if (ptr_node >= MAXSIZE) return -1;

    nodes[ptr_node].next = list->tail;
    nodes[ptr_node].prev = nodes[list->tail].prev;
	nodes[ptr_node].data = item;

    nodes[nodes[list->tail].prev].next = ptr_node;
    nodes[list->tail].prev = ptr_node;

    list->current = ptr_node;

    list->count++;
    ptr_node++;

    return 0;
}

int ListPrepend(LIST *list, void *item)
{
    if (ptr_node >= MAXSIZE) return -1;

    nodes[ptr_node].prev = list->head;
    nodes[ptr_node].next = nodes[list->head].next;
	nodes[ptr_node].data = item;

    nodes[nodes[list->head].next].prev = ptr_node;
    nodes[list->head].next = ptr_node;

    list->current = ptr_node;

    list->count++;
    ptr_node++;

    return 0;

}

void *ListRemove(LIST *list)
{
    void *ret = nodes[list->current].data;

    if (list->current == list->head || list->current == list->tail)
        return NULL;

    nodes[nodes[list->current].prev].next = nodes[list->current].next;
    nodes[nodes[list->current].next].prev = nodes[list->current].prev;

    list->current = nodes[list->current].next;
    list->count--;

    return ret;
}

void ListConcat(LIST *list1, LIST *list2)
{
    nodes[nodes[list1->tail].prev].next = nodes[list2->head].next;
    nodes[nodes[list2->head].next].prev = nodes[list1->tail].prev;

    list1->count += list2->count;
    list1->tail = list2->tail;
}

void ListFree(LIST *list, void (*itemFree)(void *item))
{
    int p = nodes[list->head].next;
    while (p != list->tail) {
        itemFree(nodes[p].data);
        p = nodes[p].next;
    }
    list->count = 0;
}

void *ListTrim(LIST *list)
{
    void *ret;
    if (list->count <= 0) return NULL;
    ret = nodes[nodes[list->tail].prev].data;

    if (list->current == list->tail) {
        list->current = nodes[list->tail].prev;
    }

    list->tail = nodes[list->tail].prev;
    nodes[list->tail].next = list->tail;

    return ret;
}

void *ListSearch(LIST *list, 
        int (*comparator)(void *item1, void *item2), void *arg)
{
    int p = list->current;
    while (p != list->tail) {
        if (comparator(nodes[p].data, arg) == 0) {
            list->current = p;
            return nodes[p].data;
        }
        p = nodes[p].next;
    }

    return NULL;
}

