#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FREE    0
#define USED    1

struct mem {
    char *addr;
    int size;
    int type; /* 0: free, 1: allocated */
    struct mem *next;
};

struct pool {
    struct mem *list;
    char *memaddr;
    int leftsize;
    int totalsize;
};

bool pool_empty(struct pool *p)
{
    return p->leftsize == p->totalsize;
}

struct pool *pool_create(int size)
{
    struct pool *pol = (struct pool *) malloc(sizeof(struct pool));

    pol->leftsize = size;
    pol->totalsize = size;
    pol->memaddr = (char *) malloc(size);

    pol->list = (struct mem*) malloc(sizeof(struct mem));
    pol->list->addr = pol->memaddr;
    pol->list->size = size;
    pol->list->type = FREE;
    pol->list->next = NULL;

    return pol;
}

bool pool_destroy(struct pool *p)
{
    if (p->leftsize != p->totalsize) return false;

    free(p->list);
    free(p->memaddr);
    free(p);

    return true;
}

char *pool_alloc(struct pool *p, int size)
{
    if (size > p->leftsize) { /* max left space unfit */
        return NULL;
    }

    struct mem *ptr = p->list;
    struct mem *prev = NULL;
    while (ptr != NULL) {
        if (ptr->type == FREE && ptr->size >= size) {
            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }

    if (ptr == NULL) return NULL; /* no fit space */

    if (ptr->size == size) {
        ptr->type = USED;
        p->leftsize -= size;

        return ptr->addr;
    }

    /* allocate */
    struct mem *node = (struct mem *) malloc(sizeof(struct mem));
    node->addr = ptr->addr;
    node->size = size;
    node->type = USED;
    node->next = ptr;

    ptr->addr = ptr->addr+size;
    ptr->size = ptr->size-size;

    if (prev == NULL) { // first node
        p->list = node;
    } else {
        prev->next = node;
    }

    p->leftsize -= size;

    return node->addr;
}

bool pool_free(struct pool *p, char *addr)
{
    struct mem *ptr = p->list;
    struct mem *prev = NULL;

    while (ptr != NULL) {
        if (ptr->addr == addr) {
            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }

    if (ptr == NULL || ptr->type == FREE) return false;
    else {
        ptr->type = FREE;

        p->leftsize += ptr->size;

        if (ptr->next != NULL && ptr->next->type == FREE) {
            /* merge with next */
            ptr->size = ptr->size + ptr->next->size;
            struct mem *pnext = ptr->next;
            ptr->next = ptr->next->next;
            free(pnext);
        }

        if (prev != NULL && prev->type == FREE) {
            /* merge with prev */
            prev->size = prev->size+ptr->size;
            prev->next = ptr->next;
            free(ptr);
        }


        return true;
    }
}

char *pool_realloc(struct pool *p, char *addr, int size)
{
    struct mem *ptr = p->list;
    struct mem *prev = NULL;

    while (ptr != NULL) {
        if (ptr->addr == addr) {
            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }

    if (ptr == NULL || ptr->type == FREE) return NULL; /* not exist */

    if (size < ptr->size) {
        if (ptr->next != NULL && ptr->next->type == FREE) {
            /* merge with next */
            ptr->next->size += (ptr->size-size);
            ptr->next->addr = ptr->next->addr-size;
        } else {
            /* insert new free space node */
            struct mem *node = (struct mem *)malloc(sizeof(struct mem));
            node->addr = ptr->addr+size;
            node->type = FREE;
            node->size = ptr->size-size;
            node->next = ptr->next;
            ptr->next = node;
        }

        p->leftsize += ptr->size-size; /* update free size */

        ptr->size = size;

        return addr;
    } else if (size > ptr->size) {
        if (ptr->next != NULL && ptr->next->type == FREE
                && ptr->next->size >= size-ptr->size) {

            p->leftsize -= size-ptr->size; /* update free size */

            /* extend it */
            if (ptr->next->size != size-ptr->size) {
                ptr->next->addr = ptr->next->addr+(size-ptr->size);
                ptr->next->size = ptr->next->size-(size-ptr->size);
                ptr->size = size;
            } else {
                ptr->size = size;
                ptr->next = ptr->next->next;
                free(ptr->next);
            }
            return addr;
        } else {
            /* new alloc */
            char *naddr = pool_alloc(p, size);
            if (naddr != NULL) {
                pool_free(p, addr);
            }

            return naddr;
        }
    }

    /* same size, no change */
    return addr;
}

void pool_print_active(struct pool *p)
{
    printf("active: ");
    if (p->leftsize == p->totalsize) {
        printf("none\n");
    } else {
        struct mem *ptr = p->list;
        int size = 0;
        while (ptr != NULL) {
            if (ptr->type == USED) {
                printf("%d [%d]", ptr->addr-p->memaddr, ptr->size);
                size += ptr->size;
                if (size != p->totalsize-p->leftsize) {
                    printf(", ");
                }
            }
            ptr = ptr->next;
        }
        printf("\n");
    }
}

void pool_print_available(struct pool *p)
{
    printf("available: ");
    if (p->leftsize == 0) {
        printf("none\n");
    } else {
        struct mem *ptr = p->list;
        int size = 0;
        while (ptr != NULL) {
            if (ptr->type == FREE) {
                printf("%d [%d]", ptr->addr-p->memaddr, ptr->size);
                size += ptr->size;
                if (size != p->leftsize) {
                    printf(", ");
                }
            }
            ptr = ptr->next;
        }
        printf("\n");
    }
}

int hole_count(struct pool *p)
{
    int count = 0;
    if (p->leftsize == 0) {
        count = 0;
    } else {
        struct mem *ptr = p->list;
        while (ptr != NULL) {
            if (ptr->type == FREE) {
                count++;
            }
            ptr = ptr->next;
        }
    }

    return count;
}

int mem_usage(struct pool *p)
{
    double usage = (p->totalsize-p->leftsize)*100.0/p->totalsize;
    return (int)(ceil(usage));
}
