#include "gc.c"

int main()
{
	int i;
	Tiger_heap_init (512);

	char *temp = Tiger_new_array(10);

	strcpy(temp, "Hello");

	for (i = 0; i < 10; i++) {
	}

	printf("%s\n", temp);

	char *obj = Tiger_new(&temp, 10);

	strcpy(obj, "WORLD");
	printf("%s\n", obj);

	return 0;
}
