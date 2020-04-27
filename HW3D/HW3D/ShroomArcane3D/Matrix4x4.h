#pragma once

#include "ShroomArcaneWin.h"

struct Matrix4x4
{
public:
	Matrix4x4(float angle, float aspect_ratio_width, float aspect_ratio_height) 
	{
		matrix =
		{
				(aspect_ratio_height / aspect_ratio_width) *  std::cos(angle), std::sin(angle), 0.0f, 0.0f,
				(aspect_ratio_height / aspect_ratio_width) * -std::sin(angle), std::cos(angle), 0.0f, 0.0f,
				0.0f		   , 0.0f			, 1.0f, 0.0f,
				0.0f		   , 0.0f			, 0.0f, 1.0f
		};
	};
	struct
	{
		float matrix[4][4];
	} matrix;
};