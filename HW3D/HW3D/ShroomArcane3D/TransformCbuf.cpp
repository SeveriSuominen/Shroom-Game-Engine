
#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(ShroomArcaneGraphics& gfx, const Transform& parentTransform, UINT slot)
	:
	parentTransform(parentTransform)
{
	// CONFIRM STATIC TRANSFORM CBUFFER ALLOCATED
	if (!pVCbuf)
	{
		pVCbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}
}

void TransformCbuf::Bind(ShroomArcaneGraphics& gfx) noexcept
{
	auto transform = parentTransform.GetTransformXM();

	const Transforms transforms
	{  
		DirectX::XMMatrixTranspose //World transform
		(
			transform
		),
	
		DirectX::XMMatrixTranspose //View projection
		(
			transform * gfx.camera.GetMatrix() * gfx.GetProjection()
		)
	};

	pVCbuf->Update(gfx,
		transforms
	);
	pVCbuf->Bind(gfx);
}

//Static var declaration
std::unique_ptr<VertexConstantBuffer< TransformCbuf::Transforms>> TransformCbuf::pVCbuf;