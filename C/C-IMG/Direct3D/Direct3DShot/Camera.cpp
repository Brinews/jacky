#include "Camera.h"

Camera::Camera(D3DXVECTOR3* position3, 
					 float fov, 
					 float aspect, 
					 float nearZ, 
					 float farZ)
{
	_positiveLookX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_positiveLookY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_positiveLookZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	_negativeLookX = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	_negativeLookY = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	_negativeLookZ = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	_position3 = *position3;
	_fieldOfView = fov;
	_aspectRatio = aspect;
	_nearZ = nearZ;
	_farZ = farZ;
	_up = _positiveLookY;
	_look3 = _positiveLookZ;
	_right = _positiveLookX;
	_camInitialY = _position3.y;
	UpdateViewProjectionMatrix();
}

D3DXVECTOR3* Camera::GetPosition()
{
	return &_position3;
}

void Camera::SetPosition(D3DXVECTOR3* position)
{
	_position3 = *position;
	_position4.x = _position3.x;
	_position4.y = _position3.y;
	_position4.z = _position3.z;
	_position4.w = 1.0f;
}

void Camera::UpdateViewProjectionMatrix()
{
	UpdateViewMatrix(); 
	setProjMat(); 

	D3DXMatrixMultiply(&_viewProjectionMatrix, &_viewMatrix, &_projectionMatrix);
}

D3DXVECTOR4* Camera::GetPosition4()
{
	return &_position4;
}

D3DXMATRIXA16* Camera::GetViewMatrix()
{
	return &_viewMatrix;
}

D3DXMATRIXA16* Camera::GetProjectionMatrix()
{
	return &_projectionMatrix;
}

D3DXMATRIXA16* Camera::GetViewProjectionMatrix()
{
	return &_viewProjectionMatrix;
}

D3DXVECTOR3* Camera::GetLook()
{
	/*D3DXVECTOR3 look;
	look.x = _viewMatrix._13;
	look.y = _viewMatrix._23;
	look.z = _viewMatrix._33;
	D3DXVECTOR3* lookBack = new D3DXVECTOR3(-look.x,-look.y,-look.z);*/
	return &_look3;
}

D3DXVECTOR4* Camera::GetLook4()
{
	return &_look4;
}
D3DXVECTOR3* Camera::GetRight()
{
	return &_right;
}

D3DXVECTOR3* Camera::GetUp()
{
	return &_up;
}

void Camera::SetLook(D3DXVECTOR3* look)
{
	_look3 = *look;
	_look4.x = _look3.x;
	_look4.y = _look3.y;
	_look4.z = _look3.z;
	_look4.w = 1.0f;
}

void Camera::SetUp(D3DXVECTOR3* up)
{
	_up = *up;
}

void Camera::SetRight(D3DXVECTOR3* right)
{
	_right = *right;
}

void Camera::UpdateViewMatrix()
{
	D3DXVec3Normalize(&_look3, &_look3);
	D3DXVec3Cross(&_up, &_look3, &_right);
	D3DXVec3Normalize(&_up, &_up);
	D3DXVec3Cross(&_right, &_up, &_look3);
	D3DXVec3Normalize(&_right, &_right);

	(_viewMatrix)(0,0) = _right.x; (_viewMatrix)(0, 1) = _up.x; (_viewMatrix)(0, 2) = _look3.x; (_viewMatrix)(0, 3) = 0.0f;
	(_viewMatrix)(1,0) = _right.y; (_viewMatrix)(1, 1) = _up.y; (_viewMatrix)(1, 2) = _look3.y; (_viewMatrix)(1, 3) = 0.0f;
	(_viewMatrix)(2,0) = _right.z; (_viewMatrix)(2, 1) = _up.z; (_viewMatrix)(2, 2) = _look3.z; (_viewMatrix)(2, 3) = 0.0f;
	(_viewMatrix)(3,0) = -D3DXVec3Dot(&_right, &_position3);       
	(_viewMatrix)(3, 1) = -D3DXVec3Dot(&_up, &_position3);    
	(_viewMatrix)(3, 2) = -D3DXVec3Dot(&_look3, &_position3);      
	(_viewMatrix)(3, 3) = 1.0f;
}

void Camera::setProjMat()
{
	D3DXMatrixPerspectiveFovLH( &_projectionMatrix, _fieldOfView, _aspectRatio, _nearZ, _farZ );
}

void Camera::UpdateCamera()
{
	UpdateViewMatrix();

	D3DXMatrixMultiply(&_viewProjectionMatrix , &_viewMatrix, &_projectionMatrix);
}