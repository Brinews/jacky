// KIT107 Assignment 2: Firing Solution
/*
 * firing_solution.c
 * Implementation for Firing Solution
 * Author Julian Dermoudy and <<INSERT YOUR NAME AND STUDENT ID HERE>>
 * Version 15/8/16
 */

#include "my_time.h"
#include "firing_solution.h"
#include <stdio.h>
#include <stdlib.h>


struct firing_solution_int {
    my_time time;
    int degree;
    double x;
    double y;
};

/*
 * 'Constructor' for firing_solution
 */
void init_firing_solution(firing_solution *sp, my_time t, int d, double x, double y)
{
    *sp = (firing_solution) malloc(sizeof(struct firing_solution_int));
    if (*sp == NULL)
    {
        fprintf(stderr, "malloc firing solution error\n");
        return;
    }

    /* initialise */
    (*sp)->time = t;
    (*sp)->degree = d;
    (*sp)->x = x;
    (*sp)->y = y;
}


/*
 * Getter for time
 * Return time field
 */
my_time get_time(firing_solution s)
{
    if (s != NULL)
    {
        return s->time;
    }

    /* NULL if s not exist */
    return NULL;
}

/*
 * Setter for time
 * Param t the value for the time field
 */
void set_time(firing_solution s, my_time t)
{
    if (s != NULL)
    {
        s->time = t;
    }
}

/*
 * Getter for degree
 * Return degree field
 */
int get_degree(firing_solution s)
{
    if (s != NULL)
    {
        return s->degree;
    }

    return 0;
}

/*
 * Setter for degree
 * Param d the value for the degree field
 */
void set_degree(firing_solution s, int d)
{
    if (s != NULL)
    {
        s->degree = d;
    }
}

/*
 * Getter for x coordinate
 * Return x field
 */
double get_x(firing_solution s)
{
    if (s != NULL)
    {
        return s->x;
    }

    return 0.0;
}

/*
 * Setter for x coordinate
 * Param v the value for the x field
 */
void set_x(firing_solution s, double v)
{
    if (s != NULL)
    {
        s->x = v;
    }
}

/*
 * Getter for y coordinate
 * Return y field
 */
double get_y(firing_solution s)
{
    if (s != NULL)
    {
        return s->y;
    }

    return 0.0;
}

/*
 * Setter for y coordinate
 * Param v the value for the y field
 */
void set_y(firing_solution s, double v)
{
    if (s != NULL)
    {
        s->y = v;
    }
}

