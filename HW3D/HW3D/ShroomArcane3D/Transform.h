#pragma once

//#include "ShroomArcane3D/ShroomArcane3D.h"
#include <DirectXMath.h>
#include <random>

struct Transform
{
public:
	Transform
	(
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		int test
	) :
		//r(rdist(rng)),
		droll(ddist(rng)),
		dpitch(ddist(rng)),
		dyaw(ddist(rng)),
		dphi(odist(rng)),
		dtheta(odist(rng)),
		dchi(odist(rng)),
		chi(adist(rng)),
		theta(adist(rng)),
		phi(adist(rng))
	{
		//TEMP
		pos.z = rdist(rng);
	}

	DirectX::XMMATRIX GetTransformXM() const noexcept
	{
		return  DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			    DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) *
			    DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
	}

	int test;

	DirectX::XMFLOAT3 pos;

	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};