#ifndef _CLGC_
#define _CLGC_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>

typedef enum
{
  TYPE_OBJECT,
  TYPE_ARRAY
}OBJECT_TYPE;

typedef struct Object_t *Object_t;

/*    
      ----------------
      | vptr         |
      |--------------|
      | isObjOrArray |
      |--------------|
      | length       |
      |--------------|
      | request      |
      |--------------|
      | isValid      |
      |--------------|
      | forwarding   |
      |--------------|\
      p---->| e_0    | \
      |--------------|  s
      | ...          |  i
      |--------------|  z
      | e_{length-1} | /e
      ----------------/
      */
struct Object_t
{
  void* vptr;
  OBJECT_TYPE objType;
  int length; // actual malloced
  int request; // actual request
  int isValid; //0:invalid, 1:valid
  void* forwarding;
};

#define MAXSIZE	(2<<17)

#define ALIGNMASK 7
#define ALIGN(s) (((s) + ALIGNMASK) & ~ALIGNMASK)
#define POFF ALIGN(sizeof(struct Object_t))
#define C2P(c) ((char *)(c) + POFF)
#define P2C(p) ((struct Object_t *)((char *)(p) - POFF))

#define GET_OBJECT_FIELD_ADDR(obj, index)   \
  (((char*)(obj + 1)) + index * (CHAR_SIZE))

#define GET_OBJECT(addr)    (*(Object_t*)addr)

#define NO_ENOUGH_SPACE(remains, need)      \
  ((remains)<(need))

#define ADDRESS_COMPARE(add1, op, add2)     \
  ((int*)(add1)op(int*)(add2))

#define IN_TO_SPACE(addr)                                \
  (ADDRESS_COMPARE(addr, <, heap.toStart+heap.size)&&    \
   ADDRESS_COMPARE(addr, >=, heap.toStart))

#define IN_FROM_SPACE(addr)                              \
  (ADDRESS_COMPARE(addr, <, heap.from+heap.size)&&       \
   ADDRESS_COMPARE(addr, >=, heap.from))



/**
 * The "prev" pointer, pointing to the top frame on the GC stack.
 */
void *previous = 0;
static int copyCount;
static const int WORLD = sizeof(int*);
static const int CHAR_SIZE = sizeof(char);
static const int OBJECT_HEADER_SIZE = POFF;

//===============================================================//
// Heap data structure.

/*
   ----------------------------------------------------
   |From Space              |To Space                 |
   ----------------------------------------------------
   ^\                      /^
   | \<~~~~~~~ size ~~~~~>/ |
   from                       to
   */
struct MyHeap
{
  int size;         // in bytes, note that this is for semi-heap size
  char *from;       // the "from" space pointer
  char *fromFree;   // the next "free" space in the from space
  char *to;         // the "to" space pointer
  char *toStart;    // "start" address in the "to" space
  char *toNext;     // "next" free space pointer in the to space
};

/**
 * The heap, which is initialized by the following
 * "heap_init" function.
 */
struct MyHeap heap = {0, NULL, NULL, NULL, NULL, NULL};

/**
 * Given the heap size (in bytes), allocate a heap
 * in the C heap, initialize the relevant fields.
 */
void CLGC_heap_init (int heapSize)
{
  // #1: allocate a chunk of memory of size "heapSize"
  char* jheap; 
  //jheap = (char*)malloc(heapSize);
  jheap = mmap(NULL, MAXSIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
  memset(jheap, 0, heapSize);

  // #2: initialize the "size" field, note that "size" field
  // is for semi-heap, but "heapSize" is for the whole heap.
  heap.size=heapSize/2;
  // #3: initialize the "from" field.
  heap.from=(char*)jheap;
  // #4: initialize the "fromFree" field.
  heap.fromFree=heap.from;
  // #5: initialize the "to" field.
  heap.to=heap.fromFree+heap.size;
  // #6: initizlize the "toStart" field. 
  heap.toNext=(char*)heap.to+1;
  // #7: initialize the "toNext" field.
  heap.toStart=(char*)heap.to+1;

  return;
}

//===============================================================//
// A copying collector based-on Cheney's algorithm.

static int swapAndCleanUp()
{
  char* swap;

  swap = heap.from;
  heap.from = heap.toStart;
  heap.to = (char*)heap.from+heap.size;
  heap.fromFree = heap.toNext;
  heap.toStart = swap;
  heap.toNext = swap;
  memset(heap.toStart, 0, heap.size);

  return 0;
}

static int objectSize(Object_t obj)
{
  int size;

  size = 0;
  switch (obj->objType)
  {
    case TYPE_OBJECT:
      size = obj->length;
      break;
    case TYPE_ARRAY:
      size = obj->length * CHAR_SIZE + OBJECT_HEADER_SIZE;
      break;
  }

  return size;
}


static int copyCollection(Object_t old_obj, Object_t new_obj)
{
  int size = objectSize(old_obj);
  char *refAddr = heap.toStart;
  Object_t refObj;
  int *content;
  int i, number;

  if (!IN_FROM_SPACE(old_obj)) {
    return 0;
  }

  memcpy(new_obj, old_obj, size);
  heap.toNext += size;

  old_obj->forwarding = new_obj;
  new_obj->vptr = C2P(new_obj);

  // copy reference address

  while (refAddr != (char*) new_obj) {

	  refObj = (Object_t) refAddr;

	  content = (int *)C2P(refObj);
	  number = refObj->request/sizeof(*content);

	  for (i = 0; i < number; i++) {
		  if (*content == (int)old_obj->vptr) {
			  *content = (int)new_obj->vptr;
		  }
		  content++;
	  }

	  refAddr += objectSize(refObj);
  }

  return 0;
}

static int collectedField()
{
  char* to_ptr, *from_ptr;
  Object_t fromObj = (Object_t)(heap.from);
  Object_t toObj = (Object_t)(heap.toStart);

  to_ptr = heap.toStart;
  from_ptr = heap.from;

  while (fromObj->length > 0) {

	if (fromObj->isValid == 0) {
		from_ptr = (char *)from_ptr + objectSize(fromObj);
		fromObj = (Object_t)from_ptr;
		continue;
	}

    toObj = (Object_t)to_ptr;

    switch (fromObj->objType) {
      case TYPE_OBJECT:
        copyCollection(fromObj, toObj);
        break;
      case TYPE_ARRAY:
		// no need now
        break;
    }

    to_ptr = (char*)to_ptr + objectSize(toObj);

	from_ptr = (char *)from_ptr + objectSize(fromObj);
	fromObj = (Object_t)from_ptr;
  }

  return 0;
}

static int Verbose_CLGC_gc()
{

  int r;

  r = collectedField();
  r = swapAndCleanUp();

  return 0;
}

static int CLGC_gc ()
{
  int before_gc;
  int gcByte;
  int r;

  before_gc = heap.to-heap.fromFree;

  r = Verbose_CLGC_gc();

  gcByte = heap.to-heap.fromFree-before_gc;

  return gcByte;
}


//===============================================================//
// Object Model And allocation


// "new" a new object, do necessary initializations, and
// return the pointer (reference).
/*    ----------------
      | vptr      ---|----> (points to the virtual method table)
      |--------------|
      | isObjOrArray | (0: for normal objects)
      |--------------|
      | length       | (this field should be empty for normal objects)
      |--------------|
      | request      |
      |--------------|
      | isValid      |
      |--------------|
      | forwarding   |
      |--------------|\
      p---->| v_0    | \
      |--------------|  s
      | ...          |  i
      |--------------|  z
      | v_{size-1}   | /e
      ----------------/
      */
/**
 * Try to allocate an object in the "from" space of the
 * heap. Read CLGC book chapter 13.3 for details on the
 * allocation.
 * There are two cases to consider:
 *   1. If the "from" space has enough space to hold this object, then
 *      allocation succeeds, return the apropriate address (look at
 *      the above figure, be careful);
 *   2. if there is no enough space left in the "from" space, then
 *      you should call the function "CLGC_gc()" to collect garbages.
 *      and after the collection, there are still two sub-cases:
 *        a: if there is enough space, you can do allocations just as case 1;
 *        b: if there is still no enough space, you can just issue
 *           an error message ("OutOfMemory") and exit.
 *           (However, a production compiler will try to expand
 *           the heap.)
 */

static void* new_block(int size, int request)
{
  Object_t obj;

  obj  = (Object_t)heap.fromFree;
  memset(obj, 0, size * CHAR_SIZE + OBJECT_HEADER_SIZE);

  obj->vptr = 0;
  obj->objType = TYPE_OBJECT;
  obj->length = size * CHAR_SIZE + OBJECT_HEADER_SIZE;
  obj->request = request;
  obj->isValid = 1;
  obj->forwarding = 0;

  heap.fromFree += (size * CHAR_SIZE + OBJECT_HEADER_SIZE);

  return obj;
}

/***************************************************************
 * Added API
 */

void *allocate(size_t request, size_t *collected)
{
	int size, gcByte;
	Object_t obj;

	size = ALIGN(request); // keep align

	if (heap.size == 0) { // first init
		CLGC_heap_init(MAXSIZE);
	}

	if(NO_ENOUGH_SPACE(heap.to-heap.fromFree, size)){
		gcByte = CLGC_gc();

		if(NO_ENOUGH_SPACE(heap.to-heap.fromFree, size)){
			*collected = 0;
			return NULL;
		}

		*collected = gcByte;
	} else {
		*collected = 0; // needn't GC
	}

	obj = new_block(size, request);

	// update vptr
	obj->vptr = C2P(obj);

	return C2P(obj);
}


Object_t find_root(void *addr)
{
	Object_t ptr = (Object_t)heap.from;

	while (ptr != NULL && ptr->length != 0) {
		if (ptr->forwarding == addr) return ptr;

		ptr = (Object_t) (((char*)ptr) + ptr->length);
	}

	return NULL;
}

void add_root(void *addr, size_t len)
{
	Object_t toAdd = NULL;
	size_t collected;
	int i;
	char *p;
	int *paddr;

	toAdd = find_root(addr);

	if (toAdd != NULL) {
		toAdd->isValid = 1;
		return; /* already exist */
	}

	/* allocate a new obj */
	p = (char *)allocate(len, &collected);
	toAdd = P2C(p);

	paddr = (int *)p;
	for (i = 0; i < len/sizeof(void*); i++) {
		*paddr = (int)(((int *)addr)+i);
		paddr++;
	}
}

void del_root(void *addr)
{
	Object_t toDel = NULL;

	toDel = find_root(addr);

	if (toDel != NULL) {
		toDel->isValid = 0;
	}
}

size_t heap_max()
{
	return heap.to - heap.fromFree;
}

#endif
