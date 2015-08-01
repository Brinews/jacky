#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/time.h>

#define MAX_SPOTS   6
#define MAX_TIMES   11


pthread_t vehicleCreationThread;
pthread_t captainThread;
pthread_t vehicleThread[300];
int threadCounter = 0;

pthread_mutex_t carQueueMutex;
int carCounter = 0;
pthread_mutex_t trucksQueueMutex;
int truckCounter = 0;
pthread_mutex_t carsUnloadedMutex;
int carsUnloadCnt = 0;
pthread_mutex_t trucksUnloadMutex;
int trucksUnloadedCnt = 0;


sem_t carsQueueSem;
sem_t truckQueueSem;
sem_t carsLoadSem;
sem_t trucksLoadSem;
sem_t vehiclesSailSem;
sem_t vehiclesArriveSem;
sem_t waitUnloadSem;
sem_t readyUnloadSem;
sem_t carsUnloadSem;
sem_t trucksUnloadedSem;
sem_t waitExitSem;


struct timespec tm;
int fullSpotsOnFerry = 0;  
int isTruck = 0;
int maxTimeToNextArrival;
int truckArrivalProb;


void* feryCaptainThread();
void* feryTruckThread();
void* feryCarThread();
void* feryVehicleCreateThread();

int feryTimeInterval( const struct timeval startTime )
{
    struct timeval nowTime;
    long int elapsed;
    int elapsedTime;

    gettimeofday(&nowTime, NULL);
    elapsed = (nowTime.tv_sec - startTime.tv_sec) * 1000000
        + (nowTime.tv_usec - startTime.tv_usec);
    elapsedTime = elapsed / 1000;
    return elapsedTime;
}

void* feryCaptainThread()
{
    int loads = 0;       
    int carQNum = 0;
    int trucksQNum = 0;
    int trucksLoadedNum = 0;
    int filledNum = 0;
    int numberOfSpacesEmpty = 0;
    int vehiclesNum = 0;
    int counter = 0;

    while (loads < MAX_TIMES) 
    {
        printf("---CAPTAIN---    %d loads around\n", loads);
        trucksLoadedNum = 0;
        filledNum = 0;
        vehiclesNum = 0;

        while(filledNum < MAX_SPOTS) {
            pthread_mutex_lock(&trucksQueueMutex);
            pthread_mutex_lock(&carQueueMutex);
            trucksQNum = truckCounter;
            carQNum = carCounter;
            pthread_mutex_unlock(&carQueueMutex);
            pthread_mutex_unlock(&trucksQueueMutex);
            while(trucksQNum > 0 && 
                    filledNum < MAX_SPOTS-1 && 
                    trucksLoadedNum < 2) {
                pthread_mutex_lock(&trucksQueueMutex);
                truckCounter--;
                printf("---CAPTAIN---    Truck get signal to leave queue \n");
                sem_post(&truckQueueSem);
                pthread_mutex_unlock(&trucksQueueMutex);
                trucksQNum--;
                trucksLoadedNum++;
                filledNum+=2;
                vehiclesNum++;
            }
            while(carQNum > 0 && 
                    filledNum < MAX_SPOTS) {
                pthread_mutex_lock(&carQueueMutex);
                carCounter--;
                printf("---CAPTAIN---    Car get signal to leave queue\n");
                sem_post(&carsQueueSem);
                pthread_mutex_unlock(&carQueueMutex);
                carQNum--;
                filledNum++;
                vehiclesNum++;
            }
        }


        for(counter = 0; counter < trucksLoadedNum; counter++) {
            sem_wait(&trucksLoadSem);
            printf("---CAPTAIN---    Truck loaded\n");
        }

        for(counter = 0; counter < vehiclesNum - trucksLoadedNum;
                counter++) {
            sem_wait(&carsLoadSem);
            printf("---CAPTAIN---    Car loaded\n");
        }

        printf("\n---CAPTAIN---  		ferry full, sailing\n");


        for(counter = 0; counter < vehiclesNum; counter++) {
            printf("---CAPTAIN---    vehicle %d has acked it is sailing\n",
                    counter);
            sem_post(&vehiclesSailSem);
        }

        printf("---CAPTAIN---     	all vehicles arrived\n");


        for(counter = 0; counter < vehiclesNum; counter++) {
            printf("---CAPTAIN---    vehicle %d has acked its arrived\n",
                    counter);
            sem_post(&vehiclesArriveSem);
        }

        for(counter = 0; counter < vehiclesNum; counter++) {
            sem_wait(&readyUnloadSem);
        }

        printf( "\n---CAPTAIN--- 	arrived at destination and docked\n");

        for(counter = 0; counter < vehiclesNum; counter++) {
            sem_post(&waitUnloadSem);
        }

        numberOfSpacesEmpty = 0;
        while(numberOfSpacesEmpty < MAX_SPOTS) {
            pthread_mutex_lock(&carsUnloadedMutex);
            if( carsUnloadCnt > 0 ){
                sem_wait(&carsUnloadSem);
                carsUnloadCnt--;
                numberOfSpacesEmpty++;
                printf("---CAPTAIN---    car has unloaded from the ferry\n");
            }
            pthread_mutex_unlock(&carsUnloadedMutex);

            pthread_mutex_lock(&trucksUnloadMutex);
            if( trucksUnloadedCnt > 0 ){
                sem_wait(&trucksUnloadedSem);
                trucksUnloadedCnt--;
                numberOfSpacesEmpty+=2;
                printf("---CAPTAIN---    truck has unloaded from the ferry\n");
            }
            pthread_mutex_unlock(&trucksUnloadMutex);
        }

        printf("\n---CAPTAIN---  		unloading complete\n");

        for(counter = 0; counter < vehiclesNum; counter++) {
            printf("---CAPTAIN---    unloaded vehicle %d is about to exit\n",
                    counter);
            sem_post(&waitExitSem);
        }

        printf("\n---CAPTAIN---  		arrived at loading dock\n");

        loads++;
    }

    exit(0);
}


void* feryVehicleCreateThread()
{
    struct timeval startTime; 
    int elapsed = 0;         
    int lastArrivalTime = 0;

    printf("-CREATE:       creation thread \n");

    gettimeofday(&startTime, NULL);

    elapsed = feryTimeInterval(startTime);

    srand (elapsed*1000+100);

    while(1) { 
        elapsed = feryTimeInterval(startTime);
        while(elapsed >= lastArrivalTime) {
            printf("-CREATE:      elapsed time %d arrival time %d\n", elapsed, lastArrivalTime); 
            if(lastArrivalTime > 0 ) { 
                isTruck = rand() % 100;
                if(isTruck >  truckArrivalProb ) {
                    pthread_create(&(vehicleThread[threadCounter]), NULL, feryCarThread, NULL);
                    printf("-CREATE:     Created a car thread\n");
                }
                else {
                    pthread_create(&(vehicleThread[threadCounter]), NULL, feryTruckThread, NULL);
                    printf("-CREATE:     Created a truck thread\n");
                }
                threadCounter++;
            }
            lastArrivalTime += rand()% maxTimeToNextArrival;
            printf("-CREATE:     present time %d, next arrival time %d\n", elapsed, lastArrivalTime);
        }
    }

    printf("-CREATE:      EXITING FROM CREATE\n");
}


void* feryTruckThread()
{
    unsigned long int *threadId;

    threadId = (unsigned long int *)pthread_self();

    pthread_mutex_lock(&trucksQueueMutex);
    truckCounter++;
    pthread_mutex_unlock(&trucksQueueMutex);

    printf("-TRUCK:%lu     truck Queued\n", *threadId);
    sem_wait(&truckQueueSem);
    printf("-TRUCK:%lu  truck leaving queue to load\n", *threadId);

    printf("-TRUCK:%lu   truck onboard ferry\n", *threadId);
    sem_post(&trucksLoadSem);

    sem_wait(&vehiclesSailSem);
    printf("-TRUCK:%lu   truck travelling\n", *threadId);

    sem_wait(&vehiclesArriveSem);
    printf("-TRUCK:%lu   truck arrived at destination\n", *threadId);
    sem_post(&readyUnloadSem);

    sem_wait(&waitUnloadSem);
    pthread_mutex_lock(&trucksUnloadMutex);
    trucksUnloadedCnt++;
    pthread_mutex_unlock(&trucksUnloadMutex);
    printf("-TRUCK:%lu     Truck unloaded from ferry\n", *threadId);
    sem_post(&trucksUnloadedSem);

    sem_wait(&waitExitSem);
    printf("-TRUCK:%lu     truck exits\n", *threadId);

    pthread_exit(0);
}


void* feryCarThread()
{
    unsigned long int *threadId;

    threadId = (unsigned long int *)pthread_self();

    pthread_mutex_lock(&carQueueMutex);
    carCounter++;
    pthread_mutex_unlock(&carQueueMutex);

    printf("-CAR:%lu   car Queued\n", *threadId);
    sem_wait(&carsQueueSem);
    printf("-CAR:%lu   car leaving queue to load\n", *threadId);

    printf("-CAR:%lu   car onboard ferry \n", *threadId);
    sem_post(&carsLoadSem);

    sem_wait(&vehiclesSailSem);
    printf("-CAR:%lu   car traveling\n", *threadId);
    sem_wait(&vehiclesArriveSem);
    printf("-CAR:%lu   car arrived at destination\n", *threadId);
    sem_post(&readyUnloadSem);

    sem_wait(&waitUnloadSem);

    pthread_mutex_lock(&carsUnloadedMutex);
    carsUnloadCnt++;
    pthread_mutex_unlock(&carsUnloadedMutex);

    printf("-CAR:%lu   unloaded from ferry\n", *threadId);
    sem_post(&carsUnloadSem);

    sem_wait(&waitExitSem);
    printf("-CAR:%lu   car exits\n", *threadId);

    pthread_exit(0);
}

int main(int argc, char **argv)
{
    sem_init(&carsQueueSem, 0, 0);
    sem_init(&truckQueueSem, 0, 0);
    sem_init(&carsLoadSem, 0, 0);
    sem_init(&trucksLoadSem, 0, 0);
    sem_init(&vehiclesSailSem, 0, 0);
    sem_init(&vehiclesArriveSem, 0, 0);
    sem_init(&waitUnloadSem, 0, 0);
    sem_init(&readyUnloadSem, 0, 0);
    sem_init(&carsUnloadSem, 0, 0);
    sem_init(&trucksUnloadedSem, 0, 0);
    sem_init(&waitExitSem, 0, 0);

    tm.tv_sec = 0;
    tm.tv_nsec = maxTimeToNextArrival * 300000;

    printf("Enter the percent probability that the next vehicle is a truck[0, 100]\n");
    scanf("%d", &truckArrivalProb);
    while (truckArrivalProb < 0 || truckArrivalProb > 100) {
        printf("Probability should be [0, 100] >> ");
        scanf("%d", &truckArrivalProb);
    }

    maxTimeToNextArrival = 0;
    while( maxTimeToNextArrival < 100) {
        printf("Enter the interval time between vehicles\n");
        printf("time interval should be >100\n");
        scanf("%d", &maxTimeToNextArrival );
    }

    pthread_create(&vehicleCreationThread, NULL, feryVehicleCreateThread, NULL);
    pthread_create(&captainThread, NULL, feryCaptainThread, NULL);

    pthread_mutex_init(&carQueueMutex, NULL);
    pthread_mutex_init(&trucksQueueMutex, NULL);
    pthread_mutex_init(&carsUnloadedMutex, NULL);
    pthread_mutex_init(&trucksUnloadMutex, NULL);


    pthread_join(vehicleCreationThread, NULL);
    pthread_join(captainThread, NULL);

    pthread_mutex_destroy(&trucksQueueMutex);
    pthread_mutex_destroy(&carQueueMutex);
    pthread_mutex_destroy(&carsUnloadedMutex);
    pthread_mutex_destroy(&trucksUnloadMutex);

    sem_destroy(&carsQueueSem);
    sem_destroy(&truckQueueSem);
    sem_destroy(&carsLoadSem);
    sem_destroy(&trucksLoadSem);
    sem_destroy(&vehiclesSailSem);
    sem_destroy(&vehiclesArriveSem);
    sem_destroy(&waitUnloadSem);
    sem_destroy(&readyUnloadSem);
    sem_destroy(&carsUnloadSem);
    sem_destroy(&trucksUnloadedSem);
    sem_destroy(&waitExitSem);

    return 0;
}



