#include "ShroomArcaneGraphics.h"
#include "ShroomArcaneThrowMacros.h"

#include "dxerr.h"
#include <d3dcompiler.h>
#include <iostream>
#include <filesystem>

#include "Parts.h"

//IMGUI
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

//IMGUIZMO
#include "ImGuizmo\ImGuizmo.h"

#pragma comment(lib, "d3d11.lib")		// linking lib file to ccp
#pragma comment(lib, "D3DCompiler.lib") // linking this to get those shader 

namespace WRL = Microsoft::WRL;
namespace DX  = DirectX;

ShroomArcaneGraphics::ShroomArcaneGraphics(HWND whdl)
{
	static int AntialiasingSampleCount = 2;


	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width  = 0; //DEFAULT
	sd.BufferDesc.Height = 0; //DEFAULT
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //VTEXTURE FORMAT
	sd.BufferDesc.RefreshRate.Numerator   = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //VTEXTURE SCALING
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count	  = 4; //ANTIALIASING
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
			&sd,//ComPtr overloads & opr    //Swap chain description
			&pSwapChain,					//Swap chain pointer
			&pDevice,						//Device pointer
			nullptr,						//Feature level pointer
			&pContext						//Device context
		)
	);

	//Gain access to back buffer texture in swap chain, ComPtr Smart Pointer so automatic releases
	WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	
	//ABOUT ComPtr
	/* 
		If u want to get comptr address without releasing,
		use GetAddressOf() func instead of &pBackBuffer
	*/

	//Using COM
	GFX_THROW_INFO
	(
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer /*No pointer casting required to <void**> with ComPtr in COM queryes*/)
	);

	//Creating render target view
	GFX_THROW_INFO
	(
		pDevice->CreateRenderTargetView
		(
			pBackBuffer.Get(), //Get() to get pointer with ComPtr 
			nullptr,
			&pTarget
		)
	);

	//Depth stensil state buffer desc
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;//Turn on or off writing to the depth stencil buffer
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	//Bind depth stensil state
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	//Depth stensil texture
	WRL::ComPtr<ID3D11Texture2D> pDSTexture;

	//Depth stencil texture description
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width   = 1920;
	descDepth.Height  = 1080;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT; //D32 FLOAT PER INDEX, normal for textures 3xD32FLoat EQ R32G32B32
	//ANTIALIASING stuff
	descDepth.SampleDesc.Count   = 4u;
	descDepth.SampleDesc.Quality = 0u;
	//------------------
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; // Type

	//Create depth stencil texture
	GFX_THROW_INFO
	(
		pDevice->CreateTexture2D(&descDepth, nullptr/*DEFAULT*/, &pDSTexture)
	);

	//Depth stensil view for our depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;//D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;

	//Create depth stencil view
	GFX_THROW_INFO
	(
		pDevice->CreateDepthStencilView(pDSTexture.Get(), &descDSV, &pDSV)
	);
	//--------------------------------------
	// Set Render Target with Stencil
	//--------------------------------------
	//Bind depth stecil view to pipeline
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	//--------------------------------------
	// Viewport
	//--------------------------------------
	D3D11_VIEWPORT vp;
	vp.Width  = 1920;
	vp.Height = 1080;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pContext->RSSetViewports(1u, &vp);

	//IMGUI
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}

void ShroomArcaneGraphics::BeginFrame()
{
	//--------------------------------------
	//IMGUI, new FRAME
	//---------------------------
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//---------------------------	
	//IMGUIZMO, new FRAME
	//---------------------------
	ImGuizmo::BeginFrame();
	//--------------------------------------
}

void ShroomArcaneGraphics::EndFrame()
{
	HRESULT hr;

	//--------------------------------------
	//IMGUI Render to dx11 vbuffer
	//--------------------------------------
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	//--------------------------------------

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

	//CLEAR VIEW
	pContext->ClearRenderTargetView( pTarget.Get(), color /*clear with color*/ );
	
	//CLEAR DEPTH STENCIL 
	pContext->ClearDepthStencilView( pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void ShroomArcaneGraphics::DrawIndexed(UINT count) noexcept
{
	GFX_THROW_INFO_ONLY
	(
		pContext->DrawIndexed(count, 0u, 0u)
	);
}

void ShroomArcaneGraphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	projection = proj;
}

DirectX::XMMATRIX ShroomArcaneGraphics::GetProjection() const noexcept
{
	return projection;
}


