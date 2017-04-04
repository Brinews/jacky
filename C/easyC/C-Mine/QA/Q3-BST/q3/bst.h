#include <stdbool.h>

// version 2 (March 25th)

// NOTES: All of the following functions REQUIRE:
//        pointers to a bst (e.g., t) are valid (not NULL)
//
//       For times, n is the size of the tree
//       For times, h is the height of the tree

// SEASHELL_READONLY

extern const int PRE_ORDER;
extern const int IN_ORDER;
extern const int POST_ORDER;

struct bstnode {
  int item;
  struct bstnode *left;
  struct bstnode *right;
  int size;                // NEW !
};

struct bst {
    struct bstnode *root;
};

// bst_create() returns a pointer to a new (empty) bst
// effects: allocates memory (caller must call bst_destroy)
// time: O(1)
struct bst *bst_create(void);

// bst_destroy(t) frees all dynamically allocated memory 
// effects: the memory at t is invalid (freed)
// time: O(n)
void bst_destroy(struct bst *t);

// bst_size(t) returns the number of nodes in the bst
// time: O(1)
int bst_size(struct bst *t);

// bst_insert(i, t) inserts the item i into the bst t
// effects: modifies t if i is not already in t
// time: O(h)
void bst_insert(int i, struct bst *t);

// bst_find(i, t) determines if i is in t
// time: O(h) where h is the height of the tree
bool bst_find(int i, struct bst *t);

// bst_select(k, t) returns the k'th element from t in sorted order
// requires: 0 <= k < bst_size(t)
// time: O(h)
int bst_select(int k, struct bst *t);

// bst_remove(i, t) removes i from bst t if it exists
// effects: modifies t if i is in t
// time: O(h)
void bst_remove (int i, struct bst *t);

// bst_range(start, end, t) returns the number of items in t that are 
//   between the values of start and end (inclusive)
// requires: start <= end
// time: O(n)
int bst_range (int start, int end, struct bst *t);

// bst_print(o, t) prints the bst to the screen in order o
// example: given a bst with the following structure
//             4
//           /   \
//          2     5
//         / \
//        1   3
//   pre-order print: "[4,2,1,3,5]\n"
//   in-order print: "[1,2,3,4,5]\n"
//   post-order print: "[1,3,2,5,4]\n"
//   if the t is empty, prints "[empty]\n"
// requires: o is a valid order
// effects: displays output
// time: O(n)
void bst_print (int o, struct bst *t);

// bst_to_sorted_array(t) returns a pointer to a new array,
//   which contains all of the items from t in sorted order.
//   Returns NULL if t is empty.
// effects : allocates memory for non-empty tree (caller must free)
// time: O(n)
int *bst_to_sorted_array(struct bst *t);

// sorted_array_to_bst(a, len) creates a new BALANCED bst that 
//   contains all of the items from a 
// note: the definition of a balanced binary tree is that, for every node 
//   in the tree, the heights of the left and right sub trees differ by 
//   at most 1
// requires: a is sorted in ascending order (no duplciates), len >= 1
// time: O(nlogn), or O(n) for bonus marks
struct bst *sorted_array_to_bst(int *a, int len);

// bst_rebalance(t) changes t so that it contains all of the same items,
//   but the tree is balanced
// effects: modifies t
// time: O(nlogn)
void bst_rebalance(struct bst *t);
