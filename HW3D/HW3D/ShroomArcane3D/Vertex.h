#pragma once

#include "Vectors.h"
#include "Color.h"

struct Vertex 
{
public:
	//Vertex(Float3 vec3) : position(vec3), color(Color(1,1,1)){}
	//Vertex(Float3 vec3, Color vcol) : position(vec3), color(vcol) {}

public: 
	//DEPTH
	//0u
	DirectX::XMFLOAT3 pos;
	
	//12u
	//Color  color;
	struct
	{
		float u;
		float v;
	} tex;
}; 

struct VertexTexCoord
{

};