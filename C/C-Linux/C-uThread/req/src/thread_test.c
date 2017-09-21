#include "threads.h"

Queue *RunQ = NULL;

int g = 0;
double pi = 4;
int dominator = 3;
int LIMIT = 500000;

void print_pi()
{
	while (dominator < LIMIT) {
		printf("pi=%.10lf\n", pi);
		yield();
	}
}

void calculate_pi()
{
	int factor = -1;

	while (dominator < LIMIT) {
		pi += factor*4.0/dominator;
		dominator += 2;
		factor *= -1;
		yield();
		printf("limit=%d\n", dominator);
	}
}

int main(int argc, char **argv)
{
	initQueue(&RunQ);

	start_thread(calculate_pi);
	start_thread(print_pi);

	run();

	return 0;
}
