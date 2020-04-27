#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(ShroomArcaneGraphics& gfx, const Drawable& parent)
	:
	vcbuf(gfx),
	parent(parent)
{}

void TransformCbuf::Bind(ShroomArcaneGraphics& gfx) noexcept
{
	vcbuf.Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
		)
	);
	vcbuf.Bind(gfx);
}