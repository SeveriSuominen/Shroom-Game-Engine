#pragma once
#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

#include "Transform.h"

class TransformCbuf : public Bindable
{
public:
	TransformCbuf(ShroomArcaneGraphics& gfx, const Transform& parentTransform);
	void Bind(ShroomArcaneGraphics& gfx) noexcept override;
private:
	struct Transforms
	{
		DirectX::XMMATRIX transform;
		DirectX::XMMATRIX transformViewProjection;
	};

	const Transform& parentTransform;

	//ONLY ONE STATIC TRANSFORM BUFFER NEEDED FOR SHARING, WE DONT NEED EVERY SINGLE INSTANCE HAVE THEIR OWN CBUFFER TO BIND,
	//WASTED MEMORY, SO MODIFYING ONLY ONE 
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVCbuf;
};