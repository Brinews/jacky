#include "Light.h"

void Light::SetCameraToPositiveX()
{
	SetLook(&_positiveLookX);
	SetUp(&_positiveLookY);
	SetRight(&_negativeLookZ);
	UpdateViewProjectionMatrix();
}

void Light::SetCameraToPositiveY()
{
	SetLook(&_positiveLookY);
	SetUp(&_negativeLookZ);
	SetRight(&_positiveLookX);
	UpdateViewProjectionMatrix();
}

void Light::SetCameraToPositiveZ()
{
	SetLook(&_positiveLookZ);
	SetUp(&_positiveLookY);
	SetRight(&_positiveLookX);
	UpdateViewProjectionMatrix();
}

void Light::SetCameraToNegativeX()
{
	SetLook(&_negativeLookX);
	SetUp(&_positiveLookY);
	SetRight(&_positiveLookZ);
	UpdateViewProjectionMatrix();
}

void Light::SetCameraToNegativeY()
{
	SetLook(&_negativeLookY);
	SetUp(&_negativeLookZ);
	SetRight(&_negativeLookX);
	UpdateViewProjectionMatrix();
}

void Light::SetCameraToNegativeZ()
{
	SetLook(&_negativeLookZ);
	SetUp(&_positiveLookY);
	SetRight(&_negativeLookX);
	UpdateViewProjectionMatrix();
}

void Light::CleanUp()
{
	if( CubicShadowMap != NULL )
	{
		CubicShadowMap->Release();
		CubicShadowMap = NULL;
	}
	if( _depthCubeFacePX != NULL )
	{
		_depthCubeFacePX->Release();
		_depthCubeFacePX = NULL;
	}
	if( _depthCubeFacePY != NULL )
	{
		_depthCubeFacePY->Release();
		_depthCubeFacePY = NULL;
	}
	if( _depthCubeFacePZ != NULL )
	{
		_depthCubeFacePZ->Release();
		_depthCubeFacePZ = NULL;
	}
	if( _depthCubeFaceNX != NULL )
	{
		_depthCubeFaceNX->Release();
		_depthCubeFaceNX = NULL;
	}
	if( _depthCubeFaceNY != NULL )
	{
		_depthCubeFaceNY->Release();
		_depthCubeFaceNY = NULL;
	}
	if( _depthCubeFaceNZ != NULL )
	{
		_depthCubeFaceNZ->Release();
		_depthCubeFaceNZ = NULL;
	}
}