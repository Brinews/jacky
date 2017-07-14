#ifndef _MLIST_H
#define _MLIST_H

typedef struct List LIST;
typedef struct Node ListNode;

struct List {
    int head;
    int tail;
    int current;
    int count;
};

struct Node {
    int prev;
    int next;
    void *data;
};

LIST *ListCreate();
int ListCount(LIST *list);
void *ListFirst(LIST *list);
void *ListLast(LIST *list);
void *ListNext(LIST *list);
void *ListPrev(LIST *list);
void *ListCurr(LIST *list);
int ListAdd(LIST *list, void *item);
int ListInsert(LIST *list, void *item);
int ListAppend(LIST *list, void *item);
int ListPrepend(LIST *list, void *item);
void *ListRemove(LIST *list);
void ListConcat(LIST *list1, LIST *list2);
void ListFree(LIST *list, void (*itemFree)(void *item));
void *ListTrim(LIST *list);
void *ListSearch(LIST *list, 
        int (*comparator)(void *item1, void *item2), void *arg);

#endif
