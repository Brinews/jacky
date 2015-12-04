/*
 * Date:2015-11-28
 * CMDA of Net-Link-Layer
 */

#include <stdio.h>

/*
 * CDMA encode
 * @param userNum no more than 8
 * @param codeLen no more than 16
 * @param dataLen no more than 8
 */
int cdma_encode(int *user_code, int *user_bit, 
		int userNum, int codeLen, int dataLen)
{
	int encoded_bit[16];
	int I, J, K;

	printf("Yes\n");

	for (I = 0; I < dataLen; I++) {
		/* init */
		for (J = 0; J < 16; J++) encoded_bit[J] = 0;

		/* encode */
		for (K = 0; K < codeLen; K++) {
			for (J = 0; J < userNum; J++) {
				/*
				printf("%d,%d,%d\n", K,
						user_code[J*codeLen+K], user_bit[J*dataLen+I]);
				*/
				encoded_bit[K] += (user_code[J*codeLen+K]*user_bit[J*dataLen+I]);
			}
		}

		/* output */
		for (J = 0; J < codeLen; J++) {
			printf("%d ", encoded_bit[J]);
		}
		printf("\n");
	}
}

void check(int expect, int real)
{
	if (expect != real) {
		printf("no\n");
		exit(-1);
	}
}

int main(int argc, char **argv)
{
	char filename[128];
	printf("Input File Name(*.txt):");

	scanf("%s", filename);

	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("no\n");
		return -1;
	}

	int userNum, codeLen, dataLen;
	int user_code[128];
	int user_bit[64];

	int I, J;

	check(1, fscanf(fp, "%d", &userNum));
	check(1, fscanf(fp, "%d", &codeLen));
	check(1, fscanf(fp, "%d", &dataLen));

	for (I = 0; I < userNum; I++) {
		for (J = 0; J < codeLen; J++) {
			check(1, fscanf(fp, "%d", &user_code[I*codeLen + J]));
		}
	}

	for (I = 0; I < userNum; I++) {
		for (J = 0; J < dataLen; J++) {
			check(1, fscanf(fp, "%d", &user_bit[I*dataLen + J]));
		}
	}

	cdma_encode(user_code, user_bit, userNum, codeLen, dataLen);

	fclose(fp);

	return 0;
}
