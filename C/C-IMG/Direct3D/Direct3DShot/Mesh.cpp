#include "Mesh.h"

Mesh::Mesh(IDirect3DDevice9* device, D3DXVECTOR3& position, std::string meshFileName)
{
	_position = position;
	_pd3dDevice = device;
	D3DXMatrixIdentity(&_worldMatrix);
	D3DXMatrixTranslation(&_worldMatrix, position.x, position.y, position.z);
	_meshFileName = meshFileName;
	_pMesh = NULL;
	_matBuffer = NULL;
	Texture = NULL;
	Visible = true;
}

Mesh::~Mesh()
{
}

D3DXVECTOR3 Mesh::GetPosition()
{
	return _position;
}

D3DXMATRIXA16* Mesh::GetWorldMat()
{
	return &_worldMatrix;
}

void Mesh::Translate(float inX, float inY, float inZ)
{
	_position.x = _position.x + inX;
	_position.y = _position.y + inY;
	_position.z = _position.z + inZ;

	D3DXMatrixIdentity(&_worldMatrix);
	D3DXMatrixTranslation(&_worldMatrix, _position.x, _position.y, _position.z);
}

bool Mesh::Load(std::string textureFileName)
{
	if( FAILED( D3DXLoadMeshFromX( _meshFileName.c_str(), D3DXMESH_MANAGED,	_pd3dDevice, NULL, &_matBuffer, NULL, &_numMaterials, &_pMesh)))
	{
		if( FAILED( D3DXLoadMeshFromX( _meshFileName.c_str(), D3DXMESH_MANAGED,	_pd3dDevice, NULL, &_matBuffer, NULL, &_numMaterials, &_pMesh)))
		{
			return false;
		}
	}

	if( FAILED(D3DXCreateTextureFromFile(_pd3dDevice, textureFileName.c_str(), &Texture)))
	{
		return false;
	}
	return true;
}

void Mesh::RenderAmbient(D3DXMATRIXA16* viewProjectionMatrix, ShadowEffect* shadowMapper)
{
	D3DXMATRIXA16 worldViewProjMat;
	D3DXMatrixMultiply(&worldViewProjMat, &_worldMatrix, viewProjectionMatrix);
	shadowMapper->Effect->SetMatrix(shadowMapper->WorldViewProjMatHandle, &worldViewProjMat);

	shadowMapper->Effect->BeginPass(0);
	_pMesh->DrawSubset(0);
	shadowMapper->Effect->EndPass();
}

void Mesh::RenderMeshWithShadowCube(D3DXMATRIXA16* viewProjectionMatrix, ShadowEffect* shadowMapper)
{
	D3DXMATRIXA16 worldViewProjMat;
	D3DXMatrixMultiply(&worldViewProjMat, &_worldMatrix, viewProjectionMatrix);

	shadowMapper->Effect->SetMatrix(shadowMapper->WorldViewProjMatHandle, &worldViewProjMat);
	shadowMapper->Effect->SetMatrix(shadowMapper->WorldMatrixHandle, &_worldMatrix);

	shadowMapper->Effect->BeginPass(0);
	_pMesh->DrawSubset(0);
	shadowMapper->Effect->EndPass();
}

void Mesh::CleanUp()
{
	if (Texture != NULL)
	{
		Texture->Release();
		Texture = NULL;
	}	
	if( _pMesh != NULL )
	{
		_pMesh->Release();
		_pMesh = NULL;
	}
}