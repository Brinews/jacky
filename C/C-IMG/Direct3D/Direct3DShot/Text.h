#pragma once

#ifndef _TEXT_H_
#define _TEXT_H_

#include "d3dx9.h"

enum FONTALIGNMENT { FA_LEFT, FA_CENTER, FA_RIGHT, FA_TOPRIGHT, FA_TOPLEFT, FA_BOTTOMRIGHT, FA_BOTTOMLEFT };

class Text
{
public:
	Text(void);
	~Text(void);

	bool Initialize(LPDIRECT3DDEVICE9 pDevice);
	void Print( char* text, int xPosition, int yPosition, DWORD color, LPD3DXSPRITE sprite,FONTALIGNMENT alignment = FA_LEFT  ); 

	void Release();


private:

	LPD3DXFONT _pFont; 
	D3DXFONT_DESC _fontDesc;
};

#endif