#pragma once

#include "ShroomArcane3D/ShroomArcane3D.h"

struct MeshRenderer 
{
public:
	MeshRenderer
	(
		IndexedTriangleList<Vertex> model
	)
	: model(model) {}

	IndexedTriangleList<Vertex> model;
};

