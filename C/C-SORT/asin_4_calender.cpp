/***
 * Program Name : wcalender
 * Date : 2014-03-09
 */

#include <stdio.h>
#include <stdlib.h>

#define DAYOFWEEK	7
#define LINES		5

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0])

/**
 * Global Vars 
 */
int DayOfMonth[2][12] = {
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

char *WeekTitle[DAYOFWEEK] 
	   = {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"};
int Months[LINES][DAYOFWEEK] = {0};


/***
 * Length of Month
 * @para(year, month)
 * ret: N days
 */
int wcal_getMonthDays(int year, int month)
{
	int isLeap = 0;

	if ((year % 4 == 0 && year % 100 != 0) 
		|| year % 400 == 0)
		isLeap = 1;

	return DayOfMonth[isLeap][month-1];
}

/***
 * Day of the Year
 * @para(year, month, day)
 * ret : Nth day of year
 */
int wcal_getNthYearDay(int year, int month, int day)
{
	int i, Nth = day;

	for (i = 1; i <= month - 1; i++)
	{
		Nth += wcal_getMonthDays(year, i);
	}

	return Nth;
}

/***
 * Day of the Century
 * @para(year, month, day)
 * ret: Nth day of century
 */
int wcal_getNthCenturyDay(int year, int month, int day)
{
	int i, Nth = wcal_getNthYearDay(year, month, day);

	/* year start from 2000 */
	for (i = 2000; i <= year - 1; i++)
		Nth += wcal_getNthYearDay(i, 12, 31);

	return Nth;
}

/***
 * Day of forever
 * @para(year, month, day)
 * ret : Nth day of forever
 */
int wcal_getNthForeverDay(int year, int month, int day)
{
	int i, Nth = wcal_getNthYearDay(0, month, day);

	for (i = 0; i <= year - 1; i++)
		Nth += wcal_getNthYearDay(i, 12, 31);

	return Nth;
}

/***
 * Day of Week
 * @para(year, month, day)
 * ret: week number
 */
int wcal_getWeekDay(int year, int month, int day)
{
	int days = wcal_getNthForeverDay(year, month, day);

	return (days-1) % DAYOFWEEK;
}

/***
 * Calender for a month
 * @para(year, month)
 * ret : Months
 */
void wcal_getCalenderMonth(int year, int month)
{
	int i, j, start = 1;
	int days = wcal_getMonthDays(year, month);
	int weekday = wcal_getWeekDay(year, month, 1);

	for (i = 0; i < 5; i ++)
	{
		for (j = 0; j < 7; j++)
		{
			if (0 == i && j < weekday) 
				Months[i][j] = 0;
			else if (start <= days)
			{
				Months[i][j] = start++;
			}
			else 
				Months[i][j] = 0;
		}
	}
}

/***
 * Print calender
 * @para(year, month)
 */
void wcal_printCalender(int year, int month)
{
	int i, j;
	wcal_getCalenderMonth(year, month);

	for (i = 0; i < 7; i++)
		printf("%s ", WeekTitle[i]);
	printf("\n");

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 7; j++)
		{
			if (Months[i][j] == 0) printf("  ");
			else
				printf("%2d", Months[i][j]);
			
			printf(" ");
		}

		if (i == 4 || Months[i+1][0] == 0) break;

		printf("\n");
	}

	printf("\n");
}

/***
 * Extra Function: calculate sundays in any given year
 * @para(year)
 * ret : sundays count
 */
int wcal_getSundaysCount(int year)
{
	int days = wcal_getNthYearDay(year, 12, 31);
	int suCnt = (int) (days / DAYOFWEEK);
	int left = days % DAYOFWEEK;
	int s = wcal_getWeekDay(year, 1, 1);

	/*
	printf("days: %d, left:%d, s:%d\n", 
			days, left, s);
	 */

	while (left--)
	{
		s = (s+1) % DAYOFWEEK;
		if (s == DAYOFWEEK - 1) suCnt++;
	}

	return suCnt;
}

int main(int argc, char **argv)
{
	int year, month, day;

	printf("Input (Year, Month, Day)  ([-1, -1, -1] for end):\n");
	while (scanf("%d%d%d", &year, &month, &day)) 
	{
		if (year == -1) break;

		printf("Century Day Number: %d\n", 
				wcal_getNthCenturyDay(year, month, day));
		printf("Forever Day Number: %d\n", 
				wcal_getNthForeverDay(year, month, day));

		printf("Sunday Count in Year: %d\n",
				wcal_getSundaysCount(year));

		printf("\n");
		wcal_printCalender(year, month);
	}

	return 0;
}
