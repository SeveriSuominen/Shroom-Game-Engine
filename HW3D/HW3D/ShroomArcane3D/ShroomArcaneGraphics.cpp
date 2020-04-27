#include "ShroomArcaneGraphics.h"
#include "dxerr.h"
#include <d3dcompiler.h>
#include <iostream>
#include <filesystem>

#pragma comment(lib, "d3d11.lib")		// linking lib file to ccp
#pragma comment(lib, "D3DCompiler.lib") // linking this to get those shader 
namespace WRL = Microsoft::WRL;

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
	pContext->ClearRenderTargetView( pTarget.Get(), color /*clear with color*/ );
}

void ShroomArcaneGraphics::DrawTestTriangle(float angle)
{
	void DrawTestTriangle();
	{
		//Required HRESULT var for exception macros
		HRESULT hr;

		namespace WRL = Microsoft::WRL;

		//--------------------------------------
		// VERTICE AND INDICE BUFFERS AND INPUT
		//--------------------------------------
		const Vertex vertices[] =
		{
		    { Float3( 0.0f,  0.5f, 0.0f),  Color(1.0f, 0.2f, 0.2f) },
			{ Float3( 0.5f,  -0.5f, 0.5f), Color(0.2f, 1.0f, 0.2f) },
			{ Float3(-0.5f,  -0.5f, 0.2f), Color(0.2f, 0.2f, 1.0)  },
		};

		const int indices[] =
		{
			0,1,2
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
			Matrix4x4 transformation;
		};

		const ConstantBuffer cb = 
		{
			{
				Matrix4x4(angle, 4.0f, 3.0f)
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
		// Render Target
		//--------------------------------------
		//Render Target
		pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf() /*Use this func to not release ComPtr*/, nullptr);

		//--------------------------------------
		// Primitive topology
		//--------------------------------------

		//Primitive topology, (aka indices)
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//--------------------------------------
		// Viewport
		//--------------------------------------
		D3D11_VIEWPORT vp;
		vp.Width  = 800;
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
}
