// KIT107 Assignment 2: Time
/*
 * my_time.c
 * Implementation for Time 
 * Author Julian Dermoudy and <<INSERT YOUR NAME AND STUDENT ID HERE>>
 * Version 15/8/16
 */

#include "my_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdbool.h>


const int HOUR = 24; /* hours a day */
const int MINUTE = 60; /* minutes an hour */
const int SECOND = 60; /* seconds a minute */
const int ONESECOND = 1; /* one second */
char time_disp[12]; /* display time buffer */

char *section[2] = {"AM", "PM"}; /* morning or afternoon */

struct my_time_int {
    int hour;
    int minute;
    int second;
};

/*
 * 'Constructor' for my_time
 */
void init_my_time(my_time *t)
{
    *t = (my_time) malloc(sizeof(struct my_time_int));
    if (*t == NULL) 
    {
        fprintf(stderr, "malloc my_time error\n");
        return;
    }

    (*t)->hour = (*t)->minute = (*t)->second = 0;
}

/*
 * Alter time
 * Param my_time nt
 */
void copy_time(my_time t, my_time nt)
{
    if (t != NULL && nt != NULL)
    {
        t->hour = nt->hour;
        t->minute = nt->minute;
        t->second = nt->second;
    }
}

/*
 * Alter hour, minute, and second
 * Param h new value for hour
 * Param m new value for minute
 * Param s new value for second
 */
void assign_time(my_time t, int h, int m, int s) 
{
    if (t != NULL)
    {
        t->hour = h;
        t->minute = m;
        t->second = s;
    }
}

/*
 * Increment the hour component of my_time
 */
void inc_hour(my_time t)
{
    if (t != NULL)
    {
        /* hour must be in [0, 24) */
        t->hour = (t->hour+1)%HOUR;
    }
}

/*
 * Increment the minute component of my_time, incrementing hour if need be
 */
void inc_minute(my_time t)
{
    if (t != NULL)
    {
        /* 60 minute is an hour */
        if (t->minute+1 == MINUTE)
        {
            inc_hour(t);
        }

        t->minute = (t->minute+1)%MINUTE;
    }
}

/*
 * Increment the second component of my_time, incrementing minute if need be
 */
void inc_second(my_time t)
{
    if (t != NULL)
    {
        /* 60 seconds is a minute */
        if (t->second+1 == SECOND)
        {
            inc_minute(t);
        }

        t->second = (t->second+1)%SECOND;
    }
}

/*
 * By incrementing, add seconds to an existing time value, discarding any fraction
 * Param s the number of seconds to add to the given variable
 */
void add_time(my_time t, double s)
{
    if (t != NULL)
    {
        /* add seconds one by one */
        while (s > ONESECOND)
        {
            s = s - ONESECOND;
            inc_second(t);
        }
    }
}


/**
 * Find printable form of time
 * Return String form of time for printing etc.
 */
char *to_string(my_time t)
{ 
    if (t != NULL) 
    {
        memset(time_disp, '\0', sizeof(time_disp));

        /* format HH:MM:SS AM/PM */
        sprintf(time_disp, "%02d:%02d:%02d %s", 
                t->hour>HOUR/2?(t->hour-HOUR/2):t->hour, 
                t->minute, t->second,
                t->hour>=HOUR/2?section[1]:section[0]);

        return time_disp;
    }

    return NULL;
}

/*
 * Find printable form of time in 24 hour mode
 * Return String form of time in 24 hour mode for printing etc.
 */
char *to_mil_string(my_time t)
{ 
    if (t != NULL) 
    {
        memset(time_disp, '\0', sizeof(time_disp));

        /* format HH:MM:SS AM/PM */
        sprintf(time_disp, "%02d:%02d:%02d %s", 
                t->hour, t->minute, t->second,
                t->hour>=HOUR/2?section[1]:section[0]);

        return time_disp;
    }

    return NULL;
}

/*
 * Find number of seconds elapsed since midnight
 * Return number of seconds elapsed since midnight as int
 */
int secs_since_midnight(my_time t)
{
    int seconds_elapsed = 0;

    if (t != NULL)
    {
        /* accumulate the hours and minutes, seconds */
        seconds_elapsed += (t->hour * MINUTE * SECOND);
        seconds_elapsed += (t->minute * SECOND);
        seconds_elapsed += t->second;
    }

    return seconds_elapsed;
}

/*
 * Check for equality of time
 * Return true if this time is the same as the provided time (that), false otherwise
 */
bool equals(my_time this, my_time that)
{
    /* compare hour, minute, second */
    return this->hour == that->hour && this->minute == that->minute
        && this->second == that->second;
}
