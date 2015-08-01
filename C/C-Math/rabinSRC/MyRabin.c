#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

/**
 * int - 4byte long 
 */

int isPrime(int number)
{
	double s = sqrt(1.0*number);

	int n = (int)s;
	int i;

	for (i = 2; i <= n; i++) {
		if (number % i == 0) return 0;
	}

	return 1;
}

int mod4(int number, int left)
{
	if (number % 4 == left) return 1;
	return 0;
}

/*
 * �������������������%4=3
 */
int randPrime(int max, int min)
{
	int prime = 4;

	while (isPrime(prime) == 0 || mod4(prime, 3) == 0) {
		prime = rand()%(max-min+1) + min;
	}

	return prime;
}

/*
 * ����
 */
int inverse(int a, int b)
{
	int i = 0;

	while ((a*(++i))%b != 1) {
		;
	}

	return i;
}

/*
 * ָ��ģ��(a^b)%c
 */
int powerMod(int a, int b, int c)
{
	int t;
	if (b == 0) return 1%c;
	if (b == 1) return a%c;

	t = powerMod(a, b/2, c);
	t = t * t % c;
	if ((b&1) == 1) t = t * a % c;

	return t;
}

/*
 * �й�ʣ�ඨ��
 */
int Deciphering(int b1,int b2,int m1,int m2)
{
	int m=m1*m2;
	int M1=m2;
	int M2=m1;

	int M11=inverse(M1,m1);
	int M22=inverse(M2,m2);

	int result=(M11*M1*b1+M22*M2*b2)%m;

	return result;
}

/*
 * 2�ֽڣ����ֽ��Ƿ���ڵ��ֽ�
 */
int sameByte(int val)
{
	int L, H;

	if (val <= 0xFFFF) {
		L = (val&0xFF);
		H = ((val>>8)&0xFF);

		if (L == H && L >= 0 && L <= 0xFF)
			return 1;
	}

	return 0;
}

/**
 * 8bit - 8bit (ǰ��8���ֽ��ظ�)
 * @param encrytext ����
 * @param p ��Կ
 * @param q ��Կ
 * @return int ����
 */
int Solution(int encrytext, int p, int q) {
	int c1 = 0, c2 = 0;
	int finaltext = 0;

	/* ��һ���� */
	c1 = powerMod(encrytext, (p+1)/4, p);
	c2 = powerMod(encrytext, (q+1)/4, q);

	finaltext = Deciphering(c1, c2, p, q);
	if (sameByte(finaltext)) return finaltext;

	/* �ڶ����� */
	c1 = p - powerMod(encrytext, (p+1)/4, p);
	c2 = powerMod(encrytext, (q+1)/4, q);

	finaltext = Deciphering(c1, c2, p, q);
	if (sameByte(finaltext)) return finaltext;

	/* �������� */
	c1 = powerMod(encrytext, (p+1)/4, p);
	c2 = q - powerMod(encrytext, (q+1)/4, q);

	finaltext = Deciphering(c1, c2, p, q);
	if (sameByte(finaltext)) return finaltext;

	/* ���ĸ��� */
	c1 = p - powerMod(encrytext, (p+1)/4, p);
	c2 = q - powerMod(encrytext, (q+1)/4, q);

	finaltext = Deciphering(c1, c2, p, q);
	if (sameByte(finaltext)) return finaltext;

	return -1;
}

/*
 * ���ܲ���
 * �ù�Կn�����ļ�src,����dst�ܼ�
 */
void Encrypt(unsigned int n, const char *src, const char *dst)
{
	FILE *infile, *outfile;
	unsigned char byte[1];
	int twoByte, encrytext;

	infile = fopen(src, "rb");
	outfile = fopen(dst, "wb");

	/* ���ֽڶ��ļ� */
	while (fread(byte, sizeof(unsigned char), 1, infile) > 0) {
		twoByte = (byte[0] << 8) + byte[0];

		encrytext = (twoByte*twoByte)%n;

		/* д�ļ� */
		fwrite(&encrytext, sizeof(int), 1, outfile);
	}

	fclose(infile);
	fclose(outfile);

}

/*
 * ���ܲ���
 * ����Կp,q�����ļ�sr,����dst����
 */
void Decrypt(int p, int q, const char *src, const char *dst)
{
	FILE *infile, *outfile;
	int *dword;
	unsigned char fourbyte[4];
	int text;
	int ret;
	unsigned char byte;

	infile = fopen(src, "rb");
	outfile = fopen(dst, "wb");
	
	fseek(infile, 0, SEEK_SET);
	//rewind(infile);

	if (infile == NULL) {
		printf("open file %s failed.", src);
	}

	/* ���ֽڶ��ļ� */
	while (!feof(infile)) {

		ret = 0;
		ret = fread(fourbyte, 4*sizeof(unsigned char), 1, infile);

		dword = (int *) fourbyte;

		text = Solution(*dword, p, q);

		if (text != -1) {
			text = text & 0xFF;
			byte = (unsigned char) text;
			//printf("dword=%d,text=%x\n", *dword,byte);
			/* д�ļ� */
			fwrite(&byte, sizeof(unsigned char), 1, outfile);
		} else {
			printf("����%d\n", *dword);
		}
	}

	fclose(infile);
	fclose(outfile);
}

int main(int argc, char **argv)
{
	int min, max;
	int p, q, n;
	char fileName[1024];
	char outFile[1024];
	char origFile[1024];

	printf("�����������䣺(100-2000):");

	scanf("%d", &min);
	scanf("%d", &max);

	srand(time(NULL));

	p = randPrime(max, min);
	q = randPrime(max, min);

	while (p == q) q = randPrime(max, min);

	n = p * q;

	printf("��Կ��%d\n", n);
	printf("˽Կ��%d, %d\n", p, q);

	printf("������������ļ�:(��c:/test/test.in):");
	scanf("%s", fileName);

	strcpy(outFile, fileName);
	strcat(outFile, ".out");
	
	strcpy(origFile, fileName);
	strcat(origFile, ".bak");

	Encrypt(n, fileName, outFile);
	Decrypt(p, q, outFile, origFile);

	return 0;
}
