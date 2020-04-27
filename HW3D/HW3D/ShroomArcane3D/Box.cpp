#include "Box.h"
#include "Bindables.h"
#include "Components.h"
#include "ShroomArcaneThrowMacros.h"
#include <utility>
#include <iostream>
#include <sstream>;
#include <filesystem>
Box::Box(ShroomArcaneGraphics& gfx,
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
		//--------------------------------------
		// VERTICES INPUT
		//--------------------------------------
		struct Vertex
		{
			struct
			{
				float x;
				float y;
				float z;
			} pos;
		};
		const std::vector<Vertex> vertices =
		{
			{ -1.0f,-1.0f,-1.0f },
			{ 1.0f,-1.0f,-1.0f },
			{ -1.0f,1.0f,-1.0f },
			{ 1.0f,1.0f,-1.0f },
			{ -1.0f,-1.0f,1.0f },
			{ 1.0f,-1.0f,1.0f },
			{ -1.0f,1.0f,1.0f },
			{ 1.0f,1.0f,1.0f },
		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

		//--------------------------------------
		// SHADERS
		//--------------------------------------
		auto pvs = std::make_unique<VertexShader>(gfx, L"../ShroomArcane3D/VertexShader.cso");
		auto pvsbc = pvs->GetBytecode();

		AddStaticBind(std::move(pvs));
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"../ShroomArcane3D/PixelShader.cso"));

		//--------------------------------------
		// INDICES INPUT
		//--------------------------------------
		const std::vector<unsigned short> indices =
		{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

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
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		//--------------------------------------
		// TOPOLOGY
		//--------------------------------------
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	//--------------------------------------
	// TRANSFORM MATRIX CONSTANT BUFFER
	//--------------------------------------
	//Only non static
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	//Use this method to set index buffer for drawable when StaticInitialized
	//if not, IndexBuffer will be nullptr even we have indices binded to pipeline
	//we still need to set buffer for drawable
	SetIndexBufferFromStaticBinds();
}

void Box::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}