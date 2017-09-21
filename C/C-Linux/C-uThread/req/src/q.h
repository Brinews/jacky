#ifndef _Q_H
#define _Q_H

#include<stdio.h>
#include<stdlib.h>

#include "TCB.h"

//xiao liu

typedef struct Queue {
	TCB_t *val;
	struct Queue* next;
	struct Queue* prev;

} Queue;

struct Queue *newItem();
void initQueue(struct Queue **h);
void AddQueue(struct Queue** head, struct Queue* item);
void printQ(struct Queue *head);

struct Queue *newItem() {
	Queue *temp = (Queue *)malloc(sizeof(Queue));
	temp->val = NULL;
	temp->next = NULL;
	temp->prev = NULL;
	return temp;
}

void initQueue(struct Queue **h) {
	*h = NULL;
}

void AddQueue(struct Queue** head, struct Queue* item)
{
	// add to an empty queue
	if (*head == NULL) {
		*head = item;
		item->next = *head; //circular
		item->prev = *head;
	}

	// add to a non-empty queue
	else {
		struct Queue *temp = *head;
		while (temp->next != *head) {
			temp = temp->next;
		}
		temp->next = item;
		item->prev = temp;

		item->next = *head;
		(*head)->prev = item;
	}

}

struct Queue *DelQueue(struct Queue **head) {
	struct Queue *temp = *head;
	struct Queue *first = *head;
	// if only 1 element in queue, set head to NULL
	if ((*head)->next == (*head) && (*head)->prev == (*head)) {
		(*head)->next = NULL;
		(*head)->prev = NULL;
		*head = NULL;
		return temp;
	}
	//delete when it has two or more elements
	else {
		while (temp->next != *head) {
			temp = temp->next;
		}
		temp->next = (*head)->next;
		*head = (*head)->next;
		(*head)->prev = temp;
		first->next = NULL;
		first->prev = NULL;
		return first;
	}
	
}

void RotateQ(struct Queue** head) {
	AddQueue(head, DelQueue(head));
}

#ifdef DEBUG

void printQ(struct Queue *head) {
	struct Queue *temp = head;
	if (head != NULL) {
		do {
			printf("%d ", temp->val);
			temp = temp->next;
		} while (temp != head);
	}
	else {
		printf("The list is empty");
	}
}


int main()
{
	Queue *head = NULL;
	int i;
	initQueue(&head);
	Queue *item;
	for (i = 0; i < 10; i++) {
		item = newItem();
		item->val = i;
		AddQueue(&head, item);
	}
	printQ(head);  // should output: 0 1 2 3 4 5 6 7 8 9
	printf("\n");

	for (i = 0; i < 5; i++) {
		item = DelQueue(&head);
		printf("Item Deleted %d \n", item->val); // output should be 0 1 2 3 4
	}

	RotateQ(&head);
	item = DelQueue(&head);
	printf("item Deleted %d \n", item->val); // output should be 6

	return 0;
}

#endif

#endif
