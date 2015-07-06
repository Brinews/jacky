#include "MenuImages.h"


MenuImages::MenuImages(void)
{
	_surface = 0;
}


MenuImages::~MenuImages(void)
{
	if( _surface != NULL) 
	{
        _surface->Release();
		_surface = NULL;
	}
}


bool MenuImages::LoadSurface(LPDIRECT3DDEVICE9 device, std::string filename)
{
	_imageScale = 100;

	HRESULT hResult;
	// Get the width and height of the image
	D3DXIMAGE_INFO imageInfo;
	hResult = D3DXGetImageInfoFromFile(filename.c_str(), &imageInfo);

	if FAILED (hResult)
	{
		return false;
	}

	_height = imageInfo.Height;
	_width = imageInfo.Width;

	hResult = device->CreateOffscreenPlainSurface(_width, //surface width
		_height, 
		D3DFMT_A8R8G8B8, 
		D3DPOOL_DEFAULT, 
		&_surface, 
		NULL
		);

	if (FAILED(hResult))
	{
		return false;
	}


	hResult = D3DXLoadSurfaceFromFile(_surface, NULL, NULL, filename.c_str(), NULL, D3DX_DEFAULT,0, NULL);

	if (FAILED(hResult))
	{
		return false;
	}

	_destRect.left = 0;
	_destRect.top = 0;
	_destRect.bottom = _destRect.top + _height;
	_destRect.right = _destRect.left + _width;

	_srcRect.left = 0;
	_srcRect.top = 0;
	_srcRect.bottom = _destRect.top + _height;
	_srcRect.right = _destRect.left + _width;

	return true;
}


void MenuImages::render(LPDIRECT3DDEVICE9 pDevice)
{
	_destRect.bottom = _destRect.top + (int)(_height * (_imageScale / 100));
	_destRect.right = _destRect.left + (int)(_width * (_imageScale / 100));

	IDirect3DSurface9* backbuffer = NULL;
	pDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	
	pDevice->StretchRect(_surface, &_srcRect, backbuffer, &_destRect, D3DTEXF_NONE);

}


void MenuImages::setPosition(int x, int y)
{
	_destRect.left = x;
	_destRect.top = y;
	_destRect.bottom = _destRect.top + _height;
	_destRect.right = _destRect.left + _width;


}


void MenuImages::setSize(int percent)
{
	_imageScale = percent;

}


void MenuImages::setSrcRect(int left, int top, int width, int height)
{
	_srcRect.left = left;
	_srcRect.top = top;
	_srcRect.bottom = _srcRect.top + height;
	_srcRect.right = _srcRect.left + width;
}

void MenuImages::setDestRect(int left, int top, int width, int height)
{
	_destRect.left = left;
	_destRect.top = top;
	_destRect.bottom = _destRect.top + height;
	_destRect.right = _destRect.left + width;
}