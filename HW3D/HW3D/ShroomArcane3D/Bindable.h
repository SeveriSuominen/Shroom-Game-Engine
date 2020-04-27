#pragma once

#include "ShroomArcaneGraphics.h"

class Bindable
{
	friend class Shader;

	public:
		virtual void Bind(ShroomArcaneGraphics& gfx) noexcept = 0;
		virtual ~Bindable() = default;

	//Only for Bindable children classes
	protected:
		static ID3D11DeviceContext* GetContext     (ShroomArcaneGraphics& gfx) noexcept;
		static ID3D11Device*        GetDevice	   (ShroomArcaneGraphics& gfx) noexcept;
		static DxgiInfoManager&     GetInfoManager (ShroomArcaneGraphics& gfx);
};