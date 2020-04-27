#pragma once

#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader(ShroomArcaneGraphics& gfx, const std::wstring& path);
	void Bind(ShroomArcaneGraphics& gfx) noexcept override;
	ID3DBlob* GetBytecode() const noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
};