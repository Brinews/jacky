#include <stdio.h>

#include "list_reverse.h"

/*
 * In this function, you will be passed the head of a singly-linked list, and
 * you should reverse the linked list and return the new head.  The reversal
 * must be done in place, and you may not allocate any new memory in this
 * function.
 *
 * Params:
 *   head - the head of a singly-linked list to be reversed
 *
 * Return:
 *   Should return the new head of the reversed list.  If head is NULL, this
 *   function should return NULL.
 */

struct link* list_reverse(struct link* head) {
    if (head == NULL || head->next == NULL) return head;


    struct link *p = list_reverse(head->next);
    head->next->next = head;
    head->next = NULL;

    return p;
}

