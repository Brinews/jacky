#include <stdio.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include <string.h>

#include "common.h"

#define pthread_num 2

pthread_t threads[pthread_num];

int get_shm();

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

	int no, dire;

	while (1) {

		sleep(1);

		printf("Please Input Floor No(1-%d) and Direction(0:up, 1:down), (0,0)for exit:\n", FLOORNUM);

		scanf("%d%d", &no, &dire);

		if (no == 0 && dire == 0) break;

		if (no < 1 || no > FLOORNUM
				|| (dire != 0 && dire != 1)
				|| (no == 1 && dire == 1)/*1 floor no down , max floor no up*/
				|| (no == FLOORNUM && dire == 0)) {
			fprintf(stderr, "Invalid input\n");
			continue;
		}

		if (dire == 0)
			eva->eva_floor.floors[no-1].floor_up = 1;
		else
			eva->eva_floor.floors[no-1].floor_down = 1;

	}

	pthread_cancel(threads[0]); /* notify inputThread to exit */
	pthread_exit(0);

	return (void*) NULL;
}


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
