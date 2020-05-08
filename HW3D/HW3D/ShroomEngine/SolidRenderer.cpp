#include "SolidRenderer.h"

#include "SolidRenderer.h"
#include "ShroomArcane3D/Bindable.h"
#include "ShroomArcane3D/ShroomArcaneThrowMacros.h"

#include "PointLight.h"
#include "ShroomAssets.h"

void SolidRenderer::Initialize()
{
	namespace dx = DirectX;
	auto view = secs.view<Transform, PointLight>();

	for (auto entity : view) 
	{
		auto& transform = view.get<Transform>(entity);
		auto& renderer  = view.get<PointLight>(entity);

		renderer.model.Transform(dx::XMMatrixScaling(1, 1, 1));

		if (!IsStaticInitialized())
		{
			auto pvs = ShroomAssets::Get_shader<VertexShader>(gfx, "SolidVS.cso"); 
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(ShroomAssets::Get_shader<PixelShader>(gfx, "SolidPS.cso"));

			struct PSColorConstant
			{
				dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
				float padding;
			} colorConst;
			AddStaticBind(std::make_unique<PixelConstantBuffer<PSColorConstant>>(gfx, colorConst));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			};
			AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

			AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}

		renderer.AddBind(std::make_unique<VertexBuffer>(gfx, renderer.model.vertices));
		renderer.AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, renderer.model.indices));
		renderer.AddBind(std::make_unique<TransformCbuf>(gfx, transform));
	}
}

void SolidRenderer::Update(float dt)
{
	auto view = secs.view<Transform, PointLight>();

	for (auto entity : view)
	{
		auto& transform = view.get<Transform>(entity);
		auto& renderer = view.get<PointLight>(entity);

		Draw(gfx, renderer.binds, renderer.pIndexBuffer->GetCount());
	}
}

void SolidRenderer::SetPos(DirectX::XMFLOAT3 pos)
{
	this->pos = pos;
}