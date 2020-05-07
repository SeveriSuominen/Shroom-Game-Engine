#pragma once

#include "ShroomImguiView.h"
#include "ShroomArcane3D/ShroomArcaneCamera.h"

class ShroomImguiSECSHierarchy : public ShroomImguiView
{
public:
	ShroomImguiSECSHierarchy(ShroomImguiView::Category category) : ShroomImguiView(category) {}
	void Draw(App* app, bool* open) override;
};