#pragma once
#include "d3dx9.h"

/* 准心控制，射击视角 */
class Camera
{
public:
	Camera(D3DXVECTOR3* inPosition3, float fieldOfView, float aspect, float nearZ, float farZ);

	~Camera(){}

	void UpdateViewMatrix(); 

	D3DXVECTOR3* GetPosition();
	D3DXVECTOR4* GetPosition4();
	void SetPosition(D3DXVECTOR3* position);

	void UpdateViewProjectionMatrix();

	D3DXMATRIXA16* GetViewMatrix();
	D3DXMATRIXA16* GetProjectionMatrix();
	D3DXMATRIXA16* GetViewProjectionMatrix();
	D3DXMATRIXA16* GetViewProjectionLastMatrix();
	D3DXVECTOR3* GetLook();
	D3DXVECTOR4* GetLook4();
	D3DXVECTOR3* GetRight();
	D3DXVECTOR3* GetUp();

	void SetLook(D3DXVECTOR3* inLook);
	void SetUp(D3DXVECTOR3* inUp);
	void SetRight(D3DXVECTOR3* inRight);
	virtual void UpdateCamera(); 

protected:
	float _fieldOfView;
	float _aspectRatio;
	float _nearZ;
	float _farZ;

	D3DXVECTOR3 _position3;
	D3DXVECTOR4 _position4;
	float _camInitialY;

	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look3;
	D3DXVECTOR4 _look4;
	D3DXVECTOR3 _right;

	D3DXMATRIXA16 _viewMatrix;
	D3DXMATRIXA16 _projectionMatrix;
	D3DXMATRIXA16 _viewProjectionMatrix;

	void setProjMat(); 

	D3DXVECTOR3 _positiveLookX;
	D3DXVECTOR3 _positiveLookY;
	D3DXVECTOR3 _positiveLookZ;
	D3DXVECTOR3 _negativeLookX;
	D3DXVECTOR3 _negativeLookY;
	D3DXVECTOR3 _negativeLookZ;
};
