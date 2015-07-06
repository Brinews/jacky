#include "TimeCounter.h"


/*
 * ��ʱ����, ��ʾ��Ϸ��ʼ�󾭹���ʱ��
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

/* ����ʱ���ֵ */
void TimeCounter::SetTime(int minutes, int seconds)
{
	_minutes = minutes;
	_seconds = seconds;
}


/* ��ʱ */
void TimeCounter::Decriment()
{
	_seconds++;

	if (_seconds > 60)
	{
		_minutes++;
		_seconds -= 60;
	}
}

/* ��ȡʱ�䣺���� */
char* TimeCounter::GetMinutes()
{
	std::ostringstream out;
	out << _minutes;

	char* output =  strdup(out.str().c_str());
	return output;
	
}

/* ��ȡʱ�䣺���� */
char* TimeCounter::GetSeconds()
{
	std::ostringstream out;
	out << _seconds;

	char* output =  strdup(out.str().c_str());
	return output;

}

/* ��ʾʱ�� */
std::string TimeCounter::GetCountDown()
{
	sprintf(_pCharBuffer, "%02i", _minutes);
	sprintf(_buffer2, "%02i", _seconds);

	_stringFps = "Time Passed: " + std::string(_pCharBuffer)+ ":"+std::string(_buffer2);

	return _stringFps;
}