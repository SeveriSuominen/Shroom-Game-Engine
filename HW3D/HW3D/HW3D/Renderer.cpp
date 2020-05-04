#include "ShroomArcane3D/ShroomArcane3D.h"
#include "Renderer.h"

#include <utility>
#include <iostream>
#include <sstream>;
#include <filesystem>
#include <vector>

#include "ShroomArcane3D/Transform.h";
#include "MeshRenderer.h";
#include <sstream>

void Renderer::Initialize() noexcept
{
	auto view = secs.view<Transform, MeshRenderer>();

	std::stringstream ss;
	ss << view.size();

	//MessageBox(nullptr, ss.str().c_str(), "terve", MB_OK);

	for (auto entity : view) {
		
		// gets only the components that are going to be used ...
		auto& transform = view.get<Transform>(entity);
		auto& renderer  = view.get<MeshRenderer>(entity);
		
		if (!(IsStaticInitialized()))
		{
			renderer.model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.2f));

			//--------------------------------------
			// TEXTURE
			//--------------------------------------
			AddStaticBind(std::make_unique<Texture>(gfx, Surface::FromFile("test.png")));
			AddStaticBind(std::make_unique<Sampler>(gfx, D3D11_TEXTURE_ADDRESS_WRAP  ));
			//--------------------------------------
			// SHADERS
			//--------------------------------------
			auto pvs = std::make_unique<VertexShader>(gfx, L"../ShroomArcane3D/TextureVS.cso");
			auto pvsbc = pvs->GetBytecode();

			AddStaticBind(std::move(pvs));
			AddStaticBind(std::make_unique<PixelShader>(gfx, L"../ShroomArcane3D/TexturePS.cso"));

			//--------------------------------------
			// CONSTANT BUFFERS
			//--------------------------------------
			struct ConstantBuffer2
			{
				struct
				{
					float r;
					float g;
					float b;
					float a;
				} face_colors[6];
			};
			const ConstantBuffer2 cb2 =
			{
				{
					{ 1.0f,0.0f,1.0f },
					{ 1.0f,0.0f,0.0f },
					{ 0.0f,1.0f,0.0f },
					{ 0.0f,0.0f,1.0f },
					{ 1.0f,1.0f,0.0f },
					{ 0.0f,1.0f,1.0f },
				}
			};
			AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

			//--------------------------------------
			// INPUT LAYOUT
			//--------------------------------------
			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
				{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			};
			AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

			//--------------------------------------
			// TOPOLOGY
			//--------------------------------------
			AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}

		//--------------------------------------
		// VERTICES
		//--------------------------------------
		AddBind(std::make_unique<VertexBuffer>(gfx, renderer.model.vertices));

		//--------------------------------------
		// INDICES INPUT
		//--------------------------------------
		AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, renderer.model.indices));


		//--------------------------------------
		// TRANSFORM MATRIX CONSTANT BUFFER
		//--------------------------------------
		//Only non static, automatically shares static pointer for transforms
		AddBind(std::make_unique<TransformCbuf>(gfx, transform/**this*/));

		//Use this method to set index buffer for drawable when StaticInitialized
		//if not, IndexBuffer will be nullptr even we have indices binded to pipeline
		//we still need to set buffer for drawable
		//SetIndexBufferFromStaticBinds();

		Draw(gfx);
	}
}

void Renderer::Update(float dt) noexcept
{
	auto view = secs.view<Transform, MeshRenderer>();
	//MessageBox(nullptr, "terska", "terve", MB_OK);

	for (auto entity : view) 
	{
		auto&  transform = view.get<Transform>   (entity);
		auto&  renderer  = view.get<MeshRenderer>(entity);

		transform.roll  += transform.droll  * dt;
		transform.pitch += transform.dpitch * dt;
		transform.yaw   += transform.dyaw   * dt;
		transform.theta += transform.dtheta * dt;
		transform.phi   += transform.dphi   * dt;
		transform.chi   += transform.dchi   * dt;

		Draw(gfx);
	}
}

