#pragma once
#include <string>
#include <Windows.h>
#include <mmsystem.h>

class TimeManager
{
	DWORD _startTime;

	DWORD _currentTime;
	DWORD _lastTime;
	DWORD _timeDelta;
	DWORD _timeElapsed;

	UINT _frameCount;

	float _fps;
	char* _pCharBuffer;
	std::string _stringFps;

public:
	TimeManager();
	~TimeManager();

	void SaveStartTime();
	DWORD GetStartTime();

	void UpdateLastTime();
	void UpdateCurrentTime();
	void UpdateTimeDelta();
	DWORD GetTimeDelta();
	DWORD GetCurrentT();
	
	std::string GetFPS(DWORD inTimeDelta);

	void CleanUp();
};
