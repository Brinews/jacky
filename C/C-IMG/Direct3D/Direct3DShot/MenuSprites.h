#pragma once
#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>



class MenuSprites
{

	LPD3DXSPRITE _sprite;
	LPDIRECT3DTEXTURE9 _texture;
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _scale;

	float _width;
	float _height;

public:
	MenuSprites();
	~MenuSprites();

	bool loadSprite(LPDIRECT3DDEVICE9 device, std::string filename, HWND hwnd);

	void render(LPDIRECT3DDEVICE9 pDevice, int alpha);
	void setPosition(int x, int y);
	void setSize(int Width, int Height);


	int getHeight();
	int getWidth();
};

