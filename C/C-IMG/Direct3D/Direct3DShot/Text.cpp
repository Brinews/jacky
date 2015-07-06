#include "Text.h"


Text::Text(void)
{
	_pFont = NULL;
	_fontDesc = D3DXFONT_DESC();
}


Text::~Text(void)
{
}


bool Text::Initialize(LPDIRECT3DDEVICE9 pDevice)
{

	D3DXCreateFontIndirect(pDevice,&_fontDesc,&_pFont);
	
	return true;
}

void Text::Print(char* text, int xPosition, int yPosition, DWORD color, LPD3DXSPRITE sprite,FONTALIGNMENT alignment)
{
	RECT poistion;

	poistion.top = xPosition;
	poistion.left = yPosition;
	poistion.right = xPosition + 190;
	poistion.bottom = yPosition + 190;


	_pFont->DrawText(sprite,text,-1,&poistion,alignment,color);
}