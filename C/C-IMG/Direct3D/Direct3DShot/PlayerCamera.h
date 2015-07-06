#pragma once
#include "Camera.h"
#include "Input.h"

/* Íæ¼Ò½Ç¶È¿ØÖÆ */
class PlayerCamera : public Camera
{
	Input* _pInput;
	float _moveSpeed;
	D3DXMATRIXA16 tmpMat;
	void BoundryCheck();
public:

	PlayerCamera(D3DXVECTOR3* inPosition3, float inFov, float inAspect, float inNearZ, float inFarZ, float inMoveSpeed, Input* inInput) 
		: Camera(inPosition3, inFov, inAspect, inNearZ, inFarZ)
	{
		_moveSpeed = inMoveSpeed;
		_pInput = inInput;
	}

	~PlayerCamera()
	{
	}

	float GetMoveSpeed();
	void SetMoveSpeed(float inMoveSpeed);
	void MoveForwards(float inTimeDelta);
	void MoveBackwards(float inTimeDelta);
	void StrafeRight(float inTimeDelta);
	void StrafeLeft(float inTimeDelta);
	void UpdateCamera(float inTimeDelta);
	void UpdateViewProjLastMat();
};
