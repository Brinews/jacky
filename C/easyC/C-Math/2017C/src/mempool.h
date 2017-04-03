#include <stdbool.h>
#include <stdlib.h>

struct pool;

// pool_create() creates a new pool of size characters
// effects: allocates memory (caller must call pool_destroy)
struct pool *pool_create(int size);

// pool_destroy(p) destroys the pool p if there are no active allocations
//   returns true if successful or false if there are active allocations
bool pool_destroy(struct pool *p);

// pool_alloc(p, size) returns a pointer to an uninitialized char array 
//   of size from within pool p, or NULL if no block of size is available
char *pool_alloc(struct pool *p, int size);

// pool_free(p, addr) makes the active allocation at addr available in the pool.
//   returns true if successful (addr corresponds to an active allocation
//   from a previous call to pool_alloc or pool_realloc) or false otherwise
bool pool_free(struct pool *p, char *addr);

// pool_realloc(p, addr, size) changes the size of the active allocation at
//   addr and returns the new address for the allocation.
//   returns NULL if addr does not correspond to an active allocation 
//   or the pool can not be resized (in which case the original allocation
//   does not change)
char *pool_realloc(struct pool *p, char *addr, int size);

// pool_print_active(p) prints out a description of the active allocations 
//   in pool p using the following format:
//   "active: index1 [size1], index2 [size2], ..., indexN [sizeN]\n" or
//   "active: none\n" if there are no active allocations
//   where the index of an allocation is relative to the start of the pool
void pool_print_active(struct pool *p);

// pool_print_available(p) prints out a description of the available 
//   contiguous blocks of memory still available in pool p:
//   "available: index1 [size1], index2 [size2], ..., indexM [sizeM]\n" or
//   "available: none\n" if all of the pool has been allocated
void pool_print_available(struct pool *p);

bool pool_empty(struct pool *p);

int hole_count(struct pool *p);
int mem_usage(struct pool *p);
