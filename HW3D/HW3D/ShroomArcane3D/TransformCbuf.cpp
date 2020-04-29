#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(ShroomArcaneGraphics& gfx, const Drawable& parent)
	:
	parent(parent)
{
	// CONFIRM STATIC TRANSFORM CBUFFER ALLOCATED
	if (!pVCbuf)
	{
		pVCbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformCbuf::Bind(ShroomArcaneGraphics& gfx) noexcept
{
	pVCbuf->Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
		)
	);
	pVCbuf->Bind(gfx);
}

//Static var declaration
std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::pVCbuf;