#include "GameManager.h"


GameManager::GameManager(Input* userInput)
{
	_input = userInput;
	_mainGame = 0;
	_gameMenu = 0;
	_gameTime = 0;
	_endMenu = 0;
	_timeCountDown = 0;
	_DecrimentTime = 0;
}


GameManager::~GameManager(void)
{
}


bool GameManager::Initialise(HWND hWnd, HINSTANCE instance, UINT screenWidth, UINT screenHeight, BOOL windowed)
{
	_hWnd = hWnd;
	_instance = instance;
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	
	_timeCountDown = new TimeCounter;
	_gameMenu = new Menu;

	if( !(_gameMenu->Intialize(_input,hWnd,screenWidth,screenHeight)) )
	{
		MessageBoxA(NULL, "Failed to initialise the Menu.", NULL, MB_OK);
		return 0;
	}
	activeState = MENU;


}

/* 游戏状态控制 */
void GameManager::update(DWORD timeDelta, std::string fps, DWORD currentTime)
{

	bool RungameTemp = true;

	if (activeState == MENU)
	{
		_gameMenu->update();

		if(_gameMenu->getState() == Menu::EXIT)
		{
			PostQuitMessage(0);
		}

		if ((_gameMenu->getState() == Menu::NEW_GAME))
		{
			_mainGame = new Environment(_input);

			if( !(_mainGame->Initialise(_hWnd, _instance, _screenWidth, _screenHeight, true)) )
			{
				MessageBoxA(NULL, "Failed to initialise the Maingame.", NULL, MB_OK);
			}
			activeState = GAME;
			_timeCountDown->SetTime(0,0);
			_gameTime = currentTime;
			_DecrimentTime = currentTime;
		}
	}

	if (activeState == ENDSCORE)
	{
		
		_endMenu->update();

		if(_endMenu->getState() == EndHighscores::EXIT)
		{
			PostQuitMessage(0);
		}

		if ((_endMenu->getState() == EndHighscores::MAIN_MENU))
		{
			if (!_input->IsEnterPressed())
			{
				activeState = MENU;
			}
			
		}
	}

	if (activeState == MENU)
	{
		_gameMenu->render();
	}

	if (activeState == GAME)
	{
		if(currentTime > _DecrimentTime + 1000)
		{
			_timeCountDown->Decriment();
			_DecrimentTime = currentTime;
		}

		if ( currentTime < _gameTime + 3600000) /* 最长允许1小时 */
		{
			_mainGame->Update();
			_mainGame->Render(timeDelta,fps,_timeCountDown->GetCountDown());
		}
		else
		{
			_endMenu = new EndHighscores();
			if( !(_endMenu->Intialize(_input,_hWnd,_screenWidth,_screenHeight)) )
			{
				MessageBoxA(NULL, "Failed to initialise the Menu.", NULL, MB_OK);
				
			}
			_gameMenu->ResetMenu();
			activeState = ENDSCORE;
		}
	}

	if (activeState == ENDSCORE)
	{
		_endMenu->render(_mainGame->getScore());
	}
	
}