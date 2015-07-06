#pragma once
#include <Windows.h>
#include <dinput.h>

#define KEYDOWN(name, key) (name[key] & 0x80)

class Input
{
	IDirectInput8* _directInput;
	IDirectInputDevice8* _keyboard;
	IDirectInputDevice8* _mouse;

	HRESULT _hr;

public:

	char _keyBuffer[256];
	DIMOUSESTATE _mouseState;

	Input()
	{
		_directInput = NULL;
		_keyboard = NULL;
		_mouse = NULL;
	}

	~Input()
	{
	}

	bool Initialise(HWND hwnd, HINSTANCE hInst);
	void GetInputData();
	void GetKeyboardState();

	int IsWPressed();
	int IsSPressed();
	int IsAPressed();
	int IsDPressed();
	int IsEscapePressed();
	int IsEnterPressed();

	bool IsLeftMouseButtonPressed();

	void CleanUp();
};
