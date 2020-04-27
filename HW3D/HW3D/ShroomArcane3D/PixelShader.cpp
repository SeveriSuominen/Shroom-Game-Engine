#include "PixelShader.h"
#include "ShroomArcaneThrowMacros.h"

#include <d3dcompiler.h>

//L"../ShroomArcane3D/VertexShader.cso"
PixelShader::PixelShader(ShroomArcaneGraphics & gfx, const std::wstring & path)
{
	//Info mng
	INIT_DXINFOMNG(gfx);

	//Read file
	GFX_THROW_INFO
	(
		D3DReadFileToBlob(path.c_str(), &pBytecodeBlob)
	);

	//Create Vertex shader
	GFX_THROW_INFO
	(
		GetDevice(gfx)->CreatePixelShader
		(
			pBytecodeBlob->GetBufferPointer(),
			pBytecodeBlob->GetBufferSize(),
			nullptr,
			&pPixelShader
		)
	);
}

void PixelShader::Bind(ShroomArcaneGraphics & gfx) noexcept
{
	//Bind shader to pipeline
	GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0);
}

ID3DBlob * PixelShader::GetBytecode() const noexcept
{
	return pBytecodeBlob.Get();
}

