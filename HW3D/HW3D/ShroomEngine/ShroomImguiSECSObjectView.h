#pragma once

#include "ShroomImguiView.h"
#include "ShroomArcane3D/ShroomArcaneCamera.h"

class ShroomImguiSECSObjectView : public ShroomImguiView
{
public:
	ShroomImguiSECSObjectView(ShroomImguiView::Category category) : ShroomImguiView(category) {}
	void Draw(App* app, bool* open) override;
};