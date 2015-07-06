#include "ShadowEffect.h"

void ShadowEffect::GetHandles()
{
	DepthMapHandle = Effect->GetTechniqueByName("depthMap");
	CubicShadowMappingHandle = Effect->GetTechniqueByName("cubicShadowMapping");
	AmbientHandle = Effect->GetTechniqueByName("ambient");
	WorldMatrixHandle = Effect->GetParameterByName(NULL, "worldMat");
	WorldViewProjMatHandle = Effect->GetParameterByName(NULL, "worldViewProjMat");
	CubeShadowMapHandle = Effect->GetParameterByName(NULL, "cubeShadowMap");
	CubeShadowMap2Handle = Effect->GetParameterByName(NULL, "cubeShadowMap2");
	CubeShadowMap3Handle = Effect->GetParameterByName(NULL, "cubeShadowMap3");
	EyePositionHandle = Effect->GetParameterByName(NULL, "eyePosition");
	LightPositionHandle = Effect->GetParameterByName(NULL, "lightPosition");
	LightNumberHandle = Effect->GetParameterByName(NULL, "lightNumber");
	ShadowPositionHandle = Effect->GetParameterByName(NULL, "shadowLightPosition");
	MaterialTexture = Effect->GetParameterByName(NULL, "materialTexture");
}

bool ShadowEffect::ValidateTechniques()
{
	if(FAILED(Effect->ValidateTechnique(DepthMapHandle)))
	{
		MessageBoxA(NULL, "depth map technique validation failed.", NULL, MB_OK);
		return false;
	}
	if(FAILED(Effect->ValidateTechnique(CubicShadowMappingHandle)))
	{
		MessageBoxA(NULL, "cubic shadow mapping technique validation failed.", NULL, MB_OK);
		return false;
	}
	if(FAILED(Effect->ValidateTechnique(AmbientHandle)))
	{
		MessageBoxA(NULL, "ambient technique validation failed.", NULL, MB_OK);
		return false;
	}

	return true;
}

bool ShadowEffect::SetUp()
{
	if(!(Initialise()) )
	{
		return false;
	}

	GetHandles();

	if( !(ValidateTechniques()) )
	{
		return false;
	}

	return true;
}