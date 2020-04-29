#include "Sampler.h"
#include "ShroomArcaneThrowMacros.h"

Sampler::Sampler(ShroomArcaneGraphics & gfx, const D3D11_TEXTURE_ADDRESS_MODE textureAddressMode)
{
	INIT_DXINFOMNG(&gfx);

	/*//Texture sampler desc
	D3D11_SAMPLER_DESC samplerDesc = {};

	//Many filter modes to different looking results
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //Linear smoother blending
	
	//UVW modes, Wrap, Clamp, or Box, same for all dims for now
	samplerDesc.AddressU = textureAddressMode;
	samplerDesc.AddressV = textureAddressMode;
	samplerDesc.AddressW = textureAddressMode;

	//Create texture sampler
	GFX_THROW_INFO
	(
		GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler)
	);*/

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
}

void Sampler::Bind(ShroomArcaneGraphics & gfx) noexcept
{
	//Set pixel shader sampler
	GetContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
}
