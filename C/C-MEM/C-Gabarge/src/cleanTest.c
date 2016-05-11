#include "cleanGc.c"

char *strings[4] = {"A", "B", "C", "D"};

int main()
{
	int byte, i;

	CLGC_heap_init (1024);

	void *p = allocate(10, &byte);
	strcpy(p, "hello");
	printf("%s\n", p);

	void *q = allocate(sizeof(strings), &byte);

	int **addr;
	*addr = (int *)q;

	printf("%x\n", &strings);

	int *t = q;

	for (i = 0; i < 4; i++) {
		*addr = ((int *)strings) + i;
		printf("%x\n", *addr);
		(*addr)++;

		*t = (int)(((int *)strings) + i);
		t++;
	}

	printf("%x\n", *(int *)q);

	return 0;
}
