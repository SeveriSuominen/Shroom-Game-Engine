#pragma once

#include "ShroomArcaneWin.h"
#include <DirectXMath.h>

struct Matrix4x4 : public DirectX::XMMATRIX
{
public:
	Matrix4x4
	(
		float angle, float aspect_ratio_width, float aspect_ratio_height
	) 	
	: 
	DirectX::XMMATRIX
	(
		(aspect_ratio_height / aspect_ratio_width) *  std::cos(angle), std::sin(angle), 0.0f, 0.0f,
		(aspect_ratio_height / aspect_ratio_width) * -std::sin(angle), std::cos(angle), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	) {};
};