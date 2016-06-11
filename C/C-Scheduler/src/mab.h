/*** NO CHANGES TO THIS FILE ARE ALLOWED. PLEASE DO NOT SUBMIT THIS FILE FOR YOUR ASSIGNMENT. ***/

/* MAB include header file for HOSTD */

#ifndef HD_MAB
#define HD_MAB

/* Include files */
#include <stdlib.h>
#include <stdio.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/*** END OF SECTION MARKER ***/
/*** YOU MAY ADD DEFINITIONS OR PROTOTYPES BEYOND THIS POINT ***/
/*** DO NOT CHANGE OR DELETE EXISTING DEFINITIONS OR PROTOTYPES ***/

/* Memory Management Definitions *******************************/

#define MEMORY_SIZE       1024
#define RT_MEMORY_SIZE    64
#define USER_MEMORY_SIZE  (MEMORY_SIZE - RT_MEMORY_SIZE)

/* Custom Data Types */
struct mab {
    int offset;
    int size;
    int allocated;
    struct mab * next; // for use in a doubly linked list
    struct mab * prev; // for use in a doubly linked list
    struct mab * parent; // for use in a tree
    struct mab * left_child; // for use in a tree
    struct mab * right_child; // for use in a tree
};

typedef struct mab Mab;
typedef Mab * MabPtr;

/* Function Prototypes */
MabPtr memAlloc(MabPtr *, int);
MabPtr memFree(MabPtr *, MabPtr);
MabPtr memMerge(MabPtr *, MabPtr);
MabPtr memSplit(MabPtr *, MabPtr, int);
#endif

/*** PLEASE DO NOT SUBMIT THIS FILE FOR YOUR ASSIGNMENT. ***/
