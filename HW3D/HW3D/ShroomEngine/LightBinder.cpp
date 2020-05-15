
#include "LightBinder.h"
#include "ShroomArcane3D/Bindable.h"
#include "ShroomArcane3D/ShroomArcaneThrowMacros.h"

#include "PointLight.h"
#include "ShroomAssets.h"

void LightBinder::Initialize()
{
	//namespace dx = DirectX;
	//auto view = secs.view<Transform, PointLight>();

	//SetInitialized()

	/*for (auto entity : view) 
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
		
		//renderer.BindLight(gfx);
	}*/
}

void LightBinder::Update(float dt)
{
	auto view = secs.view<Transform, PointLight>();

	for (auto entity : view)
	{
		auto& transform = view.get<Transform>(entity);
		auto& light     = view.get<PointLight>(entity);
		
		light.SpawnControlWindow();

		float DmatrixTranslation[3], DmatrixRotation[3], DmatrixScale[3];
		transform.DecomposeMatrix
		(
			DmatrixTranslation,
			DmatrixRotation,
			DmatrixScale
		);

		DirectX::XMFLOAT4 vec4
		(
			DmatrixTranslation[0], 
			DmatrixTranslation[1],
			DmatrixTranslation[2],
			0
		);

		light.cbuf.Update(gfx, 
			PointLight::PSLightConstants
			{
				vec4,
				{
					light.diffuseColor[0],
					light.diffuseColor[1],
					light.diffuseColor[2],
					light.diffuseColor[3]
				},

			    light.diffuseIntensity,
				
			    light.attConst,
				light.attLin,
				light.attQuad
			}
		);

		light.cbuf.Bind(gfx);

		//Draw(gfx, light.binds, light.pIndexBuffer->GetCount());
	}
}

void LightBinder::SetPos(DirectX::XMFLOAT3 pos)
{
	this->pos = pos;
}