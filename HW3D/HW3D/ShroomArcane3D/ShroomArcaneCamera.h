#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class ShroomArcaneCamera
{
public:
	//void SetMatrix(DirectX::FXMMATRIX cameraMatrix);
	void Reset();

	DirectX::XMMATRIX GetCameraMatrix();
	DirectX::XMMATRIX GetMatrix();
private:
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX cameraMatrix;

//3D controls
public:
	float r = 1.0f;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta = 0.0f;
	float phi = 0.0f; 
};