/*** NO CHANGES TO THIS FILE ARE ALLOWED EXCEPT WHERE INDICATED ***/
/* MAB management functions for HOSTD */

/* Include files */
#include "mab.h"

/*** CODE COMMENTS MAY BE ADDED BEYOND THIS POINT ***/

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION PROTOTYPES MAY BE ADDED HERE ***/
int sizeRound(int size);
MabPtr findFitBlock(MabPtr m, int size);

/*
 * In buddy, it will be useless
 */
MabPtr mergeLeft(MabPtr *list, MabPtr m);
MabPtr mergeRight(MabPtr m);

/*** END OF SECTION MARKER ***/

/*******************************************************
 * MabPtr memAlloc (MabPtr * lists, int size);
 *    - allocate a memory block
 *
 * returns address of block or NULL if failure
 *******************************************************/
MabPtr memAlloc(MabPtr * lists, int size)
{
	/* find most-left child node */
	MabPtr mostLeft = (*lists)->left_child;

	while (mostLeft != NULL && mostLeft->left_child != NULL) {
		mostLeft = mostLeft->left_child;
	}

	if (mostLeft == NULL) mostLeft = *lists; /* root node */

    MabPtr ret = findFitBlock(mostLeft, size);
    if (ret == NULL) return NULL;

    return memSplit(lists, ret, size);
}

/*******************************************************
 * MabPtr memFree (MabPtr * lists, MabPtr mab);
 *    - de-allocate a memory block
 *
 * returns address of block or merged block
 *******************************************************/
MabPtr memFree(MabPtr * lists, MabPtr m)
{
    if (m == NULL) return NULL;
    m->allocated = 0;

    return memMerge(lists, m);
}

/*******************************************************
 * MabPtr memMerge(MabPtr * lists, Mabptr m);
 *    - merge m recursively with its Buddy if free
 *
 * returns a pointer to one of the following:
 * - merged block if m's Buddy is free
 * - m if m is free but its Buddy is not
 * - NULL if there is an error
 *******************************************************/
MabPtr memMerge(MabPtr * lists, MabPtr m)
{
	MabPtr parent = m->parent;

	if (parent == NULL) return m;

	if (parent->left_child == m) {
		/* m is left child */
		MabPtr right = parent->right_child;

		if (right &&
				right->allocated == 0) { /* right node is free, merge it */
			parent->allocated = 0;

			parent->next = right->next;
			parent->prev = m->prev;
			if (m->prev) m->prev->next = parent;
			if (right->next) right->next->prev = parent;

			free(right);
			free(m);

			return memMerge(lists, parent);
		}
	} else if (parent->right_child == m){
		/* m is right child */
		MabPtr left = parent->left_child;

		if (left &&
				left->allocated == 0) { /* left node is free, merge it */
			parent->allocated = 0;

			parent->next = m->next;
			parent->prev = left->prev;
			if (m->next) m->next->prev = parent;
			if (left->prev) left->prev->next = parent;

			free(left);
			free(m);

			return memMerge(lists, parent);
		}
	}

	return m;

	/* //simple linear merge algorithm 
	 *
    MabPtr leftMerged = mergeLeft(lists, m);
    return mergeRight(leftMerged);
	*/
}

/*******************************************************
 * MabPtr memSplit(MabPtr * lists, MabPtr m, int size);
 *    - split m into two if needed
 *
 * returns a pointer to m or one of its children
 * returns NULL if there is an error
 *******************************************************/
MabPtr memSplit(MabPtr * lists, MabPtr m, int size)
{
    if (m == NULL) return NULL;

    int orig_size = m->size;

    /* allocated = size actual required, 0 means all free */
    m->allocated = size; 

    m->size = sizeRound(size);

	MabPtr parent;
	/* m : left child, newMab : right child */

    while (m->size <= orig_size/2) {
        // split free block into two equal block

        MabPtr newMab = (MabPtr) malloc(sizeof(Mab));
		newMab->left_child = newMab->right_child = NULL;
        MabPtr origNext = m->next;

        newMab->size = orig_size/2;
        newMab->allocated = 0;
        newMab->offset = m->offset+orig_size/2;

        // connected into double link list
        newMab->next = origNext;
        if (origNext != NULL) {
            origNext->prev = newMab;
        }

        m->next = newMab;
        newMab->prev = m;


		// tree build
		parent = (MabPtr) malloc(sizeof(Mab));
		parent->offset = m->offset;
		parent->size = orig_size;
		parent->allocated = orig_size;
		parent->next = parent->prev = NULL;
		parent->parent = m->parent;
		parent->left_child = m;
		parent->right_child = newMab;

		if (parent->parent == NULL) *lists = parent;

		m->parent = parent;
		newMab->parent = parent;

        orig_size /= 2;
    }

    return m;
}

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION IMPLEMENTATIONS MAY BE ADDED HERE ***/
int sizeRound(int size)
{
    int i;
    
    if (size == 0) return 0;

    for (i = 0; ; i++) {
        if (size == (1<<i)) return size;
        if ((1<<i) < size && size < (1<<(i+1))) return (1<<(i+1));
    }
}

/*
 * first fit mode
 */
MabPtr findFitBlock(MabPtr m, int size)
{
    if (m == NULL) return NULL;

    MabPtr itr = m;
    while (itr) {
        if (size <= itr->size && itr->allocated == 0)
            return itr;
        itr = itr->next;
    }

    return NULL;
}

MabPtr mergeLeft(MabPtr *list, MabPtr m)
{
    if (m == NULL) return NULL;

    MabPtr now = m->prev;
    MabPtr prev;
    MabPtr rightBlock = m->next;

    int totalSize = m->size;
    int offset = m->offset;

    while (now) {
        if (now->allocated != 0) break; /* not free left block */

        /* merge */
        totalSize = totalSize + now->size;
        offset = now->offset;
        prev = now;
        now = now->prev;
        if (prev->prev == NULL) { /* head node, need delete m */
            prev->size = totalSize;
            prev->allocated = 0;
            prev->next = rightBlock;
            if (rightBlock) {
                rightBlock->prev = prev;
            }
            free(m);

            return prev;
        }
        free(prev);
    }

    m->size = totalSize;
    m->allocated = 0;
    m->offset = offset;

    m->prev = now;
    if (now) {
        now->next = m;
    }

    return m;
}

MabPtr mergeRight(MabPtr m)
{
    if (m == NULL) return NULL;

    MabPtr now = m->next;
    MabPtr prev = NULL;

    /* we need not change offset */
    int totalSize = m->size;

    while (now) {
        if (now->allocated != 0) break;

        totalSize += now->size;

        prev = now;
        now = now->next;

        free(prev);
    }

    m->size = totalSize;
    m->allocated = 0;

    m->next = now;
    if (now) {
        now->prev = m;
    }

    return m;
}

/*** END OF SECTION MARKER ***/
/*** END OF CODE; DO NOT ADD MATERIAL BEYOND THIS POINT ***/
