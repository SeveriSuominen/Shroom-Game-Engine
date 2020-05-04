#pragma once

#include "ShroomArcane3D/ShroomArcane3D.h"
#include <random>

#include "SECS.h"


class Renderer : public SECS::System, public DrawableStatic<Renderer>
{
public:
	Renderer(ShroomArcaneGraphics& gfx, SECS& secs) : gfx(gfx), secs(secs), SECS::System(&secs) {};

	void Initialize()     noexcept override;
	void Update(float dt) noexcept override;
private:
	SECS& secs;
	ShroomArcaneGraphics& gfx;
};