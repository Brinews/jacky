#include "Environment.h"


/* 环境准备 */
Environment::Environment(Input* input)
{
	_pD3D = NULL;
	_pd3dDevice = NULL;
	_pBackBufferSurface = NULL;

	_pInput = NULL;  
	_pMainCamera = NULL;
	_pLight[0] = NULL;
	_pLight[1] = NULL;
	_pLight[2] = NULL;

	_pShadowEffect = NULL;

	_pTeapot = NULL;
	_pTeapot2 = NULL;
	_pTeapot3 = NULL;
	_pGround = NULL;
	_pBackWall = NULL;

	_lightMoveSpeed = 0.28f;
	_lightYMoveSpeed = 0.25f;
	_font = NULL;
	_fontDesc = D3DXFONT_DESC();

	_textFont = NULL;
	_score = NULL;
	_crosshair = 0;
	_crossHair = 0;

	_fontPosition.top = 180;
	_fontPosition.left = 180;
	_fontPosition.right = 200;
	_fontPosition.bottom = 200;
	_pInput = input;
	_reloaded = 100;
	_translatedLook = NULL;
	_translatedPos = NULL;
}

Environment::~Environment()
{
	CleanUp();
}

bool Environment::InitialiseDirectX( HWND hWnd, UINT screenWidth, UINT screenHeight, BOOL windowed )
{
	UINT AdapterToUse = D3DADAPTER_DEFAULT;
	D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL;

	if( NULL == ( _pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		return false;
	}
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.BackBufferWidth = screenWidth;
	d3dpp.BackBufferHeight = screenHeight;
	d3dpp.Windowed = windowed;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	int VP = 0;
	D3DCAPS9 caps;
	_pD3D->GetDeviceCaps(AdapterToUse, DeviceType, &caps);
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		VP = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		VP = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if (FAILED(_pD3D->CreateDevice( AdapterToUse, 
		DeviceType, 
		hWnd, 
		VP, 
		&d3dpp, 
		&_pd3dDevice) ) ) 
	{ 
		MessageBox(hWnd,"Failed To Create Device","BOOM!",MB_OK);
		return false;
	}
	_pd3dDevice->GetRenderTarget(0, &_pBackBufferSurface);

	return true;
}

bool Environment::Initialise( HWND hWnd, HINSTANCE instance, UINT screenWidth, UINT screenHeight, BOOL windowed )
{
	if( !(InitialiseDirectX(hWnd, screenWidth, screenHeight, windowed)) )
	{
		MessageBoxA(NULL, "Direct3d initialisation failed.", NULL, MB_OK);
		return false;
	}

	D3DXCreateFontIndirect(_pd3dDevice,&_fontDesc,&_font);

	_textFont = new Text;

	_textFont->Initialize(_pd3dDevice);

	_score = new Score;

	_crosshair = new MenuImages;
	_crosshair->LoadSurface(_pd3dDevice, "1_crosshair.jpg");
	_crosshair->setPosition(487,359);

	_crossHair = new MenuSprites;
	_crossHair->loadSprite(_pd3dDevice,"1_crosshair.png",hWnd);
	_crossHair->setSize(50,50);
	_crossHair->setPosition(487,359);

	D3DXVECTOR3 initialCamPos = D3DXVECTOR3(0.0f, 10.0f, -240.0f);

	for(int i = 0; i < 3; i++)
	{
		_pLight[i] = new Light(_pd3dDevice, &initialCamPos, (D3DX_PI / 2.0f), 1.0f, 1.0f, 500.0f);
	}

	_pShadowEffect = new ShadowEffect(&_pd3dDevice, "1_shadowEffect.fx");
	if( !(_pShadowEffect->SetUp()) )
	{
		MessageBoxA(NULL, "loading and initialising the shadowing effect failed", "BOOM!", MB_OK);
		return false;
	}

	_pMainCamera = new PlayerCamera(&initialCamPos, D3DX_PI / 2.0f, ((float)screenWidth / (float)screenHeight), 
									1.0f, 500.0f, 20.0f, _pInput);


	D3DXVECTOR3 teapotPos = D3DXVECTOR3(0.0f, 0.0f, 80.0f);
	D3DXVECTOR3 teapotPos2 = D3DXVECTOR3(90.0f, 0.0f, -160.0f);
	D3DXVECTOR3 teapotPos3 = D3DXVECTOR3(-110.0f, 0.0f, -60.0f);
	D3DXVECTOR3 cargoPos = D3DXVECTOR3(-110.0f, 0.0f, 10.0f);
	D3DXVECTOR3 spherePos = D3DXVECTOR3(-150.0f, 0.0f, -150.0f);
	D3DXVECTOR3 groundPos = D3DXVECTOR3(0.0f, 0.0f, -60.0f);
	D3DXVECTOR3 ceilingPos = D3DXVECTOR3(0.0f, -40.0f, -80.0f);
	D3DXVECTOR3 wallPos = D3DXVECTOR3(-4.0f, -7.0f, -230.0f);
	D3DXVECTOR3 backWallPos = D3DXVECTOR3(-20.0f, 0.0f, -275.0f);
	D3DXVECTOR3 palletPos = D3DXVECTOR3(-4.0f, 0.0f, -120.0f);
	D3DXVECTOR3 drumPos = D3DXVECTOR3(-40.0f, 0.0f, -140.0f);
	D3DXVECTOR3 shelvePos = D3DXVECTOR3(110.0f, 0.0f, -60.0f);

	D3DXVECTOR4 lightPos = D3DXVECTOR4(0.0f, 20.0f, 30.0f, 1.0f);
	D3DXVECTOR4 lightPos2 = D3DXVECTOR4(0.0f, 20.0f, -65.0f, 1.0f);
	D3DXVECTOR4 lightPos3 = D3DXVECTOR4(0.0f, 20.0f, -160.0f, 1.0f);

	_pTeapot = new Mesh(_pd3dDevice, teapotPos, "1_target.x");
	if( !(_pTeapot->Load("1_aim.bmp")) )
	{
		MessageBoxA(NULL, "loading teapot mesh failed.", "BOOM!", MB_OK);
		return false;
	}
	_pTeapot2 = new Mesh(_pd3dDevice, teapotPos2, "1_Boxes.x");
	if( !(_pTeapot2->Load("1_BoxColor.jpg")) )
	{
		MessageBoxA(NULL, "loading teapot2 mesh failed.", "BOOM!", MB_OK);
		return false;
	}
	_pTeapot3 = new Mesh(_pd3dDevice, teapotPos3, "1_BoxesR.x");
	if( !(_pTeapot3->Load("1_BoxColor.jpg")) )
	{
		MessageBoxA(NULL, "loading teapot3 mesh failed.", "BOOM!", MB_OK);
		return false;
	}
	
	_pGround = new Mesh(_pd3dDevice, groundPos, "1_Floor.x");
	if( !(_pGround->Load("1_Floor2.jpg")) )
	{
		MessageBoxA(NULL, "loading ground mesh failed.", "BOOM!", MB_OK);
		return false;
	}

	_pWall = new Mesh(_pd3dDevice, wallPos, "1_sandbags.x");
	if( !(_pWall->Load("1_sandbags_d.jpg")) )
	{
		MessageBoxA(NULL, "loading Wall mesh.", "BOOM!", MB_OK);

		return false;
	}

	_pBackWall = new Mesh(_pd3dDevice, backWallPos, "1_wall.x");
	if( !(_pBackWall->Load("1_wall.png")) )
	{
		MessageBoxA(NULL, "loading Wall mesh.", "BOOM!", MB_OK);

		return false;
	}


	_lightPosition[0] = lightPos;
	_lightPosition[1] = lightPos2;
	_lightPosition[2] = lightPos3;

	return true;
}

/* 飞盘移动 */
void Environment::OnFrameMove(DWORD inTimeDelta)
{
	_pInput->GetInputData();
	_pMainCamera->UpdateCamera((float)(inTimeDelta / 1000.0f));

	/* 碰到墙反向 */
	if (_pTeapot->GetPosition().x > 90 || _pTeapot->GetPosition().x < -90)
	{
		_lightMoveSpeed = -_lightMoveSpeed;
	}

	/* 碰到墙反向 */
	if (_pTeapot->GetPosition().y > 40 || _pTeapot->GetPosition().y < 0)
	{
		_lightYMoveSpeed = -_lightYMoveSpeed;
	}

	_pTeapot->Translate(_lightMoveSpeed,_lightYMoveSpeed,0);

	_pShadowEffect->Effect->SetVectorArray(_pShadowEffect->LightPositionHandle, _lightPosition, 3);

	for(int i = 0; i < 3; i++)
	{
		_pLight[i]->SetPosition(&D3DXVECTOR3(_lightPosition[i].x,_lightPosition[i].y,_lightPosition[i].z));
	}

	_pShadowEffect->Effect->SetVector(_pShadowEffect->EyePositionHandle, _pMainCamera->GetPosition4());
}

/* 3D曲面 */
void Environment::RenderDepthToCubeFace(Light* light, IDirect3DSurface9* cubeFaceSurface)
{
	D3DXMATRIXA16 worldViewProjectionMatrix;
	if(SUCCEEDED(_pd3dDevice->SetRenderTarget( 0, cubeFaceSurface )))
	{
		_pd3dDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, NULL);
	}
	else
	{
		return;
	}

	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pWall->GetWorldMat(), light->GetViewProjectionMatrix());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldMatrixHandle, _pWall->GetWorldMat());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldViewProjMatHandle, &worldViewProjectionMatrix);

	_pShadowEffect->Effect->BeginPass(0);
	_pWall->_pMesh->DrawSubset(0);
	_pShadowEffect->Effect->EndPass();

	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pGround->GetWorldMat(), light->GetViewProjectionMatrix());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldMatrixHandle, _pGround->GetWorldMat());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldViewProjMatHandle, &worldViewProjectionMatrix);

	_pShadowEffect->Effect->BeginPass(0);
	_pGround->_pMesh->DrawSubset(0);
	_pShadowEffect->Effect->EndPass();

	if(_pTeapot->Visible)
	{
		D3DXMatrixMultiply(&worldViewProjectionMatrix, _pTeapot->GetWorldMat(), light->GetViewProjectionMatrix());
		_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldMatrixHandle, _pTeapot->GetWorldMat());
		_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldViewProjMatHandle, &worldViewProjectionMatrix);

		_pShadowEffect->Effect->BeginPass(0);
		_pTeapot->_pMesh->DrawSubset(0);
		_pShadowEffect->Effect->EndPass();
	}

	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pTeapot2->GetWorldMat(), light->GetViewProjectionMatrix());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldMatrixHandle, _pTeapot2->GetWorldMat());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldViewProjMatHandle, &worldViewProjectionMatrix);

	_pShadowEffect->Effect->BeginPass(0);
	_pTeapot2->_pMesh->DrawSubset(0);
	_pShadowEffect->Effect->EndPass();
	
	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pTeapot3->GetWorldMat(), light->GetViewProjectionMatrix());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldMatrixHandle, _pTeapot3->GetWorldMat());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldViewProjMatHandle, &worldViewProjectionMatrix);

	_pShadowEffect->Effect->BeginPass(0);
	_pTeapot3->_pMesh->DrawSubset(0);
	_pShadowEffect->Effect->EndPass();

	D3DXMatrixMultiply(&worldViewProjectionMatrix, _pBackWall->GetWorldMat(), light->GetViewProjectionMatrix());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldMatrixHandle, _pBackWall->GetWorldMat());
	_pShadowEffect->Effect->SetMatrix(_pShadowEffect->WorldViewProjMatHandle, &worldViewProjectionMatrix);

	_pShadowEffect->Effect->BeginPass(0);
	_pBackWall->_pMesh->DrawSubset(0);
	_pShadowEffect->Effect->EndPass();

}

/* 立体阴影 */
void Environment::FillCubicShadowMap(Light* light)
{
	UINT numOfPasses;
	if( FAILED(_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE , D3DCOLORWRITEENABLE_RED )) )
	{
		return;
	}
	_pShadowEffect->Effect->SetVector(_pShadowEffect->ShadowPositionHandle, light->GetPosition4());
	_pShadowEffect->Effect->SetTechnique(_pShadowEffect->DepthMapHandle);
	_pShadowEffect->Effect->Begin(&numOfPasses, NULL);

	light->SetCameraToPositiveX();
	RenderDepthToCubeFace(light, light->_depthCubeFacePX);
	light->SetCameraToPositiveY();
	RenderDepthToCubeFace(light, light->_depthCubeFacePY);
	light->SetCameraToPositiveZ();
	RenderDepthToCubeFace(light, light->_depthCubeFacePZ);
	light->SetCameraToNegativeX();
	RenderDepthToCubeFace(light, light->_depthCubeFaceNX);
	light->SetCameraToNegativeY();
	RenderDepthToCubeFace(light, light->_depthCubeFaceNY);
	light->SetCameraToNegativeZ();
	RenderDepthToCubeFace(light, light->_depthCubeFaceNZ);

	_pShadowEffect->Effect->End();

	if( FAILED(_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_ALPHA | 
		D3DCOLORWRITEENABLE_RED | 
		D3DCOLORWRITEENABLE_GREEN | 
		D3DCOLORWRITEENABLE_BLUE)) )
	{
		return;
	}
}

/* 阴影绘制 */
void Environment::RenderSceneWithShadowMap()
{
	UINT numOfPasses;

	if( SUCCEEDED(_pd3dDevice->SetRenderTarget(0, _pBackBufferSurface)) )
	{
		_pd3dDevice->Clear(NULL, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, NULL);
	}
	_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);   


	_pShadowEffect->Effect->SetTexture(_pShadowEffect->CubeShadowMapHandle, _pLight[0]->CubicShadowMap);
	_pShadowEffect->Effect->SetTexture(_pShadowEffect->CubeShadowMap2Handle, _pLight[1]->CubicShadowMap);
	_pShadowEffect->Effect->SetTexture(_pShadowEffect->CubeShadowMap3Handle, _pLight[2]->CubicShadowMap);
	_pShadowEffect->Effect->SetTechnique(_pShadowEffect->CubicShadowMappingHandle);

	_pShadowEffect->Effect->Begin(&numOfPasses, NULL);


	if(_pTeapot->timeSinceShot-- == 0)
	{
		_pTeapot->Visible = true;
	}

	if(_pTeapot->Visible)
	{
		_pShadowEffect->Effect->SetTexture(_pShadowEffect->MaterialTexture, _pTeapot->Texture);
		_pTeapot->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);
	}

	_pShadowEffect->Effect->SetTexture(_pShadowEffect->MaterialTexture, _pTeapot2->Texture);
	_pTeapot2->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);

	_pShadowEffect->Effect->SetTexture(_pShadowEffect->MaterialTexture, _pTeapot3->Texture);
	_pTeapot3->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);

	_pShadowEffect->Effect->SetTexture(_pShadowEffect->MaterialTexture, _pGround->Texture);
	_pGround->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);

	_pShadowEffect->Effect->SetTexture(_pShadowEffect->MaterialTexture, _pWall->Texture);
	_pWall->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);

	_pShadowEffect->Effect->SetTexture(_pShadowEffect->MaterialTexture, _pBackWall->Texture);
	_pBackWall->RenderMeshWithShadowCube(_pMainCamera->GetViewProjectionMatrix(), _pShadowEffect);

	_pShadowEffect->Effect->End();

	_pShadowEffect->Effect->SetTechnique(_pShadowEffect->AmbientHandle);
}

void Environment::Update()
{
	if (_reloaded > 0)
	{
		_reloaded--;
	}

	/* 判断射击动作 */
	if(_pInput->IsLeftMouseButtonPressed() && _reloaded == 0)
	{
		_reloaded = 10; /* 子弹间隔时间10秒 */

		PlaySound("1_cannon.wav", NULL, SND_ASYNC);
		BOOL hit = false;

		_translatedLook = _pMainCamera->GetLook();
		_translatedPos = new D3DXVECTOR3(_pMainCamera->GetPosition()->x, 
			_pMainCamera->GetPosition()->y, _pMainCamera->GetPosition()->z);
		D3DXMATRIX inverseWorld;

		if(_pTeapot->Visible)
		{
			D3DXMATRIXA16* world = _pTeapot->GetWorldMat();
			D3DXMatrixInverse(&inverseWorld, 0, world);
			D3DXVec3TransformNormal(_translatedLook, _translatedLook, &inverseWorld);
			D3DXVec3TransformCoord(_translatedPos, _translatedPos, &inverseWorld);

			/* 击中目标判断 */
			D3DXIntersect(_pTeapot->_pMesh, _translatedPos, _translatedLook, &hit, 
				NULL, NULL, NULL,NULL,NULL,NULL);

			if(hit == 1)
			{
				PlaySound("1_hit.wav", NULL, SND_ASYNC);
				_score->IncrimentScore();
				_pTeapot->Visible = false;
				_pTeapot->timeSinceShot = 20; /* 飞盘间隔时间 */
			}
		}
	}
}

/* 画面更新 */
void Environment::Render(DWORD inTimeDelta, std::string fps,std::string countDown)
{
	OnFrameMove(inTimeDelta);
	
	if( SUCCEEDED(_pd3dDevice->BeginScene()) )
	{
		for (int i = 0; i < 3; i++)
		{
			FillCubicShadowMap(_pLight[i]);
		}
		RenderSceneWithShadowMap();
		char* text = strdup(fps.c_str());
		char* text2 = strdup(countDown.c_str());

		_textFont->Print(text,20,20,0xffffffff,NULL);

		_textFont->Print("High Score: ",40,20,0xffffffff,NULL);
		
		_textFont->Print(_score->GetScore(),40,100,0xffffffff,NULL); 

		_textFont->Print(text2,60,20,0xffffffff,NULL); 

		free(text);
		free(text2);

		//_crosshair->render(_pd3dDevice);
		_crossHair->render(_pd3dDevice,200);
	}
	_pd3dDevice->EndScene();

	_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

char* Environment::getScore()
{
	return _score->GetScore();
}

/* 清理 */
void Environment::CleanUp()
{
	if( _pD3D != NULL )
	{
		_pD3D->Release();
		_pD3D = NULL;
	}
	if( _pd3dDevice != NULL )
	{
		_pd3dDevice->Release();
		_pd3dDevice = NULL;
	}
	if( _pBackBufferSurface != NULL )
	{
		_pBackBufferSurface->Release();
		_pBackBufferSurface = NULL;
	}
	if( _pInput != NULL )
	{
		_pInput->CleanUp();
		delete _pInput;
		_pInput = NULL;
	}
	if( _pMainCamera != NULL )
	{
		delete _pMainCamera;
		_pMainCamera = NULL;
	}

	if( _pLight[0] != NULL )
	{
		delete _pLight[0];
		_pLight[0] = NULL;
	}

	if( _pLight[1] != NULL )
	{
		delete _pLight[1];
		_pLight[1] = NULL;
	}
	if( _pLight[2] != NULL )
	{
		delete _pLight[2];
		_pLight[2] = NULL;
	}

	if( _pShadowEffect != NULL )
	{
		_pShadowEffect->CleanUp();
		delete _pShadowEffect;
		_pShadowEffect = NULL;
	}
	if( _pTeapot != NULL )
	{
		_pTeapot->CleanUp();
		delete _pTeapot;
		_pTeapot = NULL;
	}
	if( _pTeapot2 != NULL )
	{
		_pTeapot2->CleanUp();
		delete _pTeapot2;
		_pTeapot2 = NULL;
	}
	if( _pTeapot3 != NULL )
	{
		_pTeapot3->CleanUp();
		delete _pTeapot3;
		_pTeapot3 = NULL;
	}

	if( _pGround != NULL )
	{
		_pGround->CleanUp();
		delete _pGround;
		_pGround = NULL;
	}
	if( _pWall != NULL )
	{
		_pWall->CleanUp();
		delete _pWall;
		_pWall = NULL;
	}

	if(_font != NULL)
	{
		_font->Release();
		_font = NULL;
	}
	if(_translatedPos != NULL)
	{
		delete _translatedPos;
		_translatedPos = NULL;
	}
	if(_translatedLook != NULL)
	{
		delete _translatedLook;
		_translatedLook = NULL;
	}

	if(_textFont != NULL)
	{
		delete _textFont;
		_textFont = NULL;
	}
}