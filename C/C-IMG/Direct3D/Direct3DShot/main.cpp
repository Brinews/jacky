#include <Windows.h>
#include <mmsystem.h>
#include ".\Environment.h"
#include ".\timeManager.h"
#include ".\GameManager.h"

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{

	case WM_DESTROY:

		PostQuitMessage( 0 );
		return 0;

	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	WNDCLASSEX wc = 
		{
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			MsgProc,
			0L,
			0L,
			GetModuleHandle(NULL),
			NULL,
			NULL,
			NULL,
			NULL,
			"Direct3D Game",
			NULL 
		};
	RegisterClassEx( &wc );

	int windowWidth = 1024;
	int windowHeight = 768;
	int posX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth)  / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;
	HWND hWnd = CreateWindow("Direct3D Game",
							 "Direct3D Game",
							 WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION| WS_SIZEBOX,
							 posX,
							 posY,
							 windowWidth,
							 windowHeight,
							 GetDesktopWindow(),
							 NULL,
							 wc.hInstance,
							 NULL);

	Input* pInput = new Input();
	if( !(pInput->Initialise(hWnd, wc.hInstance)) )
	{
		MessageBoxA(NULL, "Direct Input initialisation failed.", "BOOM!", MB_OK);
		return false;
	}

	/*Environment game = Environment(pInput);

	if( !(game.Initialise(hWnd, hInst, windowWidth, windowHeight, true)) )
	{
		MessageBoxA(NULL, "Failed to initialise the game.", NULL, MB_OK);
		return 0;
	}*/

	GameManager gameManager = GameManager(pInput);

	if( !(gameManager.Initialise(hWnd, hInst, windowWidth, windowHeight, true)) )
	{
		MessageBoxA(NULL, "Failed to initialise the game.", NULL, MB_OK);
		return 0;
	}

	TimeManager timeManager;

	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			timeManager.UpdateLastTime();
			gameManager.update(timeManager.GetTimeDelta(), timeManager.GetFPS(timeManager.GetTimeDelta()),timeManager.GetCurrentT());
			timeManager.UpdateCurrentTime();
			timeManager.UpdateTimeDelta();
		}

		pInput->GetKeyboardState();
		if(pInput->IsEscapePressed())
		{
			msg.message = WM_QUIT;
		}
	}

	UnregisterClass( "Direct3D Game", wc.hInstance );

	return 0;
}