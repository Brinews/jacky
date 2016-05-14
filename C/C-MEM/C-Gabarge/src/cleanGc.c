/*
 * Good code help good man.
 * Date: 2015-05-12
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <sys/mman.h>

#define HEAPSIZE	(2<<16)
#define ALIGNMASK 7

typedef struct Block_t *Block_t;

struct Block_t
{
	void* data_addr;
	int length; // block size + request size 
	int request; // request size
	int is_valid; 
	void* ref_addr;
};

#define ALIGN(s) (((s) + ALIGNMASK) & ~ALIGNMASK)

#define HEADEROFF ALIGN(sizeof(struct Block_t))
#define ADDOFFSET(c) ((char *)(c) + HEADEROFF)
#define SUBOFFSET(p) ((struct Block_t *)((char *)(p) - HEADEROFF))
#define BLOCKSIZE(x) (x->length)

#define NEED_GC(remains, need)      \
	((remains)<(need))

#define ADDR_CMP(add1, op, add2)     \
	((int*)(add1)op(int*)(add2))

#define IN_FROM_HEAP(addr)                              \
	(ADDR_CMP(addr, <, heap.from+heap.size)&&       \
	 ADDR_CMP(addr, >=, heap.from))



static const int CHAR_SIZE = sizeof(char);
static const int OBJECT_HEADER_SIZE = HEADEROFF;

struct mem_heap
{
	int size;     

	char *from;   
	char *from_free;
	char *to;      

	char *to_start;
	char *to_free; 
};

struct mem_heap heap = {0, NULL, NULL, NULL, NULL, NULL};

/**
 * allocate a heap
 */
void sgc_heap_init (int heapSize)
{
	char* iheap, *jheap; 
	iheap = (char*)malloc(heapSize);
	//iheap = mmap(NULL, HEAPSIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	memset(iheap, 0, heapSize);

	heap.size=heapSize;
	heap.from=(char*)iheap;
	heap.from_free=heap.from;

	heap.to=heap.from_free+heap.size;

	jheap = (char*)malloc(heapSize);
	//jheap = mmap(NULL, HEAPSIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	heap.to_free = jheap;
	heap.to_start = jheap;

	return;
}

// A copying collector based-on Cheney's algorithm.
static int swap_heap()
{
	char* swap;

	swap = heap.from;
	heap.from = heap.to_start;
	heap.to = (char*)heap.from+heap.size;

	heap.from_free = heap.to_free;
	heap.to_start = swap;
	heap.to_free = swap;

	memset(heap.to_start, 0, heap.size);

	return 0;
}

/*
 * block copy by memcpy
 */
static int copy_block(Block_t org_obj, Block_t upd_obj)
{
	int size = BLOCKSIZE(org_obj);
	char *ref_addr = heap.to_start;
	Block_t ref_blk;
	int *addrvalue;
	int i, number;

	if (!IN_FROM_HEAP(org_obj)) {
		return 0;
	}

	memcpy(upd_obj, org_obj, size);
	heap.to_free += size;
	upd_obj->data_addr = ADDOFFSET(upd_obj);

	while (ref_addr != (char*) upd_obj) {

		ref_blk = (Block_t) ref_addr;

		addrvalue = (int *)ADDOFFSET(ref_blk);
		number = ref_blk->request/sizeof(*addrvalue);

		for (i = 0; i < number; i++) {
			/* if exist reference address */
			if (*addrvalue == (int)org_obj->data_addr) {
				*addrvalue = (int)upd_obj->data_addr;
			}
			addrvalue++;
		}

		ref_addr += BLOCKSIZE(ref_blk);
	}

	return 0;
}

static int copy_heap()
{
	char* to_ptr, *from_ptr;
	Block_t from_obj = (Block_t)(heap.from);
	Block_t to_obj = (Block_t)(heap.to_start);

	to_ptr = heap.to_start;
	from_ptr = heap.from;

	while (from_obj->length > 0) {

		if (from_obj->is_valid == 0) {
			from_ptr = (char *)from_ptr + BLOCKSIZE(from_obj);
			from_obj = (Block_t)from_ptr;
			continue;
		}

		to_obj = (Block_t)to_ptr;
		copy_block(from_obj, to_obj);
		to_ptr = (char*)to_ptr + BLOCKSIZE(to_obj);

		from_ptr = (char *)from_ptr + BLOCKSIZE(from_obj);
		from_obj = (Block_t)from_ptr;
	}

	return swap_heap();
}

static int sgc_gc ()
{
	int before_gc;
	int gc_byte;
	int r;

	before_gc = heap.to-heap.from_free;
	r = copy_heap();
	gc_byte = heap.to-heap.from_free-before_gc;

	return gc_byte;
}

// Object Model And allocation
static void* new_block(int size, int request)
{
	Block_t obj;

	obj  = (Block_t)heap.from_free;
	memset(obj, 0, size * CHAR_SIZE + OBJECT_HEADER_SIZE);

	obj->data_addr = 0;
	obj->length = size * CHAR_SIZE + OBJECT_HEADER_SIZE;
	obj->request = request;
	obj->is_valid = 1;
	obj->ref_addr = 0;

	heap.from_free += (size * CHAR_SIZE + OBJECT_HEADER_SIZE);

	return obj;
}

Block_t find_root(void *addr)
{
	int *paddr;
	int i;
	Block_t ptr = (Block_t)heap.from;

	while (ptr != NULL && ptr->length != 0) {

		if ((int *)ptr->ref_addr == (int *)addr) return ptr;

		ptr = (Block_t) (((char*)ptr) + ptr->length);
	}

	return NULL;
}

/*
 * function to use
 */

void *allocate(size_t request, size_t *collected)
{
	int size, gc_byte;
	Block_t obj;

	size = ALIGN(request);

	if (heap.size == 0) {
		sgc_heap_init(HEAPSIZE);
	}

	if(NEED_GC(heap.to-heap.from_free, size)){
		gc_byte = sgc_gc();

		if(NEED_GC(heap.to-heap.from_free, size)){
			*collected = 0;
			return NULL;
		}
		*collected = gc_byte;
	} else {
		*collected = 0; 
	}

	obj = new_block(size, request);

	// update data_addr
	obj->data_addr = ADDOFFSET(obj);

	return ADDOFFSET(obj);
}


void add_root(void *addr, size_t len)
{
	Block_t toAdd = NULL;
	size_t collected;
	int i;
	char *p;
	int *paddr;

	toAdd = find_root(addr);

	if (toAdd != NULL) {
		toAdd->is_valid = 1;
		return; /* already exist, no need add */
	}

	/* allocate */
	p = (char *)allocate(len, &collected);
	toAdd = SUBOFFSET(p);

	//toAdd->ref_addr = addr;
	paddr = (int *)p;
	for (i = 0; i < len/sizeof(void*); i++) {
		*paddr = (int)(((int *)addr)+i);
		paddr++;
	}
}

void del_root(void *addr)
{
	Block_t needRemove = NULL;
	needRemove = find_root(addr);

	if (needRemove != NULL) {
		needRemove->is_valid = 0;
	}
}

size_t heap_max()
{
	return heap.to - heap.from_free;
}

