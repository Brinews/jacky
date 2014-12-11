#include "foo.h"

int printfoo(int a,int b)
{
	int all = 0;
	all = a + b;
	printf("foo!This is in libfoo.a");
	printf("%d(foo) + %d(foo) = %d\n",a,b,all);
	return 0;
}
