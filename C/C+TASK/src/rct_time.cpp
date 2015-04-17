/*
 * time function, 2015-04
 */

#include "rct_time.h"

#include <cstdlib>
#include <time.h>

Time::Time(string d) : dateString(d)
{
	year = atoi(d.substr(0, 4).c_str());
	month = atoi(d.substr(5, 2).c_str());
	day = atoi(d.substr(8, 2).c_str());
}

string Time::nowDay()
{
	time_t timer;
	time(&timer);

	tm *t_tm = localtime(&timer);
	
	char buf[16];
	string ret;

	sprintf(buf, "%04d-%02d-%02d", 
		t_tm->tm_year+1900, t_tm->tm_mon+1, t_tm->tm_mday);
	printf("Date:%s\n", buf);

	ret = buf;

	return ret;
}

bool Time::isLeapYear(int y)
{
	if (y % 4 == 0 && y % 100 != 0
			|| y % 400 == 0)
		return true;

	return false;
}

/* 距离2001-01-01多少天, 确保日期在这之后 */
int Time::baseDays(string oneTime)
{
	int months[2][12] = {
		{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	};

	Time t(oneTime);
	int days = 0;

	for (int y = 2001; y < t.year; y++) {
		days += 365;
		if (isLeapYear(y)) days += 1;
	}

	int Y = 0;
	if (isLeapYear(t.year)) Y = 1;

	for (int m = 0; m < t.month; m++) {
		days += months[Y][m];
	}

	days += t.day;

	return days;
}

/* 计算出两个日期之间的天数 */
int Time::diffDays(string beforeTime, string afterTime)
{
	int before = baseDays(beforeTime);
	int after = baseDays(afterTime);

	return abs(before-after);
}
