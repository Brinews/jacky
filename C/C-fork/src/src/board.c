#include <stdio.h>
#include <stdlib.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include <string.h>

#include <signal.h>

#include "common.h"

#define pthread_num 2

pthread_t threads[pthread_num];

int get_shm();

int get_shm()
{
	char path[32];
	int shm_id;

	strcpy(path, PATHNAME);

	key_t key = ftok(path, 0x03);
	if (key == -1) {
		fprintf(stderr,"ftok error\n");
		return -1;
	}

	/* find the exist shared memory */
	shm_id = shmget(key, 0, 0);

	if (shm_id == -1) {
		perror("shmget");
		fprintf(stderr, "shmget error\n");
		return -1;
	}

	return shm_id;
}

void *displayThread(void *arg)
{
	elevator *eva = (elevator *) arg;

	while (1) {

		sleep(4);

		char *direc;
		if (eva->direction == 0) direc = "UP";
		else if (eva->direction == 1) direc = "DOWN";
		else direc = "STOP";

		printf("Elevator State: At Floor-[%d], Direction [%s].\n",
				eva->floor_no+1, direc);
	}

	return ((void *)NULL);
}

void *inputThread(void *arg)
{
	elevator *eva = (elevator *) arg;

	int from_floor, to_floor;

	while (1) {

		sleep(1);

		printf("Please Input InFloor(1-%d), ToFloor(1-%d), (0,0)for exit:\n",
				FLOORNUM, FLOORNUM);

		scanf("%d%d", &from_floor, &to_floor);

		if (from_floor == 0 && to_floor == 0) break;

		if (from_floor < 1 || from_floor > FLOORNUM
				|| to_floor < 1 || to_floor > FLOORNUM
				|| to_floor == from_floor) {
			fprintf(stderr, "Invalid input(InFloor Should not equal to ToFloor).\n");
			continue;
		}

		/* set board status */
		eva->eva_board.btn_on[to_floor-1] = 1;
	}

	pthread_cancel(threads[0]); /* notify inputThread to exit */
	pthread_exit(0);

	return (void*) NULL;
}

int main(int argc, char **argv)
{
	int shm_id, i;

	shm_id = get_shm();
	if (shm_id == -1) {
		fprintf(stderr, "shared memory get error\n");
		return -1;
	}


	/* map share memory */
	elevator *eva = (elevator *)shmat(shm_id, NULL, 0);

	pthread_create(&threads[0], NULL, displayThread, eva);
	pthread_create(&threads[1], NULL, inputThread, eva);

	for (i = 0; i < pthread_num; i++)
		pthread_join(threads[i], NULL);

	if (shmdt(eva) == -1) {
		fprintf(stderr, "detach shared memory failed.\n");
		return -1;
	}

	return 0;
}
