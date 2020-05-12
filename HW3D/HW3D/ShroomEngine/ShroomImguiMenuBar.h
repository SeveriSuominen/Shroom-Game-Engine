#pragma once

#include "ShroomImguiView.h"

class ShroomImguiMenuBar : public ShroomImguiView
{
public:
	ShroomImguiMenuBar(ShroomImguiView::Category category) : ShroomImguiView(category) {}
	void Draw(App* app, bool* open) override;

private:
	void ShowMenuFile(App* shroom_app);
	void Load(App* shroom_app);
	void Save(App* shroom_app);
};