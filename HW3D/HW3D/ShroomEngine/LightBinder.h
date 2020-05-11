#pragma once
#include "ShroomArcane3D/ShroomArcane3D.h"
#include "SECS.h"

class LightBinder : public SECS::System, public DrawableStatic<LightBinder>
{
public:
	LightBinder(ShroomArcaneGraphics& gfx, SECS& secs) : gfx(gfx), secs(secs), SECS::System(&secs) {};
	void SetPos(DirectX::XMFLOAT3 pos);
	
	virtual void Initialize() override;
	virtual void Update(float dt) override;

private:
	SECS& secs;
	ShroomArcaneGraphics& gfx;
	DirectX::XMFLOAT3 pos = { 0.0f,0.0f,0.0f };
};