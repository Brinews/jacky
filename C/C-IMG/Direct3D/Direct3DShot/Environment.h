 #pragma once
#include "Input.h"
#include "PlayerCamera.h"
#include "Mesh.h"
#include "Light.h"
#include "Text.h"
#include "Score.h"
#include "MenuImages.h"
#include "MenuSprites.h"

class Environment
{
	IDirect3D9* _pD3D;
	IDirect3DDevice9* _pd3dDevice;
	IDirect3DSurface9* _pBackBufferSurface;

	Input* _pInput;
	PlayerCamera* _pMainCamera;
	Light* _pLight[3];
	ShadowEffect* _pShadowEffect;
	D3DXVECTOR4 _lightPosition[3];

	Mesh* _pTeapot;
	Mesh* _pTeapot2;
	Mesh* _pTeapot3;
	//Mesh* _pSphere;
	Mesh* _pGround;
	Mesh* _pCeiling;
	Mesh* _pWall;
	//Mesh* _pShelve;
	//Mesh* _pCargo;
	//Mesh* _pPallets;
	Mesh* _pBackWall;
	//Mesh* _pDrums;

	D3DXVECTOR3* _translatedLook;
	D3DXVECTOR3* _translatedPos;

	LPD3DXFONT _font; 
	D3DXFONT_DESC _fontDesc;
	RECT _fontPosition;

	Text* _textFont;
	Score* _score;

	MenuImages* _crosshair;
	MenuSprites* _crossHair;

	int _reloaded;
	float _lightMoveSpeed;
	float _lightYMoveSpeed;
	bool InitialiseDirectX(HWND hWnd, UINT screenWidth, UINT screenHeight, BOOL windowed);
	
	void OnFrameMove(DWORD timeDelta);

	void RenderDepthToCubeFace(Light* light, IDirect3DSurface9* cubeFaceSurface);
	void FillCubicShadowMap(Light* light);
	void RenderSceneWithShadowMap();


public:
	Environment(Input* input);
	~Environment();

	bool Initialise(HWND hWnd, HINSTANCE instance, UINT screenWidth, UINT screenHeight, BOOL windowed);
	void Update();
	void Render(DWORD timeDelta, std::string fps,std::string countdown);
	void CleanUp();
	char* getScore();

};
