#ifndef _LANDER_H
#define _LANDER_H

// need to define this macro as functions we're using are not C99
// compliant
#define _POSIX_C_SOURCE 	200112L
#define PI				 	(4.0*atan(1.0))

#define SHIPMAX	5
#define LANDMAX 25

#include <signal.h>
#include <sys/time.h>

#include <curses.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<ctype.h>
#include<math.h>
#include<assert.h>

typedef struct MapPoint{
	double x;
	double y;
} MapPoint;

extern FILE* sketchpad;
extern FILE* landscape;

extern MapPoint ship[5];
extern MapPoint LANDSCAPE[25];
extern int land_point_cnt;

extern long ship_degree;
extern int thrust_on,is_over;

extern int opt;
extern double fuel;

extern double xV,yV;
extern double gravity,thrust;

extern sigset_t block_mask_g;

void parse_args();
void ship_init();

FILE* open_pipe(const char* exec_name);
void close_pipe(FILE *);

double angle_to_radian(long angle);
void rotate_by_degree(MapPoint* P, int clockwise);

//timer handler
void handle_timeout(int signal);
void ship_rotate(int direction);

//intersection judge
int is_intersect(MapPoint A,MapPoint B,MapPoint C,MapPoint D);

//curses function 
void setup_curses();
void unset_curses();

void set_color(int r, int g, int b);

void draw_line(MapPoint A,MapPoint B);
void erase_line(MapPoint A,MapPoint B);

void draw_ship();
void draw_thrust();
void erase_ship();
void erase_thrust();

void draw_landscape(FILE* landscape);

//optimization
void draw_fuel();
void erase_fuel(double fuel);

#endif

