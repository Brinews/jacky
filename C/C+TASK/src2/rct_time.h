/*
 * string time, 2015-04
 */
#ifndef _RCT_TIME_H
#define _RCT_TIME_H

#include <string>

using namespace std;

class Time {
private:
	int year;
	int month;
	int day;

	string dateString;
public:
	Time(string date);

	static string nowDay(); //返回当天日期
	static int diffDays(string beforeTime, string afterTime);
	static int baseDays(string oneTime);
	static bool isLeapYear(int year);

	/* 
	 * 日期比较, 日期操作
	 * Maybe there is too much need implement,
	 * but we do not consider now.
	 */
	string addDay();
	string minusDay();
};

#endif
