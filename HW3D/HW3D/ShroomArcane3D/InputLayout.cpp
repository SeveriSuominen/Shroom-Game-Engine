#include "InputLayout.h"
#include "ShroomArcaneThrowMacros.h"

InputLayout::InputLayout(ShroomArcaneGraphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
{
	INIT_DXINFOMNG(gfx);

	GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	));
}

void InputLayout::Bind(ShroomArcaneGraphics& gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(pInputLayout.Get());
}