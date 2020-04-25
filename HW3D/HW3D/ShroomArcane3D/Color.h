#pragma once

struct Color 
{
public:
	Color(float R, float G, float B) : R(R), G(G), B(B), A(1) {};
	Color(float R, float G, float B, float A) : R(R), G(G), B(B), A(A) {};
public:
	float R, G, B, A;
};