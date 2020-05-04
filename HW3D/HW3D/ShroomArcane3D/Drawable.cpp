#include "Drawable.h"
#include "ShroomArcaneThrowMacros.h"
#include "IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(ShroomArcaneGraphics& gfx) const noexcept
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}

	for (auto& sb : GetStaticBinds())
	{
		sb->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::Draw(ShroomArcaneGraphics& gfx, std::vector<std::unique_ptr<Bindable>>& manual_binds, UINT count) const noexcept
{
	for (auto& b : manual_binds)
	{
		b->Bind(gfx);
	}

	for (auto& sb : GetStaticBinds())
	{
		sb->Bind(gfx);
	}

	gfx.DrawIndexed(count);
}


void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
{
	assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
	pIndexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}
