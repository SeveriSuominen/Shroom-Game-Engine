#include "VertexShader.h"
#include "ShroomArcaneThrowMacros.h"

#include <d3dcompiler.h>

VertexShader::VertexShader(ShroomArcaneGraphics & gfx, const std::wstring & path)
{
	//Info mng
	INIT_DXINFOMNG(gfx);

	//Get file
	GFX_THROW_INFO
	(
		D3DReadFileToBlob(path.c_str(), &pBytecodeBlob)
	);

	//Create Vertex shader
	GFX_THROW_INFO_ONLY
	(
		GetDevice(gfx)->CreateVertexShader
		(
			pBytecodeBlob->GetBufferPointer(),
			pBytecodeBlob->GetBufferSize(),
			nullptr,
			&pVertexShader
		)
	);
}

void VertexShader::Bind(ShroomArcaneGraphics & gfx) noexcept
{
	//Bind shader to pipeline
	GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
	return pBytecodeBlob.Get();
}
