#pragma once
#pragma once

#include "ShroomImguiView.h"

class ShroomImguiAssetView : public ShroomImguiView
{
public:
	ShroomImguiAssetView(ShroomImguiView::Category category) : ShroomImguiView(category) {}
	void Draw(App* app, bool* open) override;
};