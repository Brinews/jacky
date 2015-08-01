/*
 *  homework asgn-4
 *  2015-07-25
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// message types
#define MSG_IS_CAR 				0x01
#define MSG_IS_TRUCK 			0x02

#define MSG_VEHICLE_WAITING     0x20
#define MSG_VEHICLE_LATE        0x21
#define MSG_TO_WAIT             0x30

#define MSG_STATE_ONBOARD       0x10
#define MSG_STATE_TRAVELING     0x11
#define MSG_STATE_ARRIVED       0x12
#define MSG_STATE_UNLOAD        0x13
#define MSG_STATE_UNLOAD_ACK    0x14

#define SIZE_OF_CAR 	1
#define SIZE_OF_TRUCK 	2

typedef struct mymsgbuf
{
    long mtype; // message type
    int data; 
    int pid; 
} mess_t;

mess_t msg;
int msgSize;

// Queues
int msgFromCaptain;
int msgFromVechicles;

int waitingQueue;
int lateQueue;

int boardingQueue;
int onBoardQueue;

// Processes
int feryCaptainProcessPID;
int feryVehicleCreatePID;
int feryVehicleGID;

#define PROBABILITY	 100
#define MIN_INTERVAL 200
#define MAX_INTERVAL 100000

//some resource limitation
#define MAX_LOADS 	11
#define MAX_SPACES 	6
#define MAX_TRUNCKS 2

int arrivalProb;
int arrivalInterval;

int feryCaptainProcess();
int vehicleCreationProcess();
int feryCarProcess();
int feryTruckProcess();
int feryTimeInterval(const struct timeval startTime );

int feryTimeInterval(const struct timeval startTime ) {
    struct timeval nowTime;
    long int elapsed;
    int elapsedTime;

    gettimeofday(&nowTime, NULL);

    elapsed = (nowTime.tv_sec - startTime.tv_sec) * 1000000 
        + (nowTime.tv_usec - startTime.tv_usec);
    elapsedTime = elapsed / 1000;

    return elapsedTime;
}

/* main captain process */
int feryCaptainProcess() 
{
    char *vehicleType;
    int currentLoad = 1;

    int loadCars = 0;
    int loadTrucks = 0;
    int filledSpaces = 0;
    int counter = 0;

    printf("---CAPTAIN---   Captain created. pid = %d\n", getpid());

    while(currentLoad < MAX_LOADS) {
        loadTrucks = 0;
        loadCars = 0;
        filledSpaces = 0;

        printf("---CAPTAIN---   Transaction %d started\n", currentLoad);
        printf("---CAPTAIN---   The ferry is about to load!\n");
        while(msgrcv(msgFromVechicles, &msg, msgSize, 0, IPC_NOWAIT) != -1) {
            vehicleType = msg.mtype == MSG_IS_CAR ? "Car" : "Truck";
            printf("---CAPTAIN---   Captain tells %s %d that it is waiting\n", vehicleType, msg.pid);
            msg.mtype = msg.pid; msg.data = MSG_VEHICLE_WAITING;
            if(msgsnd(msgFromCaptain, &msg, msgSize, 0) == -1) {
                return -1;
            }
        }

        while(filledSpaces < MAX_SPACES) {
            while(msgrcv(msgFromVechicles, &msg, msgSize, 0, IPC_NOWAIT) != -1) {
                vehicleType = msg.mtype == MSG_IS_CAR ? "Car" : "Truck";
                printf("---CAPTAIN---   Captain tells %s %d that it is late\n", vehicleType, msg.pid);
                msg.mtype = msg.pid; msg.data = MSG_VEHICLE_LATE;
                if(msgsnd(msgFromCaptain, &msg, msgSize, 0) == -1) {
                    return -1;
                }
            }

            while(loadTrucks < MAX_TRUNCKS && 
                    filledSpaces + SIZE_OF_TRUCK <= MAX_SPACES &&
                    msgrcv(waitingQueue, &msg, msgSize, MSG_IS_TRUCK, IPC_NOWAIT) != -1) {
                printf("---CAPTAIN---   Captain tells Truck %d to leave waiting queue\n", msg.pid);
                msg.mtype = msg.pid;
                if(msgsnd(boardingQueue, &msg, msgSize, 0) == -1) {
                    return -1;
                }
                loadTrucks++;
                filledSpaces += SIZE_OF_TRUCK;
            }

            while(filledSpaces + SIZE_OF_CAR <= MAX_SPACES &&
                    msgrcv(waitingQueue, &msg, msgSize, MSG_IS_CAR, IPC_NOWAIT) != -1) {
                printf("---CAPTAIN---   Captain tells Car %d to leave waiting queue\n", msg.pid);
                msg.mtype = msg.pid;
                if(msgsnd(boardingQueue, &msg, msgSize, 0) == -1) {
                    return -1;
                }
                loadCars++;
                filledSpaces += SIZE_OF_CAR;
            }

            while(loadTrucks < MAX_TRUNCKS && 
                    filledSpaces + SIZE_OF_TRUCK <= MAX_SPACES &&
                    msgrcv(lateQueue, &msg, msgSize, MSG_IS_TRUCK, IPC_NOWAIT) != -1) {
                printf("---CAPTAIN---   Captain tells Truck %d to leave late queue\n", msg.pid);
                msg.mtype = msg.pid;
                if(msgsnd(boardingQueue, &msg, msgSize, 0) == -1) {
                    return -1;
                }
                loadTrucks++;
                filledSpaces += SIZE_OF_TRUCK;
            }

            while(filledSpaces + SIZE_OF_CAR <= MAX_SPACES &&
                    msgrcv(lateQueue, &msg, msgSize, MSG_IS_CAR, IPC_NOWAIT) != -1) {
                printf("---CAPTAIN---   Captain tells Car %d to leave late queue\n", msg.pid);
                msg.mtype = msg.pid;
                if(msgsnd(boardingQueue, &msg, msgSize, 0) == -1) {
                    return -1;
                }
                loadCars++;
                filledSpaces += SIZE_OF_CAR;
            }

            while(1) {
                int ret = msgrcv(onBoardQueue, &msg, msgSize, MSG_STATE_ONBOARD, IPC_NOWAIT);
                if(ret != -1) {
                    vehicleType = msg.data == MSG_IS_CAR ? "Car" : "Truck";
                    printf("---CAPTAIN---   Captain acks that %s %d is onboard\n", vehicleType, msg.pid);
                } else if (errno == ENOMSG) {
                    break;
                } else { return -1; }
            }

            if(loadCars * SIZE_OF_CAR + loadTrucks * SIZE_OF_TRUCK  == MAX_SPACES) {
                printf("---CAPTAIN---   Ferry is Full, Captain tells the boarded vehicles that they are sailing\n");

                msg.mtype = MSG_STATE_TRAVELING;
                for(counter = 0; counter < loadCars + loadTrucks; counter++) 
                    if(msgsnd(onBoardQueue, &msg, msgSize, 0) == -1) { return -1; }

                printf("---CAPTAIN---   All vehicles ack'd they are sailing\n");

                printf("---CAPTAIN---   Tells all the boarded vehicles that they have arrived\n");

                msg.mtype = MSG_STATE_ARRIVED;
                for(counter = 0; counter < loadCars + loadTrucks; counter++) 
                    if(msgsnd(onBoardQueue, &msg, msgSize, 0) == -1) { return -1; }

                printf("---CAPTAIN---   Captain tells the boarded vehicles to begin unloading\n");

                msg.mtype = MSG_STATE_UNLOAD;
                for(counter = 0; counter < loadCars + loadTrucks; counter++) 
                    if(msgsnd(onBoardQueue, &msg, msgSize, 0) == -1) { return -1; }

                counter = 0;

                while(counter < loadCars + loadTrucks) {
                    int ret = msgrcv(onBoardQueue, &msg, msgSize, MSG_STATE_UNLOAD_ACK, IPC_NOWAIT);
                    if(ret == msgSize) { 
                        vehicleType = msg.data == MSG_IS_CAR ? "Car" : "Truck";
                        printf("---CAPTAIN---   Captain acks that %s %d has unloaded\n", vehicleType, msg.pid);
                        counter++;
                    } else if(ret == -1 && errno != ENOMSG) { return -1; }
                }

                printf("---CAPTAIN---   All vehicles have unloaded\n");

                while(msgrcv(lateQueue, &msg, msgSize, 0, IPC_NOWAIT) != -1) {
                    msg.mtype = msg.pid; msg.data = MSG_TO_WAIT;
                    if(msgsnd(boardingQueue, &msg, msgSize, 0) == -1) { return -1; }
                }

                printf("---CAPTAIN---   All late vehicles now waiting\n");
            }

        }

        printf("\n");
        printf("---CAPTAIN---   Returned at main loading dock\n");
        printf("\n");

        currentLoad++;
        if(currentLoad >= MAX_LOADS)
            printf("---CAPTAIN---   Transaction %d . Termination\n", currentLoad);
    }

    return 0;
}

int vehicleCreationProcess() 
{
    int localpid = getpid();
    struct timeval startTime;
    int elapsed = 0;
    int lastArrivalTime = 0;

    printf("---VEHICLE---   vehicleCreationProcess created. PID is: %d \n", localpid);
    gettimeofday(&startTime, NULL);
    elapsed = feryTimeInterval(startTime);
    srand (elapsed*1000+44);
    int zombies = 0;
    while(1) { 

        elapsed = feryTimeInterval(startTime);
        if(elapsed >= lastArrivalTime) {
            printf("---VEHICLE---   elapsed time %d arrival time %d\n", elapsed, lastArrivalTime); 
            if(lastArrivalTime > 0 ) { 
                if(rand() % PROBABILITY < arrivalProb ) {
                    printf("---VEHICLE---   Created a truck process\n");
                    int childPID = fork(); 
                    if(childPID == 0) {
                        return feryTruckProcess();
                    } else if(childPID == -1) {
                        return -1;
                    }
                }
                else {
                    printf("---VEHICLE---   Created a car process\n");
                    int childPID = fork(); 
                    if(childPID == 0) {
                        return feryCarProcess();
                    } else if(childPID == -1) {
                        return -1;
                    }
                }
            }
            lastArrivalTime += rand()% arrivalInterval;
            printf("---VEHICLE---   present time %d, next arrival time %d\n", elapsed, lastArrivalTime);
        }

        zombies++;
        if(zombies % 10 == 0) {
            zombies -= 10;
            int w = 0; int status = 0;
            while((w = waitpid( -1, &status, WNOHANG)) > 1){
            }
        }
    }

    printf("---VEHICLE---   vehicleCreationProcess ended\n");

    return 0;
}

int feryCarProcess() {
    int localpid = getpid();
    setpgid(localpid, feryVehicleGID);
    printf("---CAR---   Car %d arrives at ferry dock\n", localpid);

    while(1) {
        msg.mtype = MSG_IS_CAR; msg.pid = localpid;
        if(msgsnd(msgFromVechicles, &msg, msgSize, 0) == -1) { return -1; }

        if(msgrcv(msgFromCaptain, &msg, msgSize, localpid, 0) == -1) { return -1; }
        msg.mtype = MSG_IS_CAR; msg.pid = localpid;
        if(msg.data == MSG_VEHICLE_LATE) {
            printf("---CAR---   Car %d acks that it is %s\n", localpid,  "late");
            if(msgsnd(lateQueue, &msg, msgSize, 0) == -1) { return -1; }
        } else {
            printf("---CAR---   Car %d acks that it is %s\n", localpid,  "waiting");
            if(msgsnd(waitingQueue, &msg, msgSize, 0) == -1) { return -1; }
        }

        if(msgrcv(boardingQueue, &msg, msgSize, localpid, 0) == -1) { return -1; }
        if(msg.data != MSG_TO_WAIT)
            break;
        else
            printf("---CAR---   Car %d was late from last load\n", localpid);
    }

    printf("---CAR---   Car %d acks Captain's signal to leave its queue\n", localpid);

    printf("---CAR---   Car %d is onboard the ferry\n", localpid);
    msg.mtype = MSG_STATE_ONBOARD; msg.data = MSG_IS_CAR; msg.pid = localpid;
    if(msgsnd(onBoardQueue, &msg, msgSize, 0) == -1) { return -1; }

    if(msgrcv(onBoardQueue, &msg, msgSize, MSG_STATE_TRAVELING, 0) == -1) { return -1; }
    printf("---CAR---   Car %d acks Captain's signal that it is sailing\n", localpid);

    if(msgrcv(onBoardQueue, &msg, msgSize, MSG_STATE_ARRIVED, 0) == -1) { return -1; }
    printf("---CAR---   Car %d acks Captain's signal that it has arrived\n", localpid);

    if(msgrcv(onBoardQueue, &msg, msgSize, MSG_STATE_UNLOAD, 0) == -1) { return -1; }
    printf("---CAR---   Car %d acks Captain's signal to unload\n", localpid);

    printf("---CAR---   Car %d tells Captain that it has unloaded\n", localpid);
    msg.mtype = MSG_STATE_UNLOAD_ACK; msg.data = MSG_IS_CAR; msg.pid = localpid;
    if(msgsnd(onBoardQueue, &msg, msgSize, 0) == -1) { return -1; }

    return 0;
}

int feryTruckProcess() 
{
    int localpid = getpid();
    setpgid(localpid, feryVehicleGID);
    printf("---TRUCK---   Truck %d arrives at ferry dock\n", localpid);

    while(1) {
        msg.mtype = MSG_IS_TRUCK; msg.pid = localpid;
        if(msgsnd(msgFromVechicles, &msg, msgSize, 0) == -1) { return -1; }

        if(msgrcv(msgFromCaptain, &msg, msgSize, localpid, 0) == -1) { return -1; }
        msg.mtype = MSG_IS_TRUCK; msg.pid = localpid;
        if(msg.data == MSG_VEHICLE_LATE) {
            printf("---TRUCK---   Truck %d acks that it is %s\n", localpid,  "late");
            if(msgsnd(lateQueue, &msg, msgSize, 0) == -1) { return -1; }
        } else {
            printf("---TRUCK---   Truck %d acks that it is %s\n", localpid,  "waiting");
            if(msgsnd(waitingQueue, &msg, msgSize, 0) == -1) { return -1; }
        }

        if(msgrcv(boardingQueue, &msg, msgSize, localpid, 0) == -1) { return -1; }
        if(msg.data != MSG_TO_WAIT)
            break;
        else
            printf("---TRUCK---   Truck %d was late from last load\n", localpid);
    }
    printf("---TRUCK---   Truck %d acks Captain's signal to leave its queue\n", localpid);

    printf("---TRUCK---   Truck %d is onboard the ferry\n", localpid);
    msg.mtype = MSG_STATE_ONBOARD; msg.data = MSG_IS_TRUCK; msg.pid = localpid;
    if(msgsnd(onBoardQueue, &msg, msgSize, 0) == -1) { return -1; }

    if(msgrcv(onBoardQueue, &msg, msgSize, MSG_STATE_TRAVELING, 0) == -1) { return -1; }
    printf("---TRUCK---   Truck %d acks Captain's signal that it is sailing\n", localpid);

    if(msgrcv(onBoardQueue, &msg, msgSize, MSG_STATE_ARRIVED, 0) == -1) { return -1; }
    printf("---TRUCK---   Truck %d acks Captain's signal that it has arrived\n", localpid);

    if(msgrcv(onBoardQueue, &msg, msgSize, MSG_STATE_UNLOAD, 0) == -1) { return -1; }
    printf("---TRUCK---   Truck %d acks Captain's signal to unload.\n", localpid);

    printf("---TRUCK---   Truck %d tells Captain that it has unloaded\n", localpid);
    msg.mtype = MSG_STATE_UNLOAD_ACK; msg.data = MSG_IS_TRUCK; msg.pid = localpid;
    if(msgsnd(onBoardQueue, &msg, msgSize, 0) == -1) { return -1; }

    return 0;
}

int main() 
{
    int status;

    msgSize = sizeof(mess_t) - sizeof(long);

    if((msgFromCaptain = msgget(IPC_PRIVATE, IPC_CREAT | 0660)) == -1) {
        exit(1);
    }
    if((msgFromVechicles = msgget(IPC_PRIVATE, IPC_CREAT | 0660)) == -1) {
        exit(1);
    }
    if((waitingQueue = msgget(IPC_PRIVATE, IPC_CREAT | 0660)) == -1) {
        exit(1);
    }
    if((lateQueue = msgget(IPC_PRIVATE, IPC_CREAT | 0660)) == -1) {
        exit(1);
    }
    if((boardingQueue = msgget(IPC_PRIVATE, IPC_CREAT | 0660)) == -1) {
        exit(1);
    }
    if((onBoardQueue = msgget(IPC_PRIVATE, IPC_CREAT | 0660)) == -1) {
        exit(1);
    }

    printf("---INIT---   All queues successfully created\n");
    feryVehicleGID = getpid() - 1;

    printf("Enter the probability that the next vehicle is a truck (0..%d): ", PROBABILITY);
    scanf("%d", &arrivalProb);

    while(arrivalProb < 0 || arrivalProb > PROBABILITY) {
        printf("Probability should be [0, %d]: ", PROBABILITY);
        scanf("%d", &arrivalProb);
    }

    printf("Enter the time length of the interval between vehicles (%d..%d)): ", MIN_INTERVAL, MAX_INTERVAL);
    scanf("%d", &arrivalInterval);
    while(arrivalInterval < MIN_INTERVAL ||
            arrivalInterval > MAX_INTERVAL) {
        printf("Interval must be greater than %d: ", MIN_INTERVAL);
        scanf("%d", &arrivalInterval);
    }

    if((feryCaptainProcessPID = fork()) == 0) {
        return feryCaptainProcess();
    }

    if((feryVehicleCreatePID = fork()) == 0) {
        return vehicleCreationProcess();
    }

    waitpid(feryCaptainProcessPID, &status, 0);

    kill(feryVehicleCreatePID, SIGKILL);
    waitpid(feryVehicleCreatePID, &status, 0);

    msgctl(msgFromCaptain, IPC_RMID , 0);
    msgctl(msgFromVechicles, IPC_RMID , 0);
    msgctl(waitingQueue, IPC_RMID , 0);
    msgctl(lateQueue, IPC_RMID , 0);
    msgctl(boardingQueue, IPC_RMID , 0);
    msgctl(onBoardQueue, IPC_RMID , 0);

    if(killpg(feryVehicleGID, SIGKILL) == -1 && errno == EPERM) {
        printf("vehicle processes not killed\n");
    }

    while (waitpid( -1, &status, WNOHANG) > 1){
        printf("process in cleanup\n" );
    }

    return 0;
}
