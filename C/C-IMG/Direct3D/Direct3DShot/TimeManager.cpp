#include "TimeManager.h"

TimeManager::TimeManager()
{
	_currentTime = 0;
	_lastTime = 0;
	_timeDelta = 0;
	_fps = 0.0f;
	_frameCount = 0;
	_timeElapsed = 0;

	_fps = 0.0;
	_pCharBuffer = new char[16];
	
}

TimeManager::~TimeManager()
{
	CleanUp();
}

void TimeManager::SaveStartTime()
{
	_startTime = timeGetTime();
}

DWORD TimeManager::GetStartTime()
{
	return _startTime;
}

void TimeManager::UpdateLastTime()
{
	_lastTime = _currentTime;
}

void TimeManager::UpdateCurrentTime()
{
	_currentTime = timeGetTime();
}

void TimeManager::UpdateTimeDelta()
{
	_timeDelta = _currentTime - _lastTime;
}

DWORD TimeManager::GetTimeDelta()
{
	return _timeDelta;
}

DWORD TimeManager::GetCurrentT()
{
	return _currentTime;
}

std::string TimeManager::GetFPS(DWORD inTimeDelta)
{
	_frameCount++;
	_timeElapsed += inTimeDelta;
	float timeElapsed = ((float)_timeElapsed / 1000.0f);
	if( timeElapsed >= 1.0f )
	{
		_fps = ((float)_frameCount) / timeElapsed;
		_frameCount = 0;
		_timeElapsed = 0;
		
		sprintf(_pCharBuffer, "%f", _fps);
		_stringFps = "FPS: " + std::string(_pCharBuffer);
	}

	return _stringFps;
}

void TimeManager::CleanUp()
{
	if(_pCharBuffer != NULL)
	{
		delete[] _pCharBuffer;
		_pCharBuffer = NULL;
	}
}