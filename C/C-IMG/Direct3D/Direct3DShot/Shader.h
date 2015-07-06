#pragma once
#include <string>
#include "d3dx9.h"

class Shader
{
protected:
	ID3DXBuffer* _pErrorBuffer;
	IDirect3DDevice9* _pd3dDevice;
	std::string _effectFileName;

public:
	ID3DXEffect* Effect;

	Shader(IDirect3DDevice9** device, std::string effectFileName) 
	{
		_pd3dDevice = *device;
		_effectFileName = effectFileName;
		Effect = NULL;
		_pErrorBuffer = NULL;
	}

	~Shader( )
	{
		CleanUp();
	}

	bool Initialise();  
	void CleanUp();
};
