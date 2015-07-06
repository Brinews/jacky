#pragma once

#include "Input.h"
#include "d3dx9.h"
#include "MenuImages.h"


class EndMenu
{
public:
	EndMenu();
	~EndMenu();

	bool Intialize(Input* input, HWND hwnd, int width, int height);
	void update();
	void render();
	int getState();

	enum{MAIN_MENU = 1, EXIT = 2};

	void CleanUp();
	void ResetMenu();

private:

	Input* _userInput;
	MenuImages* _background;
	MenuImages* _mainMenu;
	MenuImages* _mainMenuHigh;
	MenuImages* _exitGame;
	MenuImages* _exitGameHigh;
	

	int _menuItemSelected;
	int _state;
	int _count;

	bool IntializeDirectx(HWND hwnd, int width, int height, bool fullscreen);

	LPDIRECT3D9             _direct3d; //This is the Direct3D object
	LPDIRECT3DDEVICE9       _direct3dDevice; //This is our device object
};

