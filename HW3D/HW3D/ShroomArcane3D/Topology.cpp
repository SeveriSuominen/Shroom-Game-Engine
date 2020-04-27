#include "Topology.h"

Topology::Topology(ShroomArcaneGraphics & gfx, D3D11_PRIMITIVE_TOPOLOGY type) : type(type) {}

void Topology::Bind(ShroomArcaneGraphics & gfx) noexcept
{
	GetContext(gfx)->IASetPrimitiveTopology(type);
}
