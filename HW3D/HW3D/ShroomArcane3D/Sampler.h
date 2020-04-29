#pragma once

#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler   (ShroomArcaneGraphics& gfx, const D3D11_TEXTURE_ADDRESS_MODE textureAddressMode);
	void Bind (ShroomArcaneGraphics& gfx) noexcept override;
protected: 
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler; //Sampler
};