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
	//ONLY ONE STATIC TRANSFORM BUFFER NEEDED FOR SHARING, WE DONT NEED EVERY SINGLE INSTANCE HAVE THEIR OWN CBUFFER TO BIND,
	//WASTED MEMORY, SO MODIFYING ONLY ONE 
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVCbuf;
	const Drawable& parent;
};