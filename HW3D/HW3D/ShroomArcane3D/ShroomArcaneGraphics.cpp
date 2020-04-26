#include "ShroomArcaneGraphics.h"
#include "dxerr.h"
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

	HRESULT hr; // Error check macros except hr var in local scope

	//Create device and chain, if failed throw graphics exception
	GFX_THROW_INFO
	(
		D3D11CreateDeviceAndSwapChain
		(
			nullptr,						//Adapter pointer
			D3D_DRIVER_TYPE_HARDWARE,		//Driver type
			nullptr,					    //Driver software handle
			D3D10_CREATE_DEVICE_DEBUG,		//Flags
			nullptr,						//Feature levels pointer
			0,								//Feature levels
			D3D11_SDK_VERSION,				//SDK version
			&sd,							//Swap chain description
			&pSwapChain,					//Swap chain pointer
			&pDevice,						//Device pointer
			nullptr,						//Feature level pointer
			&pContext						//Device context
		)
	);

	//Gain access to back buffer texture in swap chain
	ID3D11Resource* pBackBuffer = nullptr;
	
	//Using COM
	GFX_THROW_INFO
	(
		pSwapChain->GetBuffer(22, __uuidof(ID3D11Resource), reinterpret_cast<void**/*PeePee*/>(&pBackBuffer))
	);

	//Creating render target view
	GFX_THROW_INFO
	(
		pDevice->CreateRenderTargetView
		(
			pBackBuffer,
			nullptr,
			&pTarget
		)
	);

	//This temp var can be released
	pBackBuffer->Release();
}

void ShroomArcaneGraphics::EndFrame()
{
	HRESULT hr;
	 
	//Present back buffer result AKA flip buffers
	if (hr = FAILED (pSwapChain->Present(1u, 0u)))
	{
		//We need to catch this one manually, we also have custom exception 
		//for this error
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			//Custom exception
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			GFX_THROW_INFO(hr);
		}
	}
}

void ShroomArcaneGraphics::ClearBuffer(Color c)
{
	const float color[] = {c.R, c.G, c.B, c.A};
	pContext->ClearRenderTargetView( pTarget, color /*clear with color*/ );
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

