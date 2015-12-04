/*
 * Date: 2015-12-01
 * Link-State route algorithm
 */

#include <stdio.h>

#define MAX 26
#define INF 0x3fffffff

int graph[MAX][MAX];
char node[MAX];

/*
 * @nodeNum number of node in route
 * @gh route link edges
 */
int ls_route(int nodeNum, int gh[MAX][MAX], int startNo)
{
	int D[MAX]; /* distance from startNo node */
	int p[MAX]; /* record previous node */
	int i;

	/* init */
	for (i = 0; i < nodeNum; i++) {
		D[i] = INF;
		if (gh[startNo][i] != -1) {
			D[i] = gh[startNo][i];
			p[i] = startNo;
		}
	}

	int N[MAX];
	for (i = 0; i < nodeNum; i++) {
		N[i] = 1; /* not in N' */
	}
	N[startNo] = 0;

	/* loop update */
	while (1) {
		int r = 0;

		for (i = 0; i < nodeNum; i++) r += N[i];

		/* N' = N */
		if (r == 0) break;

		/* find w not in N' such that D[w] is a minimum */
		int w = -1;
		int min = INF;

		for (i = 0; i < nodeNum; i++) {
			if (N[i] == 1 && D[i] < min) {
				w = i;
				min = D[i];
			}
		}

		N[w] = 0; /* add w to N' */

		/* update D[v] for each neighbour v of w and not in N' */
		for (i = 0; i < nodeNum; i++) {
			if (gh[w][i] > 0) {
				if (D[w] + gh[w][i] < D[i]) {
					D[i] = D[w] + gh[w][i];
					p[i] = w;
				}
			}
		}
	}

	/*
	for (i = 0; i < nodeNum; i++) {
		printf("%c ", node[i]);
	}
	printf("\n");
	*/

	/* output */
	for (i = 0; i < nodeNum; i++) {
		if (i != startNo) {
			int k = p[i];

			if (p[i] == startNo) k = i;
			else {
				int m = i;
				while (p[m] != startNo) {
					k = p[m];
					m = p[m];
				}
			}

			printf("%c (%c-%c) %d\n", node[i], node[startNo],
					node[k], D[i]);
		}
	}
}


int main(int argc, char **argv)
{
	int nodeNum, edgeNum;
	int i,j;

	char filename[128];
	printf("Input file name(*.txt):");
	scanf("%s", filename);

	FILE *fp = fopen(filename, "r");

	/* read route links */
	fscanf(fp, "%d", &nodeNum);
	fscanf(fp, "%d", &edgeNum);

	for (i = 0; i < nodeNum; i++) {
		char buf[10];
		fscanf(fp, "%s", buf);
		node[i] = buf[0];
	}

	for (i = 0; i < nodeNum; i++)
		for (j = 0; j < nodeNum; j++)
			fscanf(fp, "%d", &graph[i][j]);

	/* use link-state algorithm */
	ls_route(nodeNum, graph, 0);

	fclose(fp);

	return 0;
}
