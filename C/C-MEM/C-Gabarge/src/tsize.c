#include <stdio.h>

char *strings[4] = {"A", "B", "C", "D"};

typedef struct tree_node_st {
	struct tree_node_st *sibling;
	struct tree_node_st *child;
	char *value;
} node_t;

node_t *tree_top;

int test ()
{
	node_t *temp;

	printf("temp=%x\n", &temp);

	return 0;
}


int main()
{
	test();

	node_t *result = NULL;
	printf("%x\n", result);
	printf("%x\n", &result);

	test();

	printf("string_size=%d\n", sizeof(strings));
	printf("%d\n", sizeof(tree_top));

	test();

	return 0;
}
