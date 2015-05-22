#include <stdio.h>

int main(int argc, char **argv) {
	int i = 0;
	while (i < 10000) {
		printf("%d\n", i);
		sleep(1);
		i++;
	}

	return 0;
}
