#pragma once

#include "ShroomImguiView.h"
#include "ShroomArcane3D/ShroomArcaneCamera.h"

class ShroomImguiCameraControl : public ShroomImguiView
{
public:
	ShroomImguiCameraControl(ShroomImguiView::Category category) : ShroomImguiView(category) {}
	void Draw(App* app, bool* open) override;
};