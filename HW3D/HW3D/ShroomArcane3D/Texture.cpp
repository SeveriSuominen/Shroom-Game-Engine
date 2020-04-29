#include "Texture.h"
#include "ShroomArcaneThrowMacros.h"
#include <sstream>

Texture::Texture(ShroomArcaneGraphics & gfx, const Surface & s)
{
	INIT_DXINFOMNG(gfx);
	// create texture resource
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = s.GetWidth();
	textureDesc.Height = s.GetHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = s.GetBufferPtr();
	sd.SysMemPitch = s.GetWidth() * sizeof(Surface::Color);
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(
		&textureDesc, &sd, &pTexture
	));

	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
		pTexture.Get(), &srvDesc, &pTextureView
	));
	
	/*
	std::stringstream ss;
	ss << surface.GetHeight();

	MessageBox(nullptr, ss.str().c_str(), " terve ", MB_OK);

	//---------------------------------------------------
	// Texture
	//---------------------------------------------------
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width  = surface.GetWidth ();
	textureDesc.Height = surface.GetHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //Per Pixel format
	textureDesc.SampleDesc.Count   = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; //TYPE
	textureDesc.CPUAccessFlags = 0; // No cpu access;
	textureDesc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = surface.GetBufferPtr(); //p to resource
	sd.SysMemPitch = surface.GetWidth() * sizeof(Surface::Color); // PIXEL * TEXTURE WIDTH == Pitch EQ ROW WIDTH

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

	// Create TEXTURE
	GFX_THROW_INFO
	(
		GetDevice(gfx)->CreateTexture2D
		(
			&textureDesc, &sd, &pTexture
		)
	);

	//---------------------------------------------------
	// Resource view, to bind to pipeline from texture
	//---------------------------------------------------
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderrvDesc = {};
	shaderrvDesc.Format = textureDesc.Format; //Check texture format;
	shaderrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; //Resource type
	shaderrvDesc.Texture2D.MostDetailedMip = 0;
	shaderrvDesc.Texture2D.MipLevels = 1;

	// Create shader resource view from TEXTURE
	GFX_THROW_INFO
	(
		GetDevice(gfx)->CreateShaderResourceView(
			pTexture.Get(), &shaderrvDesc, &pTextureView
		)
	);*/
}

void Texture::Bind(ShroomArcaneGraphics & gfx) noexcept
{	//Bind texture to Pixel shader				  //No releasing wanted
	GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}
