#include "EndMenu.h"


EndMenu::EndMenu(void)
{

	_userInput = 0;
	_background = 0;
	_mainMenu = 0;
	_mainMenuHigh = 0;
	_exitGame = 0;
	_exitGameHigh = 0;
	_count = 0;
}


EndMenu::~EndMenu(void)
{
	CleanUp();
}

bool EndMenu::Intialize(Input* input,HWND hwnd, int width, int height)
{
	if( !(IntializeDirectx(hwnd,width,height,false)))
	{
		MessageBoxA(NULL, "of EndMenu Direct3d initialisation failed.", NULL, MB_OK);
		return false;
	}

	_userInput = input;

	_menuItemSelected = 0;
	_state = 0;


	_background = new MenuImages;
	_background->LoadSurface(_direct3dDevice, "background.jpg");

	_mainMenu = new MenuImages;
	_mainMenu->LoadSurface(_direct3dDevice, "background.jpg");
	_mainMenu->setPosition(400,400);

	_mainMenuHigh = new MenuImages;
	_mainMenuHigh->LoadSurface(_direct3dDevice, "background.jpg");
	_mainMenuHigh->setPosition(400,400);


	_exitGame = new MenuImages;
	_exitGame->LoadSurface(_direct3dDevice, "newgame.jpg");
	_exitGame->setPosition(200,400);
	_exitGameHigh = new MenuImages;
	_exitGameHigh->LoadSurface(_direct3dDevice, "newgameh.jpg");
	_exitGameHigh->setPosition(200,400);

	return true;
}

bool EndMenu::IntializeDirectx(HWND hwnd, int width, int height, bool fullscreen)
{
	HRESULT hr;
	_direct3d = Direct3DCreate9( D3D_SDK_VERSION );
	if(_direct3d == NULL)
	{
		return false;
	}

	bool windowed = true;
	if (fullscreen){
		windowed = false;
	}

	//Setup the present parameters
	D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = windowed; //Windowed or Fullscreen
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //discards the previous frames
	d3dpp.BackBufferFormat = D3DFMT_R5G6B5; //The display format
	d3dpp.BackBufferCount  = 1;	//Number of back buffers
	d3dpp.BackBufferHeight = height; //height of the backbuffer
	d3dpp.BackBufferWidth  = width; //width of the backbuffer
	d3dpp.hDeviceWindow    = hwnd; //handle to our window
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; //The stencil format
    d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;


	//Create the Video Device
	hr = _direct3d->CreateDevice( D3DADAPTER_DEFAULT, //The default adapter is the primary display adapter
							D3DDEVTYPE_HAL, //the HAL (hardware accelerated layer) uses your 3d accelerator card
							hwnd,
                            D3DCREATE_HARDWARE_VERTEXPROCESSING, //sets the graphic card to do the hardware vertexprocessing
                            &d3dpp, //The present parameters we created above
							&_direct3dDevice 
							);
	if( FAILED(hr)){
        return false;
    }
	_direct3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	_direct3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	_direct3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE); //this normalizes the normal values (this is important for how lighting effects your models)
	return true;
}

void EndMenu::update()
{
	_userInput->GetInputData();

	if(_count == 0)
	{
		if (_userInput->IsWPressed() || _userInput->IsSPressed())
		{
			if (_menuItemSelected == 0)
			{
				_menuItemSelected = 1;
			} 
			else 
			{
				_menuItemSelected = 0;
			}
		}
		_count = 100;
	}

	if (_userInput->IsEnterPressed())
	{
		switch (_menuItemSelected)
		{
			case 0: {_state = MAIN_MENU; break;}
			case 1: {_state = EXIT; break;}
		}	

	}

	if( _count > 0)
	{
		_count--;
	}


}


void EndMenu::render()
{
	if ( NULL == _direct3dDevice)
		return;

	_direct3dDevice->Clear( 0, 
						NULL, 
						D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
						D3DCOLOR_XRGB(55,55,55), 
						1.0f, 
						0 
						);

	_direct3dDevice->BeginScene();

	_background->render(_direct3dDevice);

	if (_menuItemSelected == 0){
		_mainMenuHigh->render(_direct3dDevice);
		_exitGame->render(_direct3dDevice);
	} else {
		_mainMenu->render(_direct3dDevice);
		_exitGameHigh->render(_direct3dDevice);
	}

	_direct3dDevice->EndScene();
    _direct3dDevice->Present( NULL, NULL, NULL, NULL );

}

int EndMenu::getState()
{

	return _state;

}

void EndMenu::ResetMenu()
{
	_state = 0;
}


void EndMenu::CleanUp()
{
	if( _direct3dDevice != NULL) 
	{
        _direct3dDevice->Release();
		_direct3dDevice = NULL;
	}
    if( _direct3d != NULL)
	{
        _direct3d->Release();
		_direct3d = NULL;
	}

	if( _userInput != NULL )
	{
		_userInput->CleanUp();
		delete _userInput;
		_userInput = NULL;
	}

	if (_background != NULL)
	{
		delete _background;
	}

	if (_mainMenu != NULL)
	{
		delete _mainMenu;
	}

	if (_mainMenuHigh != NULL)
	{
		delete _mainMenuHigh;
	}

	if (_exitGame != NULL)
	{
		delete _exitGame;
	}
	if (_exitGameHigh != NULL)
	{
		delete _exitGameHigh;
	}

}