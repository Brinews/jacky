#include "EndHighscores.h"


EndHighscores::EndHighscores()
{

	_userInput = 0;
	_background = 0;
	_mainMenu = 0;
	_mainMenuHigh = 0;
	_exitGame = 0;
	_exitGameHigh = 0;
	_count = 0;
	_textFont = NULL;
}


EndHighscores::~EndHighscores()
{
	CleanUp();
}

bool EndHighscores::Intialize(Input* input,HWND hwnd, int width, int height)
{
	if( !(IntializeDirectx(hwnd,width,height,false)))
	{
		MessageBoxA(NULL, "of EndHighscores Direct3d initialisation failed.", NULL, MB_OK);
		return false;
	}

	_userInput = input;

	_menuItemSelected = 0;
	_state = 0;

	_textFont = new Text;

	_textFont->Initialize(_direct3dDevice);

	_background = new MenuImages;
	_background->LoadSurface(_direct3dDevice, "1_startground.jpg");

	_mainMenu = new MenuImages;
	_mainMenu->LoadSurface(_direct3dDevice, "1_MainMenu.jpg");
	_mainMenu->setPosition(200,300);

	_mainMenuHigh = new MenuImages;
	_mainMenuHigh->LoadSurface(_direct3dDevice, "1_MainMenuHigh.jpg");
	_mainMenuHigh->setPosition(200,300);


	_exitGame = new MenuImages;
	_exitGame->LoadSurface(_direct3dDevice, "1_new.jpg");
	_exitGame->setPosition(200,400);
	_exitGameHigh = new MenuImages;
	_exitGameHigh->LoadSurface(_direct3dDevice, "1_newh.jpg");
	_exitGameHigh->setPosition(200,400);

	return true;
}

bool EndHighscores::IntializeDirectx(HWND hwnd, int width, int height, bool fullscreen)
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

void EndHighscores::update()
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
		_count = 10;
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


void EndHighscores::render(char* score)
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

	_textFont->Print(score,40,100,0xffffffff,NULL);

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

int EndHighscores::getState()
{

	return _state;

}

void EndHighscores::ResetMenu()
{
	_state = 0;
}


void EndHighscores::CleanUp()
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