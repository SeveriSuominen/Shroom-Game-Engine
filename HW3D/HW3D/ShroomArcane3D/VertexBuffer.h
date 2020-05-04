#pragma once
#include "ShroomArcaneWin.h"
#include <sstream>
#include "Bindable.h"
#include "ShroomArcaneThrowMacros.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(ShroomArcaneGraphics& gfx, const std::vector<V>& vertices)
		:
		stride(sizeof(V))
	{
		INIT_DXINFOMNG(gfx);

		std::stringstream ss;
		ss << vertices.size();

		//MessageBox(nullptr, ss.str().c_str(), "moi", MB_OK);

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(V) * vertices.size());
		bd.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}
	void Bind(ShroomArcaneGraphics& gfx) noexcept override;
protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};