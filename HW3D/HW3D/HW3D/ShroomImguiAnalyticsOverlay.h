#pragma once

#include "ShroomImguiView.h"

class ShroomImguiAnalyticsOverlay : public ShroomImguiView
{
public:
	ShroomImguiAnalyticsOverlay(ShroomImguiView::Category category) : ShroomImguiView(category) {}
	void Draw(App* app, bool* open) override;
};