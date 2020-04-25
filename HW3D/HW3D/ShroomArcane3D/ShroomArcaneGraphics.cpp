#include "ShroomArcaneGraphics.h"

#pragma comment(lib, "d3d11.lib") // linking lib file to ccp

ShroomArcaneGraphics::ShroomArcaneGraphics(HWND whdl)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width  = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //VTEXTURE FORMAT
	sd.BufferDesc.RefreshRate.Numerator   = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //VTEXTURE SCALING
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count	  = 1; //ANTIALIASING
	sd.SampleDesc.Quality = 0; //ANTIALIASING
	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1; //<- ONE FRONT AND ONE BACK BUFFER
	sd.OutputWindow = whdl;
	sd.Windowed		= TRUE;
	sd.SwapEffect	= DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	//Create device and chain
	D3D11CreateDeviceAndSwapChain
	(
		nullptr,						//Adapter pointer
		D3D_DRIVER_TYPE_HARDWARE,		//Driver type
		nullptr,					    //Driver software handle
		0,								//Flags
		nullptr,						//Feature levels pointer
		0,								//Feature levels
		D3D11_SDK_VERSION,				//SDK version
		&sd,							//Swap chain description
		&pSwapChain,					//Swap chain pointer
		&pDevice,						//Device pointer
		nullptr,						//Feature level pointer
		&pContext						//Device context
	);

	//Gain access to back buffer texture in swap chain
	ID3D11Resource* pBackBuffer = nullptr;
	//Using COM
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**/*PeePee*/>(&pBackBuffer));

	//Creating render target view
	pDevice->CreateRenderTargetView
	(
		pBackBuffer,
		nullptr,
		&pTarget
	);
	//This temp var can be released
	pBackBuffer->Release();
}

void ShroomArcaneGraphics::EndFrame()
{
	//Present result
	pSwapChain->Present(1u, 0u);
}

void ShroomArcaneGraphics::ClearBuffer(Color c)
{
	const float color[] = {c.R, c.G, c.B, c.A};
	pContext->ClearRenderTargetView( pTarget, color );
}

ShroomArcaneGraphics::~ShroomArcaneGraphics()
{
	if (pContext != nullptr)
		pContext->Release();
	if (pSwapChain != nullptr)
		pSwapChain->Release();
	if (pDevice != nullptr)
		pDevice->Release();
}

