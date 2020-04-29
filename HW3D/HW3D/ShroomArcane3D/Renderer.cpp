#include "Renderer.h"
#include "Bindables.h"
#include "Components.h"
#include "ShroomArcaneThrowMacros.h"

#include <utility>
#include <iostream>
#include <sstream>;
#include <filesystem>

Renderer::Renderer(ShroomArcaneGraphics& gfx,

	IndexedTriangleList<Vertex> model,

	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	if ( ! (IsStaticInitialized()))
	{

		model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.2f));

		//--------------------------------------
		// TEXTURE
		//--------------------------------------
		//AddStaticBind(std::make_unique<Texture>(gfx, Surface::FromFile("test.png")));
		//AddStaticBind(std::make_unique<Sampler>(gfx, D3D11_TEXTURE_ADDRESS_WRAP  ));
		//--------------------------------------
		// SHADERS
		//--------------------------------------
		auto pvs = std::make_unique<VertexShader>(gfx, L"../ShroomArcane3D/VertexShader.cso");
		auto pvsbc = pvs->GetBytecode();

		AddStaticBind(std::move(pvs));
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"../ShroomArcane3D/PixelShader.cso"));

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
	AddBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

	//--------------------------------------
	// INDICES INPUT
	//--------------------------------------
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));


	//--------------------------------------
	// TRANSFORM MATRIX CONSTANT BUFFER
	//--------------------------------------
    //Only non static, automatically shares static pointer for transforms
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	//Use this method to set index buffer for drawable when StaticInitialized
	//if not, IndexBuffer will be nullptr even we have indices binded to pipeline
	//we still need to set buffer for drawable
	//SetIndexBufferFromStaticBinds();
}

void Renderer::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Renderer::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}