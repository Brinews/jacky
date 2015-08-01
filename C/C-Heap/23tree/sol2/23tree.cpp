#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <cstring>

using namespace std;

#define TREE23_STACK_SIZE 100000

typedef enum {
	LEAF_LINK, INTERNAL_LINK
} LinkKind;

struct tree23_node;
typedef union tree23_link {
	struct tree23_node *node;  /* link to internal node... */
	void *item;                /* ... or a link to a data item. */
} tree23_link_t;

typedef struct tree23_node {
	int count;
	void *key_item1, *key_item2;
	tree23_link_t left, middle, right;
	LinkKind link_kind;
} tree23_node_t;

typedef struct tree23 {
	tree23_node_t *root;
	int (* compar)(const void *, const void *);
	int n;
	void *min_item;
	tree23_node_t **stack;
	signed char *path_info;
} tree23_t;

tree23_t *tree23_alloc(int (* compar)(const void *, const void *));
void tree23_free(tree23_t *t);

void *tree23_insert(tree23_t *t, void *item);
void *tree23_find(tree23_t *t, void *key_item);
void *tree23_delete(tree23_t *t, void *key_item);

int tree23_update_count(tree23_t *t); /* update count wholely */

void *tree23_select(tree23_node_t *t, int kth);
void tree23_sequence(tree23_node_t *p); /* traverse */
void tree23_node_print(tree23_node_t *p);

int tree23_get_seq(tree23_node_t *t, void *item,
		int (* compar)(const void *, const void *));

/* -1 means no found */
int tree23_get_seq(tree23_node_t *t, void *item,
		int (* compar)(const void *, const void *))
{
	if (t == NULL) return -1;

	if (t->link_kind == LEAF_LINK) {
		if (t->left.item && compar(t->left.item, item) == 0) 
			return 1;
		if (t->middle.item && compar(t->middle.item, item) == 0)
			return 2;
		if (t->right.item && compar(t->right.item, item) == 0)
			return 3;

		return -1;
	}

	if (t->link_kind == INTERNAL_LINK) {
		if (t->key_item1 && compar(item, t->key_item1) < 0)
			return tree23_get_seq(t->left.node, item, compar);
		else if ((t->left.node && t->middle.node &&
					!t->right.node) || compar(t->key_item2, item) > 0)
			return t->left.node->count + tree23_get_seq(t->middle.node, 
					item, compar);
		else {
			return t->left.node->count + t->middle.node->count
				+ tree23_get_seq(t->right.node, item, compar);
		}
	}
}

tree23_t *tree23_alloc(int (* compar)(const void *, const void *))
{
	tree23_t *t;
	tree23_node_t *r;

	t = (tree23_t *) malloc(sizeof(tree23_t));
	t->n = 0;
	t->min_item = NULL;
	t->compar = compar;
	t->stack = (tree23_node_t **) malloc(TREE23_STACK_SIZE * sizeof(tree23_node_t *));
	t->path_info = (signed char *) malloc(TREE23_STACK_SIZE * sizeof(signed char));
	r = t->root = (tree23_node_t *) malloc(sizeof(tree23_node_t));
	r->key_item1 = r->key_item2 = NULL;
	r->link_kind = LEAF_LINK;
	r->left.item = r->middle.item = r->right.item = NULL;

	return t;
}


/* tree23_free() - Frees space used by the 2-3 tree pointed to by t. */
void tree23_free(tree23_t *t)
{
	int tos;
	tree23_node_t *p, **stack;

	/* In order to free all nodes in the tree a depth first search is performed
	 * This is implemented using a stack.
	 */

	stack = (tree23_node_t **) malloc(2 * TREE23_STACK_SIZE * sizeof(tree23_node_t *));
	stack[0] = t->root;
	tos = 1;

	while(tos) {
		p = stack[--tos];
		if(p->link_kind == INTERNAL_LINK) {
			stack[tos++] = p->left.node;
			stack[tos++] = p->middle.node;
			if(p->right.node) stack[tos++] = p->right.node;
		}
		free(p);
	}

	free(stack);

	free(t->stack);
	free(t->path_info);
	free(t);
}

void *tree23_insert(tree23_t *t, void *item)
{
	int (* compar)(const void *, const void *);
	int cmp_result;
	void *key_item1, *key_item2, *temp_item, *x_min, *return_item;
	tree23_node_t *new_node, *x, *p;
	tree23_node_t **stack;
	int tos;
	signed char *path_info;


	p = t->root;
	compar = t->compar;

	/* Special case: only zero or one items in the tree already. */
	if(t->n <= 1) {
		if(t->n == 0) {  /* 0 items --> 1 item */   
			t->min_item = p->left.item = item;
		}
		else {  /* 1 item --> 2 items */
			cmp_result = compar(item, p->left.item);

			/* Check that an item with the same key does not already exist. */
			if(cmp_result == 0) return p->left.item;
			/* else */

			/* Determine insertion position. */
			if(cmp_result > 0) {  /* Insert as middle child. */	
				p->key_item1 = p->middle.item = item;
			}
			else {  /* Insert as left child. */
				p->key_item1 = p->middle.item = p->left.item;
				t->min_item = p->left.item = item;
			}
		}

		t->n++;
		return NULL;  /* Insertion successful. */
	}

	/* Stacks for storing the sequence of nodes traversed when
	 * locating the insertion position.
	 */
	stack = t->stack;
	path_info = t->path_info;
	tos = 0;

	/* Search the tree to locate the insertion position. */
	while(p->link_kind != LEAF_LINK) {
		stack[tos] = p;
		if(p->key_item2 && compar(item, p->key_item2) >= 0) {
			p = p->right.node;
			path_info[tos] = 1;
		}
		else if(compar(item, p->key_item1) >= 0) {
			p = p->middle.node;
			path_info[tos] = 0;
		}
		else {
			p = p->left.node;
			path_info[tos] = -1;
		}
		tos++;
	}

	key_item1 = p->key_item1;
	key_item2 = p->key_item2;

	/* Insert at the leaf items of node p.   Note that key_item1 is the same as
	 * p->middle.item and key_item2 is the same as p->right.item.
	 */
	if(key_item2 && (cmp_result = compar(item, key_item2)) >= 0) {
		/* Insert beside right branch. */

		/* Check if the same key already exists. */
		if(cmp_result == 0) {
			return key_item2;  /* Insertion failed. */
		}
		/* else */

		/* Create a new node.  Node p's right child becomes the left child
		 * of the new node.  The inserted item is the middle child of the
		 * new node.
		 */
		new_node = (tree23_node_t *) malloc(sizeof(tree23_node_t));
		new_node->link_kind = LEAF_LINK;
		new_node->left.item = p->right.item;
		new_node->key_item1 = new_node->middle.item = item;
		new_node->key_item2 = new_node->right.item = NULL;	
		p->key_item2 = p->right.item = NULL;

		/* Insertion for new_node will continue higher up in the tree. */
	}
	else if((cmp_result = compar(item, key_item1)) >= 0) {
		/* Insert beside the middle branch. */

		/* Check if the same key already exists. */
		if(cmp_result == 0) {
			return key_item1;  /* Insertion failed. */
		}
		/* else */

		/* Insertion depends on the number of children node p currently has. */
		if(key_item2) {  /* Node p has three children. */

			/* Create a new node.  The inserted item is the left child of
			 * the new node.  Node p's right child becomes the middle
			 * child of the new node.
			 */
			new_node =(tree23_node_t *) malloc(sizeof(tree23_node_t));
			new_node->link_kind = LEAF_LINK;
			new_node->left.item = item;
			new_node->key_item1 = new_node->middle.item = p->right.item;
			new_node->key_item2 = new_node->right.item = NULL;
			p->key_item2 = p->right.item = NULL;

			/* Insertion for new_node will continue higher up in the tree. */
		}
		else {  /* Node p has two children. */

			/* The item is inserted as the right child of node p. */
			p->key_item2 = p->right.item = item;

			/* No need to insert higher up. */
			t->n++;
			return NULL;  /* Insertion successful. */
		}
	}
	else {
		/* Insert beside the left branch. */

		/* Account for the special case, where the item being inserted is
		 * smaller than any other item in the tree.
		 */
		if((cmp_result = compar(item, p->left.item)) <= 0) {

			/* Check if the same key already exists in the tree. */
			if(cmp_result == 0) {
				return p->left.item;  /* Insertion failed. */
			}
			/* else */

			/* The item being inserted is smaller than any other item in the
			 * tree.  Treat p's left child as the item begin inserted.  This
			 * done by replacing p's left child with the item being inserted.
			 */
			temp_item = item;
			item = p->left.item;
			t->min_item = p->left.item = temp_item;
		}

		/* Insertion depends on the number of children node p currently has. */
		if(key_item2) {  /* Node p has three children. */

			/* Create a new node.  Node p's middle child becomes the left
			 * child of the new node.  Node p's right child becomes the middle
			 * child of the new node.  The item being inserted becomes the
			 * middle child of node p.
			 */
			new_node = (tree23_node_t *)malloc(sizeof(tree23_node_t));
			new_node->link_kind = LEAF_LINK;
			new_node->left.item = p->middle.item;
			new_node->key_item1 = new_node->middle.item = p->right.item;
			new_node->key_item2 = new_node->right.item = NULL;
			p->key_item1 = p->middle.item = item;
			p->key_item2 = p->right.item = NULL;

			/* Insertion for new_node will continue higher up in the tree. */
		}
		else {  /* Node p has two children. */

			/* The middle child of node p becomes node p's right child, and
			 * the item being inserted becomes the middle child.
			 */
			p->key_item2 = p->right.item = p->middle.item;
			p->key_item1 = p->middle.item = item;

			/* No need to insert higher up. */
			t->n++;
			return NULL;  /* Insertion successful. */
		}
	}

	return_item = NULL;  /* Insertion successful. */
	t->n++;

	/* x points to the node being inserted into the tree.  x_min keeps track of
	 * the minimum item in the subtree rooted at the node x.
	 */
	x = new_node;
	x_min = new_node->left.item;

	/* Insertion of new nodes can keep propagating up one level in the tree.
	 * This stops when an insertion does not result in a new node at the next
	 * level up, or the root level (tos == 0) is reached.
	 */
	while(tos) {
		p = stack[--tos];  /* p is the parent node for x. */

		/* Determine the insertion position of x under p. */
		if(path_info[tos] > 0) {  /* Insert beside the right branch. */

			/* Create a new node.  Node p's right child becomes the left child
			 * of the new node.  Node x is the middle child of the new node.
			 */
			new_node = (tree23_node_t *)malloc(sizeof(tree23_node_t));
			new_node->link_kind = INTERNAL_LINK;
			new_node->left.node = p->right.node;
			new_node->middle.node = x;
			new_node->key_item1 = x_min;
			new_node->right.node = NULL;	
			new_node->key_item2 = NULL;
			x_min = p->key_item2;
			p->right.node = NULL;
			p->key_item2 = NULL;

			/* Insertion for new_node will continue higher up in the tree. */
		}
		else if(path_info[tos] == 0) {  /* Insert beside the middle branch. */

			/* Insertion depends on the number of children node p currently
			 * has.
			 */
			if(p->key_item2) {  /* Node p has three children. */

				/* Create a new node.  Node x is the left child of the new
				 * node.  Node p's right child becomes the middle child of the
				 * new node.
				 */
				new_node = (tree23_node_t *)malloc(sizeof(tree23_node_t));
				new_node->link_kind = INTERNAL_LINK;
				new_node->left.node = x;  /* x_min does not change. */
				new_node->middle.node = p->right.node;
				new_node->key_item1 = p->key_item2;
				new_node->right.node = NULL;
				new_node->key_item2 = NULL;
				p->right.node = NULL;
				p->key_item2 = NULL;

				/* Insertion for new_node will continue higher up in the tree.
				*/
			}
			else {  /* Node p has two children. */

				/* Node x is inserted as the right child of node p. */
				p->right.node = x;
				p->key_item2 = x_min;

				/* No need to insert higher up. */
				return return_item;
			}
		}
		else {  /* Insert beside the left branch. */

			/* Insertion depends on the number of children node p currently
			 * has.
			 */
			if(p->key_item2) {  /* Node p has three children. */

				/* Create a new node.  Node p's middle child becomes the left
				 * child of the new node.  Node p's right child becomes the
				 * middle child of the new node.  Node x becomes the middle
				 * child of node p.
				 */
				new_node = (tree23_node_t *)malloc(sizeof(tree23_node_t));
				new_node->link_kind = INTERNAL_LINK;
				new_node->left.node = p->middle.node;
				new_node->middle.node = p->right.node;
				new_node->key_item1 = p->key_item2;
				new_node->right.node = NULL;
				new_node->key_item2 = NULL;
				p->middle.node = x;
				temp_item = p->key_item1;
				p->key_item1 = x_min;
				x_min = temp_item;
				p->right.node = NULL;
				p->key_item2 = NULL;

				/* Insertion for new_node will continue higher up in the tree.
				*/
			}
			else {  /* Node p has two children. */

				/* The middle child of node p becomes node p's right child, and
				 * node x becomes the middle child.
				 */
				p->right.node = p->middle.node;
				p->key_item2 = p->key_item1;
				p->middle.node = x;
				p->key_item1 = x_min;

				/* No need to insert higher up. */
				return return_item;
			}
		}

		x = new_node;
	}

	/* This point is only reached if the root node was split.  A new root node
	 * will be created, with the child nodes pointed to by p (old root node)
	 * and x (inserted node).
	 */
	new_node = (tree23_node_t *)malloc(sizeof(tree23_node_t));
	new_node->link_kind = INTERNAL_LINK;
	new_node->left.node = p;
	new_node->middle.node = x;
	new_node->key_item1 = x_min;
	new_node->right.node = NULL;
	new_node->key_item2 = NULL;
	t->root = new_node;


	return return_item;
}

void *tree23_find(tree23_t *t, void *key_item)
{
	int (* compar)(const void *, const void *);
	int cmp_result;
	tree23_node_t *p;
	void *key_item1, *key_item2;

	p = t->root;
	compar = t->compar;

	/* First check for the special cases where the tree contains contains only
	 * zero or one nodes.
	 */
	if(t->n <= 1) {
		if(t->n && (compar(key_item, p->left.item) == 0)) return p->left.item;
		/* else */

		return NULL;
	}

	/* Search the tree to locate the item with key key_item. */
	while(p->link_kind != LEAF_LINK) {
		if(p->key_item2 && compar(key_item, p->key_item2) >= 0) {
			p = p->right.node;
		}
		else if(compar(key_item, p->key_item1) >= 0) {
			p = p->middle.node;
		}
		else {
			p = p->left.node;
		}
	}

	key_item1 = p->key_item1;
	key_item2 = p->key_item2;

	/* Find a leaf item of node p.   Note key_item1 is the same as
	 * p->middle.item and key_item2 is the same as p->right.item.
	 */
	if(key_item2 && (cmp_result = compar(key_item, key_item2)) >= 0) {
		/* Item may be right child. */

		if(cmp_result) {
			/* Find failed - matching item does not exist in the tree. */
			return NULL;
		}
		/* else */
		return key_item2;  /* Item found. */
	}
	else if((cmp_result = compar(key_item, key_item1)) >= 0) {
		/* Item may be middle child. */

		if(cmp_result) {
			/* Find failed - Matching item does not exist in the tree. */
			return NULL;
		}
		/* else */
		return key_item1;  /* Item found. */
	}
	else {
		/* Item may be left child. */

		if(compar(key_item, p->left.item)) {
			/* Find failed - matching item does not exist in the tree. */
			return NULL;
		}
		/* else */
		return p->left.item;  /* item found. */
	}
}

void *tree23_delete(tree23_t *t, void *key_item)
{
	int (* compar)(const void *, const void *);
	int cmp_result;
	void *key_item1, *key_item2, *return_item, *merge_item;
	void **min_key_ptr;
	tree23_node_t *p, *q, *parent, *merge_node;
	tree23_node_t **stack;
	int tos;
	signed char *path_info;


	p = t->root;
	compar = t->compar;

	/* Special cases: 0, 1, or 2 items in the tree. */
	if(t->n <= 2) {
		if(t->n <= 1) {
			if(t->n == 0) {
				return NULL;   /* Tree empty.  Delete failed. */
			}
			/* else: one item in the tree... */

			/* Check if the item is the left child. */
			if(compar(key_item, p->left.item) == 0) {
				return_item = p->left.item;  /* Item found. */
				t->min_item = p->left.item = NULL;
				t->n--;
				return return_item;
			}
			/* else */

			return NULL;  /* Item not found. */
		}
		/* else: two items in the tree... */

		/* Check if the item may be the middle child. */
		if((cmp_result = compar(key_item, p->middle.item)) >= 0) {

			/* check if the item is the middle child. */
			if(cmp_result == 0) {
				return_item = p->middle.item;  /* Item found. */
				p->key_item1 = p->middle.item = NULL;
				t->n--;
				return return_item;
			}
			/* else */

			return NULL;  /* Item not found. */
		}

		/* Check if the item is the left child. */
		if(compar(key_item, p->left.item) == 0) {
			return_item = p->left.item;  /* Item found. */
			t->min_item = p->left.item = p->key_item1;
			p->key_item1 = p->middle.item = NULL;
			t->n--;
			return return_item;
		}
		/* else */

		return NULL;  /* Item not found. */
	}

	/* Allocate stacks for storing information about the path from the root
	 * to the node to be deleted.
	 */
	stack = t->stack;
	path_info = t->path_info;
	tos = 0;
	min_key_ptr = NULL;

	/* Search the tree to locate the node pointing to the leaf item to be
	 * deleted from the 2-3 tree.
	 */
	while(p->link_kind != LEAF_LINK) {
		stack[tos] = p;
		if(p->key_item2 && compar(key_item, p->key_item2) >= 0) {
			min_key_ptr = &p->key_item2;
			p = p->right.node;
			path_info[tos] = 1;
		}
		else if(compar(key_item, p->key_item1) >= 0) {
			min_key_ptr = &p->key_item1;
			p = p->middle.node;
			path_info[tos] = 0;
		}
		else {
			p = p->left.node;
			path_info[tos] = -1;
		}
		tos++;
	}

	key_item1 = p->key_item1;
	key_item2 = p->key_item2;

	/* Delete the appropriate leaf item of node p.  Note that key_item1 is the
	 * same as p->middle.item and key_item2 is the same as p->right.item.
	 */
	if(key_item2 && (cmp_result = compar(key_item, key_item2)) >= 0) {
		/* Item may be right child. */

		/* Check whether the item to be deleted was found. */
		if(cmp_result) {
			/* Item not found. */
			return_item = NULL;
		}
		else {
			/* Item found. */
			return_item = key_item2;
			t->n--;
			p->key_item2 = p->right.item = NULL;
		}

		/* No need for merge since node p still has two items. */
		return return_item;
	}
	else if((cmp_result = compar(key_item, key_item1)) >= 0) {
		/* Item may be middle child. */

		/* Check whether the item to be deleted was found. */
		if(cmp_result) {
			/* Item not found. */
			return NULL;
		}
		/* else */

		return_item = key_item1;  /* Item found. */
		t->n--;

		/* If node p has three children, two are left after the delete, and no
		 * further rearrangement is needed.
		 */
		if(key_item2) {
			p->key_item1 = p->middle.item = key_item2;
			p->key_item2 = p->right.item = NULL;
			return return_item;
		}
		/* else */

		/* Node p has only its left child remaining.  The remaining child will
		 * be merged with a child from a sibling of node p.
		 */
		merge_item = p->left.item;
	}
	else {
		/* Item may be left child. */

		if(compar(key_item, p->left.item)) {
			/* Delete failed - matching item does not exist in the tree. */
			return NULL;
		}

		return_item = p->left.item;  /* item found. */
		t->n--;

		/* Check if a key in the tree changes after deletion. */
		if(min_key_ptr) {
			*min_key_ptr = p->middle.item;  /* Change key. */
		}
		else {
			t->min_item = key_item1;  /* Minimum node in the tree changes. */
		}

		/* If node p has three children, two are left after the delete, and no
		 * further rearrangement is needed.
		 */
		if(key_item2) {
			p->left.item = key_item1;
			p->key_item1 = p->middle.item = key_item2;
			p->key_item2 = p->right.item = NULL;
			return return_item;
		}
		/* else */

		/* Node p has only its middle child remaining.  The remaining child
		 * will be merged with a child from a sibling of node p.
		 */
		merge_item = p->middle.item;
	}


	/* If the function has not exited by this point, then the remaining child
	 * item of node p is to be merged with a child from a sibling of node p.
	 * Node p is not the root node, since this falls under the special case
	 * delete (handled at the start of this function).
	 */

	parent = stack[--tos];  /* Node p's parent. */

	/* The following code performs the leaf level merging of merge_item.  Note
	 * that unless node p was a left child, it always has a sibling to its
	 * left.
	 */
	if(path_info[tos] > 0) {
		/* p was the right child. */
		q = parent->middle.node;  /* Sibling to the left of node p. */

		/* Merging depends on how many children node q currently has. */
		if(q->key_item2) {  /* Node q has three children. */

			/* Keep node p by assigning it the right child of q as the sibling
			 * of merge_item.
			 */
			parent->key_item2 = p->left.item = q->key_item2;
			p->key_item1 = p->middle.item = merge_item;
			q->key_item2 = q->right.item = NULL;

			return return_item;  /* Node p is not deleted. */
		}
		else {  /* Node q has two children. */

			/* Make merge_item a child of node q, and delete p. */
			q->key_item2 = q->right.item = merge_item;
			parent->right.node = NULL;
			parent->key_item2 = NULL;
			free(p);

			return return_item;  /* The parent still has two children. */
		}
	}
	else if(path_info[tos] == 0) {
		/* p was the middle child. */
		q = parent->left.node;  /* Sibling to the left of node p. */

		/* Merging depends on how many children node q currently has. */
		if(q->key_item2) {  /* Node q has three children. */

			/* Keep node p by assigning it the right child of q as the sibling
			 * of merge_item.
			 */
			parent->key_item1 = p->left.item = q->key_item2;
			p->key_item1 = p->middle.item = merge_item;
			q->key_item2 = q->right.item = NULL;

			return return_item;  /* Node p is not deleted. */
		}
		else {  /* Node q has two children. */

			/* Make merge_item a child of node q, and delete p. */
			q->key_item2 = q->right.item = merge_item;
			free(p);

			/* If the parent of p and q had three children, then two will be
			 * left after the merge, and merging will not be needed at the next
			 * level up.
			 */
			if(parent->key_item2) {
				parent->middle.node = parent->right.node;
				parent->key_item1 = parent->key_item2;
				parent->right.node = NULL;
				parent->key_item2 = NULL;
				return return_item;
			}
			/* else */

			/* The parent only has child q remaining.  The remaining child
			 * will be merged with a child from a sibling of the parent.
			 */
			merge_node = q;
			p = parent;
		}
	}
	else {
		/* p was the left child. */
		q = parent->middle.node;  /* Sibling to the right of node p. */

		/* Merging depends on how many children node q currently has. */
		if(q->key_item2) {  /* Node q has three children. */

			/* Keep node p by assigning it the left child of q as the sibling
			 * of merge_item.
			 */
			p->left.item = merge_item;
			p->key_item1 = p->middle.item = q->left.item;
			parent->key_item1 = q->left.item = q->key_item1;
			q->key_item1 = q->middle.item = q->key_item2;
			q->key_item2 = q->right.item = NULL;

			return return_item;
		}
		else {  /* Node q has two children. */

			/* Make merge_item a child of node q, and delete p.
			*/
			q->key_item2 = q->right.item = q->key_item1;
			q->key_item1 = q->middle.item = q->left.item;
			q->left.item = merge_item;
			free(p);

			/* If the parent of p and q had three children, then two will be
			 * left after the merge, and merging will not be needed at the next
			 * level up.
			 */
			if(parent->key_item2) {
				parent->left.node = q;
				parent->middle.node = parent->right.node;
				parent->key_item1 = parent->key_item2;
				parent->right.node = NULL;
				parent->key_item2 = NULL;
				return return_item;
			}
			/* else */

			/* The parent only has child q remaining.  The remaining child
			 * will be merged with a child from a sibling of the parent.
			 */
			merge_node = q;
			p = parent;
		}
	}

	/* Merging of nodes can keep propagating up one level in the tree.  This
	 * stops when the result of a merge does not require a merge at the next
	 * level up, or the root level (tos == 0) is reached.
	 */
	while(tos) {
		/* The following code merges node p's single child, merge_node, with
		 * a children from node p's sibling, q.
		 */
		parent = stack[--tos];  /* Node p's parent. */

		/* Merging depends on which child p is. */
		if(path_info[tos] > 0) {
			/* p was the right child. */
			q = parent->middle.node;  /* Sibling to the left of node p. */

			/* Merging depends on how many children node q currently has. */
			if(q->key_item2) {  /* Node q has three children. */

				/* Keep node p by assigning it the right child of q as the
				 * sibling of merge_node.
				 */
				p->left.node = q->right.node;
				p->middle.node = merge_node;
				p->key_item1 = parent->key_item2;  /* merge_min */
				parent->key_item2 = q->key_item2;
				q->right.node = NULL;
				q->key_item2 = NULL;

				return return_item;  /* Node p is not deleted. */
			}
			else {  /* Node q has two children. */

				/* Make merge_node a child of node q, and delete p. */
				q->right.node = merge_node;
				q->key_item2 = parent->key_item2;  /* merge_min */
				parent->right.node = NULL;
				parent->key_item2 = NULL;
				free(p);

				return return_item;  /* The parent still has two children. */
			}
		}
		else if(path_info[tos] == 0) {
			/* p was the middle child. */
			q = parent->left.node;  /* Sibling to the left of node p. */

			/* Merging depends on how many children node q currently has. */
			if(q->key_item2) {  /* Node q has three children. */

				/* Keep node p by assigning it the right child of q as the
				 * sibling of merge_node.
				 */
				p->left.node = q->right.node;
				p->middle.node = merge_node;
				p->key_item1 = parent->key_item1;  /* merge_min */
				parent->key_item1 = q->key_item2;
				q->right.node = NULL;
				q->key_item2 = NULL;

				return return_item;  /* p is not deleted. */
			}
			else {  /* Node q has two children. */

				/* Make merge_node a child of node q, and delete p. */
				q->right.node = merge_node;
				q->key_item2 = parent->key_item1;  /* merge_min */
				free(p);

				/* If the parent of p and q had three children, then two will
				 * be left after the merge, and merging will not be needed at
				 * the next level up.
				 */
				if(parent->key_item2) {
					parent->middle.node = parent->right.node;
					parent->key_item1 = parent->key_item2;
					parent->right.node = NULL;
					parent->key_item2 = NULL;
					return return_item;
				}
				/* else */

				/* The parent only has child q remaining.  The remaining child
				 * will be merged with a child from a sibling of the parent.
				 */
				merge_node = q;
				p = parent;
			}
		}
		else {
			/* p was the left child. */
			q = parent->middle.node;  /* Sibling to the right of node p. */

			/* Merging depends on how many children node q currently has. */
			if(q->key_item2) {  /* Node q has three children. */

				/* Keep node p by assigning it the left child of q as the
				 * sibling of merge_node.
				 */
				p->left.node = merge_node;
				p->middle.node = q->left.node;
				p->key_item1 = parent->key_item1;
				/* Equals minimum item in tree(q->left.node). */
				q->left.node = q->middle.node;
				parent->key_item1 = q->key_item1;
				q->middle.node = q->right.node;
				q->key_item1 = q->key_item2;
				q->right.node = NULL;
				q->key_item2 = NULL;
				return return_item;
			}
			else {  /* Node q has two children. */

				/* Make merge_node a child of node q, and delete p.
				*/
				q->right.node = q->middle.node;
				q->key_item2 = q->key_item1;
				q->middle.node = q->left.node;
				q->key_item1 = parent->key_item1;
				/* Equals minimum item in tree(q->left.node). */
				q->left.node = merge_node;
				free(p);

				/* If the parent of p and q had three children, then two will
				 * be left after the merge, and merging will not be needed at
				 * the next level up.
				 */
				if(parent->key_item2) {
					parent->left.node = q;
					parent->middle.node = parent->right.node;
					parent->key_item1 = parent->key_item2;
					parent->right.node = NULL;
					parent->key_item2 = NULL;
					return return_item;
				}
				/* else */

				/* The parent only has child q remaining.  The remaining child
				 * will be merged with a child from a sibling of the parent.
				 */
				merge_node = q;
				p = parent;
			}
		}
	}


	/* Remove the old root node, p, making node merge_node the new root node.
	*/
	free(p);
	t->root = merge_node;


	return return_item;
}

int keyComp(const void *a, const void *b)
{
	const int *ia = (const int *)a;
	const int *ib = (const int *)b;

	return *ia - *ib;
}

int charToInt(char *p) 
{
	int i = 0;
	int ret = 0;

	for (i = 0; i < strlen(p); i++) {
		if (p[i] >= '0' && p[i] <= '9')
			ret = ret * 10 + (p[i] - '0');
	}

	if (p[0] == '-') ret *= -1;

	return ret;
}

void tree23_node_print(tree23_node_t *p)
{
	if (p->link_kind == LEAF_LINK) {

		if(p->left.item) {
			printf("%d ", *((int *)p->left.item));
		}
		if(p->middle.item) {
			printf("%d ", *((int *)p->middle.item));
		}
		if(p->right.item) {
			printf("%d ", *((int *)p->right.item));
		}
	}
}

void tree23_sequence(tree23_node_t *p)
{
	queue<tree23_node_t *> nodeQueue;

	if (!p) exit(1);

	nodeQueue.push(p);

	while (!nodeQueue.empty()) {
		tree23_node_t *temp = nodeQueue.front();
		nodeQueue.pop();

		tree23_node_print(temp);

		if (temp->link_kind == INTERNAL_LINK) {
			if (temp->left.node) nodeQueue.push(temp->left.node);
			if (temp->middle.node) nodeQueue.push(temp->middle.node);
			if (temp->right.node) nodeQueue.push(temp->right.node);
		}
	}
}

/**
 * kth selection
 */
void *tree23_select(tree23_node_t *t, int kth)
{
	if (t->link_kind == LEAF_LINK) {
		if (kth == 1 && t->left.item) return t->left.item;
		if (kth == 2 && t->middle.item) return t->middle.item;
		if (kth > 2 && t->right.item) return t->right.item;

		if (kth > 2 && t->middle.item) return t->middle.item;
		if (kth > 1 && t->left.item) return t->left.item;
	}

	if (t->link_kind == INTERNAL_LINK) {
		if (t->left.node && kth <= t->left.node->count) 
			return tree23_select(t->left.node, kth);

		if (t->middle.node && kth > t->left.node->count
			&& kth <= (t->left.node->count + t->middle.node->count)) {
			return tree23_select(t->middle.node, 
					kth-t->left.node->count);
		}

		if (t->right.node 
			&& kth > (t->left.node->count + t->middle.node->count)
			&& kth <= t->count) {
			return tree23_select(t->right.node, 
					kth-t->left.node->count-t->middle.node->count);
		} 

		/* out of range, just return the biggest key */
		if (kth > t->count) {
			if (t->left.node && !t->middle.node)
				return tree23_select(t->left.node, kth);
			if (t->middle.node && !t->right.node)
				return tree23_select(t->middle.node, 
						kth-t->left.node->count);
			if (t->right.node) {
				return tree23_select(t->right.node,
						kth-t->left.node->count-t->middle.node->count);
			}
		}
	}
}

int tree23_update_count(tree23_node_t *t)
{
	if (t->link_kind == LEAF_LINK) {
		if (t->left.item) t->count = 1;
		if (t->middle.item) t->count++;
		if (t->right.item) t->count++;

		return t->count;
	}

	if (t->link_kind == INTERNAL_LINK) {

		int leftCount = 0, midCount = 0, rightCount = 0;

		if (t->left.node)
			leftCount = tree23_update_count(t->left.node);
		if (t->middle.node)
			midCount = tree23_update_count(t->middle.node);
		if (t->right.node)
			rightCount = tree23_update_count(t->right.node);

		t->count = leftCount + midCount + rightCount;

		return t->count;
	}
}

int main(int argc, char **argv)
{
	tree23_t *t23 = tree23_alloc(keyComp);

	FILE *in = freopen("input.txt", "r", stdin);

	char buf[1024], t;
	int i = 0;

	vector<int> v;
	v.clear();

	while ((t = getchar()) != '\n') {
		buf[i++] = t;
		if (t == ',') {
			buf[i-1] = '\0';

			int p = charToInt(buf);
			v.push_back(p);

			i = 0;
		}
	}

	v.push_back(charToInt(buf));

	for (i = 0; i < v.size(); i++)
		tree23_insert(t23, &(v[i]));

	tree23_update_count(t23->root);

	char command[128];
	int key;

	while (fgets(buf, 1024, stdin) != NULL) {

		buf[strlen(buf)-1] = '\0';

		sscanf(buf, "%s%d", command, &key);

		if (strcasecmp(command, "find") == 0) {
			int *ret = (int *)tree23_find(t23, &key);
			if (ret != NULL && *ret == key) {
				int seq = tree23_get_seq(t23->root, &key, keyComp);
				printf("%s:Found; %d is the %dth element of the list\n",
					buf, key, seq);
			} else {
				printf("%s:Not found\n", buf);
			}
		}

		if (strcasecmp(command, "insert") == 0) {
			v.push_back(key);
			tree23_insert(t23, &(v[v.size()-1]));
			tree23_update_count(t23->root);
			int seq = tree23_get_seq(t23->root, &key, keyComp);
			printf("%s:After the insertion, %d is the %dth element of the the list\n", 
					buf, key, seq);
		}

		if (strcasecmp(command, "delete") == 0) {
			int seq = tree23_get_seq(t23->root, &key, keyComp);
			int *del = (int *)tree23_delete(t23, &key);
			if (del == NULL) {
				printf("%s:Not found\n", buf);
			} else {
				tree23_update_count(t23->root);
				printf("%s:%d is the %dth element of the list, deleted\n", buf, *del, seq);
			}
		}

		if (strcasecmp(command, "selection") == 0) {
			int *sel = (int *)tree23_select(t23->root, key);
			printf("%s:Returns %d which is the %dth smallest element\n", buf, *sel, key);
		}
	}

	fclose(stdin);

	FILE *out = freopen("1234.txt", "w", stdout);
	tree23_sequence(t23->root);
	fclose(stdout);

	tree23_free(t23);

	return 0;
}
