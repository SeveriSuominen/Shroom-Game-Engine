#pragma once
#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
public:
	TransformCbuf(ShroomArcaneGraphics& gfx, const Drawable& parent);
	void Bind(ShroomArcaneGraphics& gfx) noexcept override;
private:
	VertexConstantBuffer<DirectX::XMMATRIX> vcbuf;
	const Drawable& parent;
};