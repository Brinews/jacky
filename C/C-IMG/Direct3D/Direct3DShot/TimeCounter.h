#pragma once

#include <sstream>

class TimeCounter
{

public:
	TimeCounter();
	~TimeCounter();

	void SetTime(int minutes, int seconds);
	void Decriment();
	char* GetMinutes();
	char* GetSeconds();
	std::string GetCountDown();


private:

	int _minutes;
	int _seconds;

	char* _pCharBuffer;
	char* _buffer2;
	std::string _stringFps;
};

