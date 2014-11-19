/***************************************************  
 *                                                  *  
 *   version1: ʹ��pThreadʵ�ֲ���                  *  
 *                                                  *  
 ****************************************************/   
#include <stdio.h>   
#include <pthread.h>   
#include <string.h>   
#include <sys/time.h>   

#define MAXThreadsNum 32  //��������������߳�����   

#define chunkSize 20000 //������Ĵ�С��ÿ����������Ϊһ�������񣬷ַ���һ���߳�   
#define baseScope 1000   //�ȼ���[1  baseScope]�е��������Դ�Ϊ����������������   
#define exPrimes 10   
#define exIntegers  20   

#define maximumPrimes 20000000   

//ȫ������   
int search_scope_up_bound =30000000;     // С��N������������   
int primes[maximumPrimes];   //��primes[0] �C primes[number-1] �д�����ɵ�������   
int totalPrimes;      //primes�д�ŵ���������   
int unsearchedScope_low_bound;   
int thread_num=0, task_id=0, result_id=0;   
void *searchPrimes(void *);   
pthread_mutex_t mutex1, mutex2;   
pthread_cond_t cond;   

int main(int argc,char ** argv)   
{   
	int upBound;    
	int  i, j, k;    
	char result[20], *pStr;   
	FILE *fResult;   

	int thread_num=0;   
	pthread_t threads[MAXThreadsNum];   
	double tcost;   

	timeval start, end;   

	gettimeofday(&start, NULL);   

	for(i=1; i<argc; i++) {   
		pStr=strstr(argv[i], "-p=");   
		//printf("pStr=%S\n", pStr);   
		if ( pStr==NULL ) continue;   
		sscanf(pStr, "-p=%d", &thread_num);   
	}   
	if(thread_num<2) {   
		printf("Please input the number of threads to be created by specifying -p=num_of_threads, where num_of_threads is a integer greater than 1\n");   
		return 0;   
	}   
	if(thread_num>MAXThreadsNum) thread_num = MAXThreadsNum;   

	//����ǰ baseScope �������е�����   
	primes[0] = 2;   
	totalPrimes = 1;   
	upBound = baseScope;   
	if(upBound > search_scope_up_bound) upBound = search_scope_up_bound;   

	for(i=3; i<upBound+1; i+=2){  //������3��ʼ��� i �Ƿ�Ϊ����   
		for(k=0; primes[k]*primes[k]<i; k++)  // ���μ�� i �Ƿ���Ա�ǰ�����������   
			if( i % primes[k] == 0) break;   
		if(primes[k]*primes[k]>i){ // ��� i ���ܱ�ǰ�������������   
			// ������Ϊ��������������   
			primes[totalPrimes] = i;   
			totalPrimes++;   
		}   
	}   

	//���м���   
	if (pthread_mutex_init(&mutex1,NULL)>0) printf("error in initializing mutex1\n");   
	if (pthread_mutex_init(&mutex2,NULL)>0) printf("error in initializing mutex2\n");   
	pthread_cond_init(&cond, NULL);   
	unsearchedScope_low_bound = baseScope+1;   

	//�����߳�   
	for(i=0; i<thread_num; i++)    
		pthread_create(&threads[i],NULL,searchPrimes,NULL);   

	//�ȴ�ȫ���߳̽���   
	for(i=0; i<thread_num; i++)    
		pthread_join(threads[i],NULL);   

	pthread_mutex_destroy(&mutex1);   
	pthread_mutex_destroy(&mutex2);   
	pthread_cond_destroy(&cond);   

	gettimeofday(&end, NULL);   

	//�洢������   
	fResult=fopen("result_pThread_prime.txt", "wr");   
	for (i=0; i<totalPrimes; i++)    {    
		if (i%10 == 0 ) fputc('\n', fResult);   
		sprintf(result, "%d   ", primes[i]);   
		fwrite(result, sizeof(char), strlen(result), fResult);   
	}   
	fclose(fResult);   

	//���ʱ�俪��   
	tcost = end.tv_sec - start.tv_sec + (double)(end.tv_usec-start.tv_usec)/1000000.0;   
	printf("pThread time is %f seconds, found %d primes and the results are saved in result_pThread_prime.txt\n\n", tcost, totalPrimes);   

	return(0);   
}   

void *searchPrimes(void *) 
{   
	int my_id, loc_low_bound, loc_up_bound, i, k;   
	int locPrimes[chunkSize];   
	int locTotal;   
	char log[128];   

	//��ʼ��������   
	while(1) {   
		//ȡһ��δ������Ƭ��   
		pthread_mutex_lock(&mutex1);   
		loc_low_bound = unsearchedScope_low_bound;   
		unsearchedScope_low_bound += chunkSize;   
		my_id = task_id;   
		task_id++;   
		pthread_mutex_unlock (&mutex1);   
		if( loc_low_bound > search_scope_up_bound ) break;   

		loc_up_bound = loc_low_bound + chunkSize;   
		if ( loc_up_bound > search_scope_up_bound ) loc_up_bound = search_scope_up_bound;   
		locTotal = 0;    

		//����Ƭ���ڵ�����   
		for(i=loc_low_bound; i<loc_up_bound; i+=2){    
			// ���μ�� i �Ƿ���Ա�ǰ�����������   
			for(k=0; primes[k]*primes[k]<i; k++)   
				if( i % primes[k] == 0) break;   
			if(primes[k]*primes[k]>i){ // ��� i ���ܱ�ǰ�������������������Ϊ��������������   
				locPrimes[locTotal] = i;   
				locTotal++;     
			}   
		}   

		//���������������洢������primes��   
		pthread_mutex_lock (&mutex2);   
		while(my_id != result_id)    
			pthread_cond_wait( &cond, &mutex2);   

		for(i=0; i<locTotal; i++) {   
			primes[totalPrimes] = locPrimes[i];   
			totalPrimes++;   
		}   

		result_id++;   
		pthread_cond_broadcast(&cond);   
		pthread_mutex_unlock (&mutex2);   
	}   

	return((void*)NULL);   
}   
