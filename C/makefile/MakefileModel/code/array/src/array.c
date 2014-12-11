#include "array.h"

int printArray(char *st,int n)
{
	int i = 0;
	if(NULL == st)
	{
		printf("The array is empty!\n");
		return 1;
	}
	for(i = 0;i < n;i++)
	{
		printf("%c ",st[i]);
	}
	printf("\n");
	return 0;
}

int getArray(void)
{
	char str[10] = "array";
	int result = 0;
	printf("array!This is in libarray.a");
	printf("strlen(string) is %d!\n",strlen(str));
	printf("sizeof(string) is %d!\n",sizeof(str));
	result = printArray(str,strlen(str));
	if (0 != result)
	{
		printf("printArray error!");
		return 1;
	}
	return 0;
}
