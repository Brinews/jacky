#include <stdbool.h>
#include <stdlib.h>

typedef enum { FIRST, BEST, WORST } FitOpt;

struct pool;

// pool_create() creates a new pool of size characters
struct pool *pool_create(int size);

bool pool_destroy(struct pool *p);

int pool_alloc(struct pool *p, int size, FitOpt opt);

bool pool_free(struct pool *p, int addr);

int pool_realloc(struct pool *p, int addr, int size, FitOpt opt);

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
