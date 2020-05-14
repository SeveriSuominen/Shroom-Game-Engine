#include "ShroomArcaneCamera.h"

/*void ShroomArcaneCamera::SetMatrix(DirectX::FXMMATRIX cameraMatrix)
{
	this->cameraMatrix = cameraMatrix;
}*/

DirectX::XMMATRIX ShroomArcaneCamera::GetCameraMatrix()
{
	return //DirectX::XMMatrixRotationRollPitchYaw(theta, phi, 0.0f);
		// DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);//*
		//DirectX::XMMatrixRotationRollPitchYaw(theta, phi, 0.0f);
}

DirectX::XMMATRIX ShroomArcaneCamera::GetMatrix()
{
	const auto pos = DirectX::XMVector3Transform
	(
		DirectX::XMVectorSet(0.0f, 0.0f, -r, 0.0f), // <--- ORIGIN
		DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f) // <--- ROTATE AROUND ORIGIN
	);

	return DirectX::XMMatrixLookAtLH
	(
		pos, DirectX::XMVectorZero(), //<--- look at camera position -> position
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) //Camera rotation
	) * DirectX::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll);
}

void ShroomArcaneCamera::Reset()
{
	r = -20.0f;
	roll = 0.0f;
	pitch = 0.0f;
	yaw = 0.0f;
    theta = 0.0f;
	phi = 0.0f;
}
