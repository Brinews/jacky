#include <stdio.h>

int main(int argc, char **argv)
{
	char *filename = "./token/all_tokens";

	FILE *fp = fopen(filename, "r");

	int id, act, del, tim;
	char name[1024];

	while (!feof(fp)) {
		int t = fscanf(fp, "%d %s %d %d %ld", &id, name, &act, &del, &tim);
		if (t != 5) break;
		printf("%d %s %d %d %ld\n", id, name, act, del, tim);
	}

	fclose(fp);

	return 0;
}
