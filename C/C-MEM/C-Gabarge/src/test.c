#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cleanGc.c"

int x; // DO NOT REGISTER, it cannot contain pointers.
char *strings[4] = {"A", "B", "C", "D"};

typedef struct tree_node_st {
	struct tree_node_st *sibling;
	struct tree_node_st *child;
	char *value;
} node_t;

node_t *tree_top;

static void register_roots() {
	add_root(&strings, sizeof(strings));
	add_root(&tree_top, sizeof(tree_top));
} 

static node_t *alloc_node() {
	node_t *result; 
	size_t status;
	add_root(&result, sizeof(result));
	result = (node_t *)allocate(sizeof(node_t), &status);
	if (!result) {
		printf ("Out of memory.\n");
		exit (0);
	}
	if (status) {
		printf ("Nice! GC ran and collected %zu bytes for us.\n", status);
	}
	result->sibling = 0;
	result->child = 0;
	result->value = 0;
	del_root(&result);
	return result;
}
static char *alloc_copy_str(char *s) {
	char *result; 
	size_t status, i, l = strlen(s) + 1; // Copy the null.
	add_root(&result, sizeof(result));
	result = (char *)allocate(sizeof(node_t), &status);
	if (!result) {
		printf ("Out of memory.\n");
		exit (0);
	}
	if (status) {
		printf ("Nice! GC ran and collected %zu bytes for us.\n", status);
	}
	for (i=0;i<l;i++) result[i] = s[i];
	del_root(&result); 
	return result;
}
static void build_tree(int argc, char **argv) {
	node_t *cur = tree_top;
	node_t *next;
	int i;
	add_root(&cur, sizeof(cur));
	add_root(&next, sizeof(next));
	for (i=1;i<argc;i++) {
		if (!strlen(argv[i])) continue;
		next = alloc_node();
		next->value = alloc_copy_str(argv[i]);
		if (argv[i][0] >= 'A' && argv[i][0] <= 'Z') {
			cur->sibling = next;
		} else {
			cur->child = next;
		}
		cur = next; 
	}
	del_root(&next);
	del_root(&cur);
}

static void init_tree() {
	tree_top = alloc_node();
	tree_top->value = "root";
}

static void print_tree(node_t *root, int step)
{
	int i;

	if (root == NULL) return;

	for (i = 0; i < step; i++)
		printf(" ");
	printf("%s\n", root->value);

	print_tree(root->child, step+1);
	print_tree(root->sibling, step+1);
}

int main(int argc, char **argv) {
	register_roots();
	init_tree();
	build_tree(argc, argv);

	print_tree(tree_top, 1);

	CLGC_gc();

	CLGC_gc();

	print_tree(tree_top, 1);

	return 0;
}

