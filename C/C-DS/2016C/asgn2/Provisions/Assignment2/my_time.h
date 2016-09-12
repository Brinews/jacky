// KIT107 Assignment 2: Time
/*
 * my_time.h
 * Specification for Time 
 * Author Julian Dermoudy
 * Version 15/8/16
*/

//#include <stdbool.h>
typedef int bool;

struct my_time_int;
typedef struct my_time_int *my_time;

void init_my_time(my_time *t);
void copy_time(my_time t, my_time nt);
void assign_time(my_time t, int h, int m, int s);
void add_time(my_time t, double s);
char *to_string(my_time t);
char *to_mil_string(my_time t);
bool equals(my_time this, my_time that);
