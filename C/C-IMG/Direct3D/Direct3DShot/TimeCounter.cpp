#include "TimeCounter.h"


/*
 * 计时器类, 显示游戏开始后经过的时间
 */
TimeCounter::TimeCounter()
{
	_minutes = 0;
	_seconds = 0;
	_pCharBuffer = new char[16];
	_buffer2 = new char[16];
}


TimeCounter::~TimeCounter()
{
}

/* 设置时间初值 */
void TimeCounter::SetTime(int minutes, int seconds)
{
	_minutes = minutes;
	_seconds = seconds;
}


/* 计时 */
void TimeCounter::Decriment()
{
	_seconds++;

	if (_seconds > 60)
	{
		_minutes++;
		_seconds -= 60;
	}
}

/* 获取时间：分钟 */
char* TimeCounter::GetMinutes()
{
	std::ostringstream out;
	out << _minutes;

	char* output =  strdup(out.str().c_str());
	return output;
	
}

/* 获取时间：秒数 */
char* TimeCounter::GetSeconds()
{
	std::ostringstream out;
	out << _seconds;

	char* output =  strdup(out.str().c_str());
	return output;

}

/* 显示时间 */
std::string TimeCounter::GetCountDown()
{
	sprintf(_pCharBuffer, "%02i", _minutes);
	sprintf(_buffer2, "%02i", _seconds);

	_stringFps = "Time Passed: " + std::string(_pCharBuffer)+ ":"+std::string(_buffer2);

	return _stringFps;
}