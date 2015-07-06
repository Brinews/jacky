#pragma once
#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>


class MenuImages
{
public:
	MenuImages(void);
	~MenuImages(void);

	bool LoadSurface(LPDIRECT3DDEVICE9 device, std::string filename);

	void render(LPDIRECT3DDEVICE9 pDevice);
	void setPosition(int x, int y);
	void setSize(int percent);

	void setSrcRect(int left, int top, int height, int width);
	void setDestRect(int left, int top, int height, int width);

private:

	IDirect3DSurface9* _surface;

	RECT _srcRect; 
	RECT _destRect;
	int _width;
	int _height;
	int _imageScale;
};

