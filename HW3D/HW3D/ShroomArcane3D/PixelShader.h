#pragma once

#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader(ShroomArcaneGraphics& gfx, const std::wstring& path);
	void Bind(ShroomArcaneGraphics& gfx) noexcept override;
	ID3DBlob* GetBytecode() const noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
};