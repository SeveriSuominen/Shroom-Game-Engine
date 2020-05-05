#pragma once

#include "ShroomImguiView.h"

class ShroomImguiMenuBar : public ShroomImguiView
{
public:
	ShroomImguiMenuBar(ShroomImguiView::Category category) : ShroomImguiView(category) {}
	void Draw(App* app, bool* open) override;

private:
	void ShowMenuFile();
};