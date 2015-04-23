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

	static string nowDay(); //���ص�������
	static int diffDays(string beforeTime, string afterTime);
	static int baseDays(string oneTime);
	static bool isLeapYear(int year);

	/* 
	 * ���ڱȽ�, ���ڲ���
	 * Maybe there is too much need implement,
	 * but we do not consider now.
	 */
	string addDay();
	string minusDay();
};

#endif
