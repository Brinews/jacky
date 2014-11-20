#ifndef _COMMON_H
#define _COMMON_H

#include <unistd.h>
#include <pthread.h>

#define PATHNAME	"/tmp/1.c"

/* basic information */

#define FLOORNUM	5

/* shared info between elevator and people */
typedef struct floor_state
{
	int floor_up;	/* 1: on, 0: off */
	int floor_down; /* 1: on, 0: off */
} floor_single;

typedef struct floor_states 
{
	floor_single floors[FLOORNUM];
} floor;

/* shared info between elevator and board */
typedef struct board_state
{
	int btn_on[FLOORNUM]; /* 1:on 0:off */
} board;

typedef enum { UP, DOWN, STOP, OPEN, CLOSE } state;

/* elevator information */
typedef struct elevator_info {
	state eva_state; /* elevator now state */
	int   floor_no;	 /* elevator now floor, default at 0 floor */
	int   direction;

	floor eva_floor;
	board eva_board;

} elevator;

#endif
