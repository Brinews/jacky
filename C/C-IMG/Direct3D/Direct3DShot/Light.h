#pragma once
#include "Camera.h"

class Light : public Camera
{
public:
	Light(IDirect3DDevice9* d3device, D3DXVECTOR3* position3, float fov, float aspect, float nearZ, float farZ): Camera(position3, fov, aspect, nearZ, farZ)
	{	
		CubicShadowMap = NULL;
		_depthCubeFacePX = NULL;
		_depthCubeFacePY = NULL;
		_depthCubeFacePZ = NULL;
		_depthCubeFaceNX = NULL;
		_depthCubeFaceNY = NULL;
		_depthCubeFaceNZ = NULL;

		if( FAILED(D3DXCreateCubeTexture(d3device, 720, 1, D3DUSAGE_RENDERTARGET, 
			D3DFMT_R32F, D3DPOOL_DEFAULT,&CubicShadowMap)) )
		{
			if( FAILED(D3DXCreateCubeTexture(d3device, 720, 1, D3DUSAGE_RENDERTARGET, 
				D3DFMT_R16F, D3DPOOL_DEFAULT, &CubicShadowMap)) )
			{
				MessageBoxA(NULL, "Creating cubic shadow map failed.", "BOOM!", MB_OK);
			}

		}

		if( FAILED(CubicShadowMap->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &_depthCubeFacePX)) )
		{
			MessageBoxA(NULL, "Creating cubic shadow map 1 failed.", "BOOM!", MB_OK);
		}
		if( FAILED(CubicShadowMap->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y, 0, &_depthCubeFacePY)) )
		{
			MessageBoxA(NULL, "Creating cubic shadow map 2 failed.", "BOOM!", MB_OK);
		}
		if( FAILED(CubicShadowMap->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z, 0, &_depthCubeFacePZ)) )
		{
			MessageBoxA(NULL, "Creating cubic shadow map 3 failed.", "BOOM!", MB_OK);
		}
		if( FAILED(CubicShadowMap->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X, 0, &_depthCubeFaceNX)) )
		{
			MessageBoxA(NULL, "Creating cubic shadow map 4 failed.", "BOOM!", MB_OK);
		}
		if( FAILED(CubicShadowMap->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &_depthCubeFaceNY)) )
		{
			MessageBoxA(NULL, "Creating cubic shadow map 5 failed.", "BOOM!", MB_OK);
		}
		if( FAILED(CubicShadowMap->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &_depthCubeFaceNZ)) )
		{
			MessageBoxA(NULL, "Creating cubic shadow map 6 failed.", "BOOM!", MB_OK);
		}
	}

	IDirect3DCubeTexture9* CubicShadowMap;
	IDirect3DSurface9* _depthCubeFacePX;
	IDirect3DSurface9* _depthCubeFacePY;
	IDirect3DSurface9* _depthCubeFacePZ;
	IDirect3DSurface9* _depthCubeFaceNX;
	IDirect3DSurface9* _depthCubeFaceNY;
	IDirect3DSurface9* _depthCubeFaceNZ;

	void SetCameraToPositiveX();
	void SetCameraToPositiveY();
	void SetCameraToPositiveZ();
	void SetCameraToNegativeX();
	void SetCameraToNegativeY();
	void SetCameraToNegativeZ();

	~Light()
	{
		CleanUp();
	};

private:
	void CleanUp();
};