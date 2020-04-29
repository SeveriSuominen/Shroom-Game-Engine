#pragma once

#include "Bindable.h"
#include "Surface.h"

class Texture : public Bindable
{
public:
	Texture  (ShroomArcaneGraphics& gfx, const Surface& surface);
	void Bind(ShroomArcaneGraphics& gfx) noexcept override;
private:
	//Texture resource
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
};