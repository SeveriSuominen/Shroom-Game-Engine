#include "ShroomArcaneGraphics.h"
#include "ShroomArcaneThrowMacros.h"

#include "dxerr.h"
#include <d3dcompiler.h>
#include <iostream>
#include <filesystem>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")		// linking lib file to ccp
#pragma comment(lib, "D3DCompiler.lib") // linking this to get those shader 

namespace WRL = Microsoft::WRL;
namespace DX  = DirectX;

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
	dsDesc.DepthEnable    = TRUE; 
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
	descDepth.Width  = 900;
	descDepth.Height = 600;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT; //D32 FLOAT PER INDEX, normal for textures 3xD32FLoat EQ R32G32B32
	//ANTIALIASING stuff
	descDepth.SampleDesc.Count = 1u;
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
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
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

	//CLEAR VIEW
	pContext->ClearRenderTargetView( pTarget.Get(), color /*clear with color*/ );
	
	//CLEAR VIEW
	pContext->ClearDepthStencilView( pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void ShroomArcaneGraphics::DrawTestTriangle(float angle, float x, float y)
{
	//Required HRESULT var for exception macros
	HRESULT hr;

	namespace WRL = Microsoft::WRL;

	DX::XMVECTOR v = DX::XMVectorSet(3.0f, 3.0f, 0.0f, 0.0f);
	auto result = DX::XMVector4Dot(v, v);
	
	//--------------------------------------
	// VERTICE AND INDICE BUFFERS AND INPUT
	//--------------------------------------
	
	//PYRAMID
	/*
	const Vertex vertices[] =
	{
		{ Float3( 0.0f,   0.5f,  0.0f), Color(1.0f, 0.2f, 0.2f) },
		{ Float3( 0.5f,  -0.5f,  0.5f), Color(0.2f, 1.0f, 0.2f) },
		{ Float3(-0.5f,  -0.5f,  0.5f), Color(0.2f, 0.2f, 1.0)  },
		{ Float3(-0.5f,  -0.5f, -0.5f), Color(1.0f, 0.2f, 1.0)  },
		{ Float3( 0.5f,  -0.5f, -0.5f), Color(0.2f, 1.0f, 1.0)  },
		{ Float3(0.0f,   -0.5f,  0.0f), Color(1.0f, 0.2f, 0.2f) },
	};

	const int indices[] =
	{
		0,1,2,
		0,2,3,
		0,4,1,
		0,3,4,
	};*/

	//CUBE
	const Vertex vertices[] =
	{
		{ Float3(-1.0f,  -1.0f, -1.0f), Color(1.0f, 0.2f, 0.2f) },
		{ Float3( 1.0f,  -1.0f, -1.0f), Color(0.2f, 1.0f, 0.2f) },
		{ Float3(-1.0f,   1.0f, -1.0f), Color(0.2f, 0.2f, 1.0)  },
		{ Float3( 1.0f,   1.0f, -1.0f), Color(1.0f, 0.2f, 1.0)  },
		{ Float3(-1.0f,  -1.0f,  1.0f), Color(1.0f, 0.2f, 0.2f) },
		{ Float3( 1.0f,  -1.0f,  1.0f), Color(1.0f, 1.0f, 0.2f) },
		{ Float3(-1.0f,   1.0f,  1.0f), Color(1.0f, 0.2f, 1.0)  },
		{ Float3( 1.0f,   1.0f,  1.0f), Color(1.0f, 0.2f, 1.0)  },

	};

	const int indices[] =
	{
		0,2,1,
		2,3,1,
		1,3,5,
		3,7,5,
		2,6,3,
		3,6,7,
		4,5,7,
		4,7,6,
		0,4,2,
		2,4,6,
		0,1,4,
		1,5,4
	};

	//Tell dx what kind is our VERTEX buffer
	D3D11_BUFFER_DESC vex_bd = {};
	vex_bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// BUFFER TYPE, quite important!
	vex_bd.Usage = D3D11_USAGE_DEFAULT;				//GPU & CPU Write and Read access setup;
	vex_bd.CPUAccessFlags = 0u;						//More access stuff
	vex_bd.MiscFlags = 0u;							//Misc
	vex_bd.ByteWidth = sizeof(vertices);			//Size of vertex buffer
	vex_bd.StructureByteStride = sizeof(Vertex);	//Size of single vertex
		
								
	//Tell dx what kind is our INDICE buffer
	D3D11_BUFFER_DESC ind_bd = {};
	ind_bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		
	ind_bd.Usage = D3D11_USAGE_DEFAULT;				
	ind_bd.CPUAccessFlags = 0u;						
	ind_bd.MiscFlags = 0u;							
	ind_bd.ByteWidth = sizeof(indices);			    
	ind_bd.StructureByteStride = sizeof(int);	 


	//Vertex Resource
	D3D11_SUBRESOURCE_DATA vex_sd = {};
	vex_sd.pSysMem = vertices;

	//Index Resource
	D3D11_SUBRESOURCE_DATA ind_sd = {};
	ind_sd.pSysMem = indices;

	WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	WRL::ComPtr<ID3D11Buffer> pIndexBuffer;

	GFX_THROW_INFO
	(
		pDevice->CreateBuffer(&vex_bd /*Setup*/, &vex_sd/*Resources*/, &pVertexBuffer/*Buffer goes here!*/)
	);

	GFX_THROW_INFO
	(
		pDevice->CreateBuffer(&ind_bd /*Setup*/, &ind_sd/*Resources*/, &pIndexBuffer/*Buffer goes here!*/)
	);

	const UINT vex_stride = sizeof(Vertex);
	const UINT offset = 0u;
	
	//IA as InputAssembler
	//Bind our vertex buffer to pipeline
	pContext->IASetVertexBuffers
	(
		0u,							  //Start index for buffer
		1u,							  //Amount of of buffers
		pVertexBuffer.GetAddressOf(), //THE vertex buffer
		&vex_stride,
		&offset
	);

	//Bind our vertex buffer to pipeline
	pContext->IASetIndexBuffer
	(
		pIndexBuffer.Get(), 
		DXGI_FORMAT_R32_UINT,
		0u
	);

	//--------------------------------------
	// CONSTANT BUFFER 
	//--------------------------------------
	struct ConstantBuffer
	{
		DX::XMMATRIX transformation;
	};

	const ConstantBuffer cb = 
	{
		{   DX::XMMatrixTranspose
			(	DX::XMMatrixRotationX(angle) *
				DX::XMMatrixRotationY(angle) *					//YRotation
				DX::XMMatrixScaling(3.0f / 4.0f, 1.0f, 1.0f) *  //Scaling
				DX::XMMatrixTranslation(x, 0.0f, y * 10.0f)	*			//Position
				DX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
			)
		}
	};

	WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
		
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC; //dynamic buffer because we want to update constant buffer
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //give CPU access to write for updating buffer
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u; //stride no need because not array, so zero

	//Constant buffer resource
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;

	//Create constant buffer
	GFX_THROW_INFO_ONLY
	(
		pDevice->CreateBuffer
		(
			&cbd,
			&csd,
			&pConstantBuffer
		)
	);
	pContext->VSSetConstantBuffers( 0u,1u, pConstantBuffer.GetAddressOf());


	//--------------------------------------
	// CONSTANT BUFFER, COLOR
	//--------------------------------------
	struct ConstantBuffer2
	{
		Color face_colors[6];
	};

	const ConstantBuffer2 cb2 =
	{
		{  
			Color(0.0f, 1.0f, 1.0f),
			Color(1.0f, 1.0f, 0.0f),
			Color(1.0f, 0.0f, 1.0f),
			Color(1.0f, 0.0f, 0.0f),
			Color(0.0f, 1.0f, 0.0f),
			Color(0.0f, 0.0f, 1.0f),
		}
	};

	WRL::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	GFX_THROW_INFO(pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));

	// bind constant buffer to pixel shader
	pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

	//--------------------------------------
	// SHADERS
	//--------------------------------------
	//Pile of data, temp
	WRL::ComPtr<ID3DBlob> pBlob; 
	
	WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	WRL::ComPtr<ID3D11PixelShader>  pPixelShader;

	GFX_THROW_INFO
	(
		D3DReadFileToBlob(L"../ShroomArcane3D/PixelShader.cso", &pBlob)
	);

	//Create pixel shader
	GFX_THROW_INFO_ONLY
	(
		pDevice->CreatePixelShader
		(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&pPixelShader
		)
	);
	//Bind shader to pipeline
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	GFX_THROW_INFO
	(
		D3DReadFileToBlob(L"../ShroomArcane3D/VertexShader.cso", &pBlob)
	);

	//Create vertex shader
	GFX_THROW_INFO_ONLY
	(
		pDevice->CreateVertexShader
		(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&pVertexShader
		)
	);

	//Bind shader to pipeline
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
	

	//--------------------------------------
	// Input layout
	//--------------------------------------
	//Input vertex layout
	WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{
			"Position",					 //SEMANTIC
			0,							 //SEMANTIC INDEX
			DXGI_FORMAT_R32G32B32_FLOAT, //TELLS ELEMENT IS 3 x FLOAT VALUES
			0,							 //INPUTSLOT
			0,							 //OFFSET OF BYTES BEGINNING OF THE STRUCT
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"Color",					 
			0,							 
			DXGI_FORMAT_R32G32B32A32_FLOAT, //TELLS ELEMENT IS 4 x FLOAT VALUES
			0,							 
			12u, //Balls deep here							 
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	GFX_THROW_INFO
	(
		pDevice->CreateInputLayout
		(
			ied, (UINT)std::size(ied),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout
		)
	);

	//Bind vertex layout
	pContext->IASetInputLayout(pInputLayout.Get());

	//--------------------------------------
	// Primitive topology
	//--------------------------------------
	//Primitive topology, (aka indices in this case)
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//--------------------------------------
	// Viewport
	//--------------------------------------
	D3D11_VIEWPORT vp;
	vp.Width  = 900;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pContext->RSSetViewports(1u, &vp);

	//--------------------------------------
	// Draw
	//--------------------------------------
	//Draw call
	GFX_THROW_INFO_ONLY(pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
	
}
