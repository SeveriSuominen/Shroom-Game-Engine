#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology(ShroomArcaneGraphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(ShroomArcaneGraphics& gfx) noexcept override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};