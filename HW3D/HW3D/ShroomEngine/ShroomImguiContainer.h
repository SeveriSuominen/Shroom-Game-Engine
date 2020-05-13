#pragma once

#include "ShroomImguiView.h"
#include "ShroomImguiViews.h"
#include <memory>

//FORWARD DEC
class App;

class ShroomImguiContainer
{
public: 
	ShroomImguiContainer() 
	{
		//-----------------------------------------------------------------
		//MAIN
		//-----------------------------------------------------------------
		//views.push_back(std::make_unique<ShroomImguiMenuBar>(ShroomImguiView::Category::All));

		//-----------------------------------------------------------------
		//Utility
		//-----------------------------------------------------------------
		views.push_back(std::make_unique<ShroomImguiAnalyticsOverlay>(ShroomImguiView::Category::All));
		views.push_back(std::make_unique<ShroomImguiCameraControl>(ShroomImguiView::Category::All));
		views.push_back(std::make_unique<ShroomImguiAssetView>(ShroomImguiView::Category::All));

		views.push_back(std::make_unique<ShroomImguiSECSHierarchy>(ShroomImguiView::Category::SECS));
		views.push_back(std::make_unique<ShroomImguiSECSObjectView>(ShroomImguiView::Category::SECS));
	}

	std::vector<std::unique_ptr<ShroomImguiView>*> GetByCategory(ShroomImguiView::Category category);
	void DrawContainerByCategory(App* app);

	//ALL VIEWS CONTAINER
	std::vector<std::unique_ptr<ShroomImguiView>> views;

private:
	void ShowMenuFile(App* shroom_app);
	void New    (App* shroom_app);
	void Save   (App* shroom_app);
	void SaveAs (App* shroom_app);
	void Load   (App* shroom_app);
};