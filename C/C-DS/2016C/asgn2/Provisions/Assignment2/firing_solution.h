// KIT107 Assignment 2: Firing Solution
/*
 * firing_solution.h
 * Specification for the Firing Solution ADT
 * Author Julian Dermoudy
 * Version 15/8/16
*/

struct firing_solution_int;
typedef struct firing_solution_int *firing_solution;

void init_firing_solution(firing_solution *sp, my_time t, int d, double x, double y);
void set_time(firing_solution s, my_time t);
my_time get_time(firing_solution s);
void set_degree(firing_solution s, int d);
int get_degree(firing_solution s);
void set_x(firing_solution s, double x);
double get_x(firing_solution s);
void set_y(firing_solution s, double y);
double get_y(firing_solution s);

