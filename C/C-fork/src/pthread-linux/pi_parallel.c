#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

int MaxThreadsNum   = 32;
int MaxNum          = 1000000000;
double pi           = 0.0;

int  thread_num;
pthread_mutex_t mutex;

void* part_pi(void*);

int main(int argc,char *argv[])

{
    int i,j;
    pthread_t   threads[MaxThreadsNum];
    int         threadID[MaxThreadsNum];

    struct timeval start,end;

    char *pStr;
    for(i=1; i<argc ;i++){

        pStr = strstr(argv[i],"-p");
        if(pStr==NULL) continue;
        sscanf(pStr,"-p=%d",&thread_num);
    }

    if(thread_num>32) thread_num=MaxThreadsNum;
    if(thread_num<2) { perror("Threads Num is too small.\n");exit(0);}

    gettimeofday(&start, NULL);

    if( pthread_mutex_init(&mutex,NULL)<0)perror("mutex init error\n");
    for(i=0; i<thread_num; i++){
        threadID[i]=i;
        pthread_create(&threads[i],NULL,part_pi,&threadID[i]);
    }
    for(i=0; i<thread_num; i++){
        pthread_join(threads[i],NULL);
    }

    pthread_mutex_destroy(&mutex);

    gettimeofday(&end,NULL);

    double tcost = end.tv_sec - start.tv_sec + (double)(end.tv_usec - start.tv_usec)/1000000.0;

    printf("%.10lf",tcost);
    //printf("The time cost:%lf,PI=%.10lf\n",tcost,pi*4.0);

    return 0;
}

void *part_pi(void *begin)
{
    int be = *((int *)begin);
    double local=0.0;
    int d;

    //printf("%d\n",be);
    int i = be*2 +1;
    for(;i<MaxNum;i+=2*thread_num){
        d=i;
        if(i%4==3 ) d *= -1;
        //if(i<100)printf("%d\n",d);
        local += (1.00/d);
    }
    pthread_mutex_lock(&mutex);
    //printf("-->%lf\n",local);
    pi += local;
    pthread_mutex_unlock(&mutex);

    return ((void *)NULL);

