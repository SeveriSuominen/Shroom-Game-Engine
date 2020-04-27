#pragma once

#include <DirectXMath.h>

struct Float3 
{
	Float3(float x, float y, float z) : X(x), Y(y), Z(z) {}
public:
	float X, Y, Z;
};