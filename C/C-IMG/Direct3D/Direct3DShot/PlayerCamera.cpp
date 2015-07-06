#include "PlayerCamera.h"

float PlayerCamera::GetMoveSpeed()
{
	return _moveSpeed;
}

void PlayerCamera::SetMoveSpeed(float inMoveSpeed)
{
	_moveSpeed = inMoveSpeed;
}

void PlayerCamera::MoveForwards(float inTimeDelta)
{
	this->_position3 += D3DXVECTOR3(this->_look3.x, 0.0f, this->_look3.z) * (_moveSpeed * inTimeDelta);
	BoundryCheck();
}

void PlayerCamera::MoveBackwards(float inTimeDelta)
{
	this->_position3 -= D3DXVECTOR3(this->_look3.x, 0.0f, this->_look3.z) * (_moveSpeed * inTimeDelta);
	BoundryCheck();
}

void PlayerCamera::StrafeRight(float inTimeDelta)
{
	this->_position3 += D3DXVECTOR3(this->_right.x, 0.0f, this->_right.z) * (_moveSpeed * inTimeDelta);
	BoundryCheck();
}

void PlayerCamera::StrafeLeft(float inTimeDelta)
{
	this->_position3 -= D3DXVECTOR3(this->_right.x, 0.0f, this->_right.z) * (_moveSpeed * inTimeDelta);
	BoundryCheck();
}

/* 准心移动 */
void PlayerCamera::UpdateCamera(float inTimeDelta)
{

	D3DXMatrixRotationY(&tmpMat, (_pInput->_mouseState).lX / 300.0f);
	D3DXVec3TransformCoord(&_right, &_right, &tmpMat);
	D3DXVec3TransformCoord(&_look3, &_look3, &tmpMat);

	D3DXMatrixRotationAxis(&tmpMat, &_right, (_pInput->_mouseState).lY / 300.0f);
	D3DXVec3TransformCoord(&_up, &_up, &tmpMat);
	D3DXVec3TransformCoord(&_look3, &_look3, &tmpMat);


	/*
	if( _pInput->IsWPressed() )
	{
		MoveForwards(inTimeDelta);
	}

	if( _pInput->IsSPressed() )
	{
		MoveBackwards(inTimeDelta);
	}

	if( _pInput->IsAPressed() )
	{
		StrafeLeft(inTimeDelta);
	}

	if( _pInput->IsDPressed() )
	{
		StrafeRight(inTimeDelta);
	}
	*/

	_position4.x = _position3.x;
	_position4.y = _position3.y;
	_position4.z = _position3.z;
	_position4.w = 1.0f;

	_look4.x = _look3.x;
	_look4.y = _look3.y;
	_look4.z = _look3.z;
	_look4.w = 1.0f;

	UpdateViewMatrix();

	D3DXMatrixMultiply(&_viewProjectionMatrix , &_viewMatrix, &_projectionMatrix);
}

/* 判断边界 */
void PlayerCamera::BoundryCheck()
{
	if(this->_position3.z > -238)
	{
		this->_position3.z = -238;
	}
	if(this->_position3.z < -272)
	{
		this->_position3.z = -272;
	}
	if(this->_position3.x < -195)
	{
		this->_position3.x = -195;
	}
	if(this->_position3.x > 145)
	{
		this->_position3.x = 145;
	}
}