#pragma once

#include "ShroomArcane3D/ShroomArcane3D.h"
#include <random>

#include "SECS.h"
#include "MeshRenderer.h";

class Renderer : public SECS::System, public DrawableStatic<Renderer>
{
public:
	Renderer(ShroomArcaneGraphics& gfx, SECS& secs) : gfx(gfx), secs(secs), SECS::System(&secs) {};

	void Initialize()     override;
	void Update(float dt) override;
private:
	void CreateRendererBinds(MeshRenderer& renderer, Transform& transform);
	
	SECS& secs;
	ShroomArcaneGraphics& gfx;
};