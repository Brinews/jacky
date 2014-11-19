#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include "pthread.h"
#include <math.h>
#include <string.h>

#define MaxThreadNums 32

int bodys,steps;
double *pBody, **force, *pForce;

pthread_cond_t cond;
pthread_mutex_t mutex1;

void * callForce(void *);

int thread_num,process;

int main(int argc,char *argv[])
{
        int threadID[MaxThreadNums];
        int i,n,t;
        pthread_t threads[MaxThreadNums];
        double runtime;
        char *pStr;
        FILE *fResult;

        timeval start,end;

        gettimeofday(&start,NULL);

        for( i =1  ; i<argc ; i++){
                pStr = strstr(argv[i],"-s=");
                if(pStr != NULL) sscanf(pStr,"-s=%d",&bodys);
                pStr = strstr(argv[i],"-t=");
                if(pStr != NULL) sscanf(pStr,"-t=%d",&steps);
                pStr = strstr(argv[i],"-p=");
                if(pStr !=NULL ) sscanf(pStr,"-p=%d",&thread_num);
        }

        if( bodys * steps * thread_num == 0) {
                printf("wrong args -s=? -p=? -t=?\n");
                return 0;
        }

        if(thread_num<2){
                printf("The threads number is illegal\n");
                return 0;
        }

        if(thread_num>MaxThreadNums)thread_num=MaxThreadNums;

        pForce = new double[3*bodys];
        pBody  = new double[4*bodys];
        force  = new double*[thread_num];
        for(i=0; i<thread_num; i++)
                force[i] = new double[4*bodys];

        for(i=0 ; i<bodys ; i++){
                *(pBody+4*i) = 10.05 + i;
                *(pBody + 4*i +1 ) = 30.0*i;
                *(pBody + 4*i + 2) = 20.0*i;
                *(pBody + 4*i + 3) = 10.0*i;
                *(pForce+3*i)  = 0;
                *(pForce + 3*i + 1)=0;
                *(pForce+3*i+2)=0;
        }

        for(i=0 ; i<thread_num ;i++)
                memset(force[i],bodys*4,0);
        if(pthread_mutex_init(&mutex1,NULL)>0) printf("error in init mutex \n");

        pthread_cond_init(&cond, NULL);

        for(i=0;i<thread_num;i++){
                threadID[i]=i;
                pthread_create(&threads[i],NULL,callForce,&threadID[i]);
        }

        for(i=0;i<thread_num;i++)
                pthread_join(threads[i],NULL);

        pthread_mutex_destroy(&mutex1);

        pthread_cond_destroy(&cond);

        fResult = fopen("result_imp.txt","w");
        char res[50];
        for(i=0;i<bodys;i++){
                sprintf(res,"(%10.4f %10.4f %10.4f %10.f)\n",*(pBody+4*i),*(pBody+4*i+1),*(pBody+4*i+2),*(pBody+4*i+3));
                fwrite(res,sizeof(char),strlen(res),fResult);
        }

        fclose(fResult);
        delete[] pForce;
        delete[] pBody;
        for(i=0;i<thread_num;i++)
                delete []force[i];
        gettimeofday(&end,NULL);

        runtime=end.tv_sec - start.tv_sec +(double)(end.tv_usec - start.tv_usec)/1000000.0;
        printf("runtime is L %f\n",runtime);
        return 0;

}


void *callForce(void *arg)
{
        int myID = *((int *)arg);
        double fac,fx,fy,fz;
        double dx,dy,dz,sq,dist;

        int t=0,i,j;

        while(t < steps){
                for(i=myID;i<bodys;i+=thread_num)
                        for(j=i+1;j<bodys;j++){
                                if(i==j)continue;
                                dx = *(pBody + 4*i+1) - *(pBody+4*j+1);
                                dy = *(pBody + 4*i+2) - *(pBody+4*j+2);
                                dz = *(pBody + 4*i+3) - *(pBody+4*j+3);

                                sq = dx*dx + dy*dy + dz*dz;
                                dist = sqrt(sq);

                                fac = (*(pBody+4*i))*(*(pBody+4*j)) / (dist *sq);

                                fx = fac*dx;
                                fy = fac*dy;
                                fz = fac*dz;

                                *(force[myID]+3*i) = *(force[myID]+3*i) - fx;
                                *(force[myID]+3*i+1) = *(force[myID]+3*i+1) - fy;
                                *(force[myID]+3*i+2) = *(force[myID]+3*i+2) - fz;
                                *(force[myID]+3*j) = *(force[myID]+3*j) + fx;
                                *(force[myID]+3*j+1) = *(force[myID]+3*j+1) + fy;
                                *(force[myID]+3*j+2) = *(force[myID]+3*j+2) + fz;


                        }
                pthread_mutex_lock(&mutex1);
                process++;
                if(process!=thread_num)
                        pthread_cond_wait(&cond,&mutex1);
                else{
                        pthread_cond_broadcast(&cond);
                        process = 0;
                }
                pthread_mutex_unlock(&mutex1);

                for(i=myID;i<bodys;i+=thread_num)
                        for(j=0;j<thread_num;j++){
                                *(pForce+3*i) += *(force[j]+3*i);
                                *(pForce+3*i+1) += *(force[j]+3*i+1);
                                *(pForce+3*i+2) += *(force[j]+3*i+2);
                                *(force[j]+3*j)=0;
                                *(force[j]+3*j+1)=0;
                                *(force[j]+3*j+2)=0;
                        }

                for(i=myID;i<bodys;i+=thread_num){
                        *(pBody + 4*i+1) += (*(pForce+3*i))/(*(pBody+4*i));
                        *(pBody + 4*i+2) += (*(pForce+3*i+1))/(*(pBody+4*i));
                        *(pBody + 4*i+3) += (*(pForce+3*i+2))/(*(pBody+4*i));
                        *(pForce+3*i)=0;
                        *(pForce+3*i+1)=0;
                        *(pForce+3*i+2)=0;
                }

                pthread_mutex_lock(&mutex1);
                process++;
                if(process!=thread_num)
                        pthread_cond_wait(&cond,&mutex1);
                else{
                        pthread_cond_broadcast(&cond);
                        process = 0;
                }
                pthread_mutex_unlock(&mutex1);

                t++;
        }
        return ((void*)NULL);
}

