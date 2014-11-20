#include <stdio.h>
#include <stdlib.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include <string.h>

#include <signal.h>

#include "common.h"

/* function prototype */
int get_shared_mem();
void intr_handler(int);

int has_none_request();
int max_request_floor();
int min_request_floor();

int has_up_request();
int has_down_request();
int has_reached_request(state s);
void complete_request(state s);

elevator *eva = NULL;
int shm_id;

int debuginfo = 0;

int has_none_request()
{
	int i;
	for (i = 0; i < FLOORNUM; i++) {
		if (eva->eva_floor.floors[i].floor_up
				|| eva->eva_floor.floors[i].floor_down)
			return 0; /* has request */
	}

	for (i = 0; i < FLOORNUM; i++) {
		if (eva->eva_board.btn_on[i] == 1)
			return 0;
	}

	return 1;
}

int has_up_request()
{
	int i = eva->floor_no;
	/* request from people */
	for ( ; i < FLOORNUM; i++) {
		if (eva->eva_floor.floors[i].floor_up
				|| eva->eva_floor.floors[i].floor_down)
			return 1; /* has request in higher floor */
	}

	/* request from board */
	i = eva->floor_no;
	for (; i < FLOORNUM; i++) {
		if (eva->eva_board.btn_on[i] == 1)
			return 1;
	}

	return 0;
}

int has_down_request()
{
	int i = eva->floor_no;
	for (; i >= 0; i--) {
		if (eva->eva_floor.floors[i].floor_up
				|| eva->eva_floor.floors[i].floor_down)
			return 1; /* has request in lower floor */
	}

	/* request from board */
	i = eva->floor_no;
	for (; i >= 0; i--) {
		if (eva->eva_board.btn_on[i] == 1)
			return 1;
	}

	return 0;
}

int max_request_floor()
{
	int i = FLOORNUM - 1;
	for (; i >= 0; i--) {
		if (eva->eva_floor.floors[i].floor_up == 1
				|| eva->eva_floor.floors[i].floor_down == 1)
			return i;

		if (eva->eva_board.btn_on[i] == 1)
			return i;
	}

	return FLOORNUM - 1;
}

int min_request_floor()
{
	int i = 0;
	for (; i < FLOORNUM; i++) {
		if (eva->eva_floor.floors[i].floor_up == 1
				|| eva->eva_floor.floors[i].floor_down == 1)
			return i;

		if (eva->eva_board.btn_on[i] == 1)
			return i;
	}

	return 0;
}

int has_reached_request(state s)
{
	if (s == UP && eva->eva_floor.floors[eva->floor_no].floor_up == 1)
		return 1;

	if (s == DOWN && eva->eva_floor.floors[eva->floor_no].floor_down == 1)
		return 1;

	if (eva->eva_board.btn_on[eva->floor_no] == 1)
		return 1;

	return 0;
}

/* when request complete, clear the request state.  */
void complete_request(state s)
{
	if (s == UP)
		eva->eva_floor.floors[eva->floor_no].floor_up = 0;

	if (s == DOWN)
		eva->eva_floor.floors[eva->floor_no].floor_down = 0;

	eva->eva_board.btn_on[eva->floor_no] = 0;

	printf ("Elevator Arrived at floor-[%d], Open Door & Close Door.\n",
			eva->floor_no);
}

/* used for clean when CTRL+C */
void intr_handler(int sig)
{
	printf("\nSIGINT caught...\n");

	shmdt(eva);
	shmctl(shm_id, IPC_RMID, NULL);

	exit(0);
}

int get_shared_mem()
{
	key_t key;

	char path[32];
	strcpy(path, PATHNAME);

	key = ftok(path, 0x03);
	if (key == -1) {
		fprintf(stderr, "ftok error\n");
		return -1;
	}

	/* get one page */
	shm_id = shmget(key, 4096, IPC_CREAT|IPC_EXCL|0666);

	return shm_id;
}

int main(int argc, char **argv)
{
	int shm_id;
	shm_id = get_shared_mem();
	if (shm_id == -1) {
		fprintf(stderr, "get shared memory error\n");
		return -1;
	}

	/* signal action register */
	struct sigaction nact;
	nact.sa_handler = intr_handler;
	nact.sa_flags = SA_RESTART;

	sigaction(SIGINT, &nact, NULL);

	/* share memory match */
	eva = (elevator *) shmat(shm_id, NULL, 0);
	eva->eva_state = STOP; /* default stop */
	eva->direction = -1;    /* default up direction */

	/* main loop */
	while (1) {

		sleep(3);

		if (debuginfo) {
			printf("State: %d, FLOOR:%d, Direction:%d\n",
					eva->eva_state, eva->floor_no + 1, eva->direction);
		}

		/* state run */
		switch (eva->eva_state) {
			case STOP:
				if (has_none_request()){
					eva->direction = -1;
					printf("Elevator Stoped At floor-[%d].\n", eva->floor_no);
					break;
				}

				if ((eva->direction == -1 || eva->direction == 0)
						&& has_up_request()) { /* up */
					eva->eva_state = UP;
					eva->direction = 0; /* 0 :up, 1:down */
				}

				if ((eva->direction == -1 || eva->direction == 1 )
						&& has_down_request()) { /* down */
					eva->eva_state = DOWN;
					eva->direction = 1; /* 0 :up, 1:down */
				}

				break;

			case UP:
				if (has_reached_request(UP)) {
					complete_request(UP);

					eva->eva_state = STOP;
				}
				else {
					if (eva->floor_no >= max_request_floor()) {
						/* reach the max */
						eva->eva_state = DOWN;
						eva->direction = 1;
					}
					else {
						eva->floor_no++; /* still up */
					}
				}

				break;

			case DOWN:
				if (has_reached_request(DOWN)) {
					complete_request(DOWN);

					eva->eva_state = STOP;
				} else {
					if (eva->floor_no <= min_request_floor()) {
						eva->eva_state = UP;
						eva->direction = 0;
					}
					else {
						eva->floor_no--; /* still down */
					}
				}

				break;

			default:
				break;
		}

		printf("\nCTRL+C to exit.\n");

		if (debuginfo) {
			int i;
			printf("\nElevator State:\n");
			for (i = 0; i < FLOORNUM; i++)
			{
				printf("%d: %d,%d\n", i+1,
						eva->eva_floor.floors[i].floor_up,
						eva->eva_floor.floors[i].floor_down);
			}

			printf("\nBoard State:\n");
			for (i = 0; i < FLOORNUM; i++)
			{
				int is_on = eva->eva_board.btn_on[i];
				printf("%d : %s\n", i+1, is_on ? "ON" : "OFF");
			}
		}
	}

	if (shmdt(eva) == -1) {
		fprintf(stderr, "detach error\n");
		return -1;
	}

	/* remove shared memeory */
	shmctl(shm_id, IPC_RMID, NULL);

	return 0;
}
