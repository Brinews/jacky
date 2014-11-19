#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

int main()
{
    struct timeval start,end;
    gettimeofday(&start,NULL);
    double inc=1.0;
    double pi=0;
    int f=1;
    while(inc < 1e9){
        pi+= (f/inc);
        inc += 2;
        f *= -1;
    }
    gettimeofday(&end,NULL);
    double cost=end.tv_sec - start.tv_sec + (double)(end.tv_usec-start.tv_usec)/1000000.0;
    printf("%.10f\n",cost);
    //printf("pi=%.10f\nThe cost of time:%lf\n",pi*4,cost);

    return 0;
}
~

