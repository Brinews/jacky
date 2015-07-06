#pragma once
#include "Input.h"
#include "d3dx9.h"
#include "MenuImages.h"
#include "MenuSprites.h"

class Menu
{
public:
	Menu();
	~Menu();

	bool Intialize(Input* input, HWND hwnd, int width, int height);
	void update();
	void render();
	int getState();

	enum{NEW_GAME = 1, EXIT = 2};

	void CleanUp();
	void ResetMenu();

private:

	Input* _userInput;
	MenuImages* _background;
	MenuImages* _newGame;
	MenuImages* _newGameHigh;
	MenuImages* _exitGame;
	MenuImages* _exitGameHigh;

	int _menuItemSelected;
	int _state;
	int _count;

	bool IntializeDirectx(HWND hwnd, int width, int height, bool fullscreen);

	LPDIRECT3D9             direct3d;
	LPDIRECT3DDEVICE9       direct3dDevice;
};

