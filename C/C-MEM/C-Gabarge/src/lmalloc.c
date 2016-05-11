#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>

#define ALIGNMASK 7
#define ALIGN(s) (((s) + ALIGNMASK) & ~ALIGNMASK)
#define POFF ALIGN(sizeof(struct alloc_hdr))
#define MINSIZ ALIGN(sizeof(struct alloc_hdr *))
#define C2P(c) ((char *)(c) + POFF)
#define P2C(p) ((struct alloc_hdr *)((char *)(p) - POFF))
#define ISADJ(c1,c2) ((struct alloc_hdr *)(C2P(c1) + (c1)->chunk_size) == (struct alloc_hdr *)(c2))

#define MAXSIZE (2<<26)

struct alloc_hdr {
	unsigned int chunk_size; /* after align size or available size */
	unsigned int alloc_size; /* actual requested size */
	struct alloc_hdr *next;
};

struct memory {
	struct alloc_hdr *tail;
};

struct alloc_list {
	void *ptr;
	int status;
	struct alloc_list *next;
};

/* global var */
struct memory s = { NULL };
void *lower, *upper;
struct alloc_list head = {NULL, 0, NULL};

struct alloc_list *find_list(void *p)
{
	struct alloc_list *ptr = head.next;

	while (ptr != NULL) {
		if (ptr->ptr == p) return ptr;
		ptr = ptr->next;
	}

	return NULL;
}

void mem_init()
{
	size_t chunk_size = MAXSIZE;

	void *p = NULL;
	//p = malloc(MAXSIZE);
    p = mmap(NULL, MAXSIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

	s.tail = p;
	s.tail->chunk_size = chunk_size - POFF;
	s.tail->next = s.tail;

	lower = p;
	upper = (char *)p + MAXSIZE;
}

/* request size is chunk_size */
void *allocate(size_t chunk_size, size_t *info)
{
	struct alloc_hdr *c1, *c2, *c3;
	void *m;
	struct alloc_list *found = NULL;
	size_t request = chunk_size;
	char *stop;

	if (s.tail == NULL) mem_init();

	chunk_size = ALIGN(chunk_size+1);
	*info = 0;

	c1 = s.tail;
	while (c1->next->chunk_size < chunk_size) {
		if (c1->next == s.tail) {
			return NULL;
		}
		c1 = c1->next;
	}

	c2 = c1->next;
	if (c2->chunk_size > (POFF + chunk_size)) { /* split new alloc_hdr */
		c3 = (struct alloc_hdr *)(C2P(c2) + chunk_size);
		c3->chunk_size = c2->chunk_size - (chunk_size + POFF); /* remind space */
		c3->alloc_size = 0;
		c3->next = c2->next;
		c2->chunk_size = chunk_size; /* already allocate space */
		c2->alloc_size = request;
		c1->next = c3;
	} else { /* use the entire alloc_hdr */
		c1->next = c2->next;
		if (c2 == s.tail) {
			s.tail = c1;
		}
	}

	*info = request;

	m =  C2P(c2);

	/* add to list */
	if ((found = find_list(m)) == NULL) {
		struct alloc_list *node = (struct alloc_list *) malloc(sizeof(*node));
		node->ptr = m;
		node->next = head.next;
		node->status = 0;
		head.next = node;
	} else if (found->status == 0) {
		found->status = 2;
	}

	/* mark the space alloc bound */
	stop = ((char *)m + request);
	*stop = 'S';

	return m;
}

int deallocate(void *ptr)
{
	struct alloc_hdr *c1, *c2, *c3;
	int j1, j2;
	struct alloc_list *found;
	char *stop;

	if (ptr < lower || ptr >= upper) return -1;
	
	found = find_list(ptr);
	if (found != NULL && found->status == 1) return -2;
	if (found == NULL) return -3;

	/* splice the alloc_hdr back into the list */
	c1 = s.tail;
	c2 = P2C(ptr);

	stop = (char *)ptr + c2->alloc_size;
	if (*stop != 'S') return -4;

	if (c2 > c1) { /* append to end of list */
		if (ISADJ(c1,c2)) { /* join with last alloc_hdr */
			c1->chunk_size += POFF + c2->chunk_size;
			return;
		}
		c2->next = c1->next;
		c1->next = c2;
		s.tail = c2;

		found->status = 1; /* freed */

		return 0;
	}

	while (c1->next < c2) { /* find insertion point */
		c1 = c1->next;
	}
	c3 = c1->next;

	j1 = ISADJ(c1,c2); /* c1 and c2 need to be joined */
	j2 = ISADJ(c2,c3); /* c2 and c3 need to be joined */

	if (j1) {
		if (j2) { /* splice all three alloc_hdr together */
			c1->next = c3->next;
			c1->chunk_size += POFF + c3->chunk_size;
		}
		c1->chunk_size += POFF + c2->chunk_size;
	} else {
		c1->next = c2;
		if (j2) {
			c2->next = c3->next;
			c2->chunk_size += POFF + c3->chunk_size;
		} else {
			c2->next = c3;
		}
	}

	found->status = 1;

	return 0;
}

size_t heap_max()
{
	struct alloc_hdr *c1;
	size_t maxSize = 0;

	c1 = s.tail;
	if (c1->next == s.tail) maxSize = c1->next->chunk_size;
	else {
		while (c1->next != s.tail) {
			if (c1->next->chunk_size > maxSize)
				maxSize = c1->next->chunk_size;

			c1 = c1->next;
		}
	}

	return maxSize;
}
