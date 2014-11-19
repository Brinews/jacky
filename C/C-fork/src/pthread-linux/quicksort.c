#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pthread.h"
#include <sys/time.h>
#include <unistd.h>

#define MaxT    16
#define NUMS    100000000


pthread_mutex_t mutex1;
pthread_cond_t  cond1;

int thread_nums;
int *array;
int *arr;

void * quicksort(void*);
void Random();
void Merge();
void sort(int s,int e);


int main(int argc,char *argv[])
{
        int i,j;
        int thread_id[MaxT];
        pthread_t threads[MaxT];
        timeval start,end;
        double  timecost;

        if(argc<2){
                printf("two few args,usage:%s -p=num\n",argv[0]);
                return -1;
        }

        char *pStr=NULL;
        for(i=0 ;i<argc; i++){
                pStr= strstr(argv[i],"-p=");
                if(pStr!=NULL)
                        sscanf(pStr,"-p=%d",&thread_nums);
        }

        if(thread_nums<2){
                perror("pthreads num > 2 .\n");
                return -1;
        }

        array   = new int[NUMS];
        arr     = new int[NUMS];
        if(thread_nums  > MaxT) thread_nums = MaxT;
        Random();

        gettimeofday(&start,NULL);

        if((j=pthread_mutex_init(&mutex1,NULL))<0){
                perror("mutex init error.\n");
                return -1;
        }
        if((j=pthread_cond_init(&cond1,NULL))<0){
                perror("cond init error.\n");
                return -1;
        }

        for(i=0;i<thread_nums;i++){
                thread_id[i]=i;
                pthread_create(&threads[i],NULL,quicksort,&thread_id[i]);
        }

        for(i=0;i<thread_nums;i++){
                pthread_join(threads[i],NULL);
        }

        pthread_mutex_destroy(&mutex1);
        pthread_cond_destroy(&cond1);

        Merge();
        gettimeofday(&end,NULL);

        FILE *f=fopen("r_sort.txt","w");
        for(i=0;i<NUMS;i++){
                fprintf(f,"%12d",arr[i]);
                if(i%10==0)fprintf(f,"\n");
        }

        delete []array;
        delete []arr;

        timecost=end.tv_sec - start.tv_sec + (double)(end.tv_usec-start.tv_usec)/1000000.0;

        printf("The result in file(r_sort.txt.Time Consumed:%lf.\n",timecost);

        return 0;
}

void Random()
{
        int i;
        srand((unsigned int)time(NULL));
        for(i=0;i<NUMS;i++){
                array[i]=rand()%0x7fffffff;
                printf("%d\n",array[i]);
        }

        return;
}

void *quicksort(void *args)
{
        int id = *((int *)args);
        int s = NUMS * id / thread_nums;
        int e = s + NUMS/thread_nums;

        sort(s,e);

        return (void *)NULL;
}

void sort(int s,int e)
{
        if(s<e){
                int x=array[s];
                int i=s,j=e+1;
                int temp;
                while(i<j){
                        while(--j && array[j]>x);
                        while(++i && array[i]<x);
                        if(i>=j)break;
                        temp = array[i];
                        array[i]=array[j];
                        array[j]=temp;
                }
                temp = array[s];
                array[s] = array[j];
                array[j] = temp;
                sort(s,j-1);
                sort(j+1,e);
        }
        return;
}

void Merge()
{
        int i,min,j,N;
        int index[MaxT];
        for(i=0;i<thread_nums;i++)
                index[i] = i * NUMS / thread_nums;
        i=0;

        while(i<NUMS){
                min = 0x7fffffff;//array[index[0]];
                N       = 0;
                for(j=0 ; j<thread_nums ;j++)
                        if(index[j] < (j+1)*NUMS /thread_nums && min > array[index[j]])
                        {
                                min = array[index[j]];
                                N       = j;
                        }
                arr[i++] = min;
                index[N]++;
        }
}

