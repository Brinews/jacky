#include "MenuSprites.h"


MenuSprites::MenuSprites(void)
{
	_sprite = NULL;
}


MenuSprites::~MenuSprites(void)
{
}


bool MenuSprites::loadSprite(LPDIRECT3DDEVICE9 device, std::string filename, HWND hwnd)
{
	HRESULT hResult;

	

	hResult = D3DXCreateSprite(device,&_sprite);

	if FAILED (hResult)
	{
		MessageBox(hwnd, "Create Sprite Failed", "Error", MB_OK);
		return false;
	}

	hResult = D3DXCreateTextureFromFileEx(device, filename.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE,D3DCOLOR_ARGB(255,255,255,255) , NULL, NULL, &_texture);
	
	if FAILED (hResult)
	{
		MessageBox(hwnd, "Create texture from file failed", "Error", MB_OK);
		return false;
	}

	D3DXIMAGE_INFO imageInfo;
	D3DXGetImageInfoFromFile(filename.c_str(), &imageInfo);

	_height = (float)imageInfo.Height;
	_width = (float)imageInfo.Width;

	return true;

}


void MenuSprites::render(LPDIRECT3DDEVICE9 pDevice, int alpha)
{
	alpha = (int)(255*((float)alpha/100));
	D3DXMATRIX scaleMatrix; 
	D3DXMATRIX transMatrix;
	D3DXMatrixScaling(&scaleMatrix, _scale.x, _scale.y, _scale.z);
	D3DXMatrixTranslation(&transMatrix, _position.x, _position.y, _position.z);

	D3DXMatrixMultiply(&transMatrix, &scaleMatrix, &transMatrix);
	_sprite->SetTransform(&transMatrix);

	_sprite->Begin(D3DXSPRITE_ALPHABLEND);
	_sprite->Draw(_texture, NULL, NULL, NULL, D3DCOLOR_RGBA(255,255,255,alpha));
	_sprite->End();

}

void MenuSprites::setPosition(int x, int y)
{
	_position.x = (float)x;
	_position.y = (float)y;
	_position.z = 0.0f;

}


void MenuSprites::setSize(int width, int height)
{
	_scale.x = (float)width/_width;
	_scale.y = (float)height/_height;
	_scale.z = 0;

}

int MenuSprites::getWidth()
{
	return (int)_width;
}

int MenuSprites::getHeight()
{

	return (int)_height;
}
