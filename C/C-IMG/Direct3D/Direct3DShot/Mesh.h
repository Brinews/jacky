#pragma once
#include "d3dx9.h"
#include <string>
#include "ShadowEffect.h"

class Mesh
{
	IDirect3DDevice9* _pd3dDevice;
	D3DXMATRIXA16 _worldMatrix;
	DWORD _numMaterials;
	std::string _meshFileName;
	ID3DXBuffer* _matBuffer;
	D3DXVECTOR3 _position;
public:
	ID3DXMesh* _pMesh;
	IDirect3DTexture9* Texture;
	bool Visible;
	int timeSinceShot;

	D3DXVECTOR3 GetPosition();

	Mesh(IDirect3DDevice9* device, D3DXVECTOR3& position, std::string meshFileName);
	~Mesh();

	D3DXMATRIXA16* GetWorldMat();
	void Translate(float inX, float inY, float inZ);

	bool Load(std::string textureFileName);
	void RenderAmbient(D3DXMATRIXA16* inViewProjMat, 
					   ShadowEffect* inShadowMapper);
	void RenderMeshWithShadowCube(D3DXMATRIXA16* inViewProjMat, 
								  ShadowEffect* inShadowMapper);
	void CleanUp();
};
