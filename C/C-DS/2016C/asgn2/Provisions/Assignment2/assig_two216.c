// KIT107 Assignment 2: Harness
/*
 * asig_two216.c
 * Assignment 2
 * Author <<INSERT YOUR NAME AND STUDENT NUMBER HERE>>
 * Version 15/8/16
 */

#include <math.h>
#include <stdio.h>
//#include <stdbool.h>
#include "my_time.h"
#include "firing_solution.h"
#include "node.h"

const int MAJORRADIUS = 5000; /* elliptical path's major radius */
const int MINORRADIUS = 50; /* elliptical path's minor radius */
const double FLYTIMESLICE = 43.635; /* missile speed time slice for 1 segments */
const double MOVETIMESLICE = 35; /* laser require 35s to move 1' */
const double CHARGETIME = 65; /* laser's charge time */

const double PI = 3.1415926;

void add(node *h, firing_solution s)
{
    if (*h == NULL)
    {
        /* malloc and init node */
        init_node(h, s);
    }
    else
    {
        node n = NULL, p = NULL; /* node pointer */
        init_node(&n, s); /* malloc a new node */
        p = *h; /* pointer to head node */

        /* reach the end of list */
        while (get_next(p) != NULL)
        {
            p = get_next(p);
        }
        /* append to the last*/
        set_next(p, n);
    }
}

void aim(node h, my_time t)
{
    int deg = 0; /* degree of missile from Launchtown to Doomsville */
    int laser_deg = 90; /* laser's init degree */
    int missile_deg = 0; /* missile's init degree */

    for (deg = 0; deg <= 180; deg++)
    {
        double x = MAJORRADIUS * cos(deg/360.0*2*PI); /* impact x-position */
        double y = MINORRADIUS * sin(deg/360.0*2*PI); /* impact y-position */

        /* find solution */
        double laser_time = abs(laser_deg - deg) * MOVETIMESLICE; /* laser moving time */
        double missile_time = abs(deg - missile_deg) * FLYTIMESLICE; /* missile flying time */

        /* comparing moving time and charging time, get max */
        if (laser_time < CHARGETIME)
        {
            laser_time = CHARGETIME;
        }

        /* can fire */
        if (laser_time <= missile_time)
        {
            firing_solution fs = NULL; /* to add solution */
            my_time nt; /* to add time */

            init_my_time(&nt);
            assign_time(nt, 12, 30, 0);

            /* first fire time */
            if (equals(nt, t))
            {
                add_time(t, (deg-missile_deg-1)*FLYTIMESLICE);
                add_time(t, FLYTIMESLICE);
            }

            /* update firing time */
            copy_time(nt, t);

            init_firing_solution(&fs, nt, deg, x, y);
            add(&h, fs);

            /* update impacting degree after last impact */
            laser_deg = deg;
            missile_deg = deg;
        }

        /* non-first fire */
        if (get_next(h) != NULL)
        {
            add_time(t, FLYTIMESLICE);
        }
    }
}

void display(node h)
{
    firing_solution fs;

    /* traverse all node in list */
    while (get_next(h) != NULL)
    {
        h = get_next(h);

        fs = (firing_solution) get_data(h);
        printf("At time %s, fire at %3d degrees to impact missile at (%9.3lf, %6.3lf)\n",
                to_string(get_time(fs)), get_degree(fs), get_x(fs), get_y(fs));
    }
}

int main(int argc, char * argv[])
{
    node solution_list = NULL; /* firing solutions */
    my_time start_time; /* missile start time */

    init_node(&solution_list, NULL);
    init_my_time(&start_time);
    assign_time(start_time, 12, 30, 0);

    aim(solution_list, start_time); /* find solution */
    display(solution_list); /* display solution */

    return 0;
}
