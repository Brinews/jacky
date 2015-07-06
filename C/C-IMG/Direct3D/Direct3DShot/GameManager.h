#pragma once

#include "Input.h"
#include "Menu.h"
#include "Environment.h"
#include "EndHighscores.h"
#include "TimeCounter.h"

class GameManager
{
public:
	GameManager(Input* input);
	~GameManager(void);

	bool Initialise(HWND hWnd, HINSTANCE instance, UINT screenWidth, UINT screenHeight, BOOL windowed);
	void update(DWORD timeDelta, std::string fps,DWORD currentTime);
	enum { MENU = 1, GAME = 2, ENDSCORE = 3 };

private:

	
	Environment* _mainGame;
	Menu* _gameMenu;
	EndHighscores* _endMenu;
	Input* _input;
	int activeState;
	HWND _hWnd; 
	HINSTANCE _instance; 
	UINT _screenWidth; 
	UINT _screenHeight;
	DWORD _gameTime;
	DWORD _DecrimentTime;
	TimeCounter* _timeCountDown;

	
};

