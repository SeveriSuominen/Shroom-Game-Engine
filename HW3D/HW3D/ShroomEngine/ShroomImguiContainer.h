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

	std::vector<std::unique_ptr<ShroomImguiView>*> GetByCategory(ShroomImguiView::Category category)
	{
		auto category_views = std::vector<std::unique_ptr<ShroomImguiView>*>();

		for (size_t i = 0; i < views.size(); i++)
		{
			if (views[i].get()->category == category)
			{
				category_views.push_back(&views[i]);
			}
		}
		return category_views;
	}

	void DrawContainerByCategory(App* app)
	{
		std::vector<ShroomImguiView::Category> categories = 
		{
			ShroomImguiView::Category::All, // <---- SHOW EVERYWHERE

			ShroomImguiView::Category::General,
			ShroomImguiView::Category::SECS,
			ShroomImguiView::Category::Rendering,
			ShroomImguiView::Category::Network
		};

		std::vector<std::string> category_names =
		{
		    "ALL",

			"General",
			"SECS",
			"Rendering",
			"Network"
		};

		static bool open;

		ImGui::SetNextWindowBgAlpha(0.4f); // Transparent background

		if (ImGui::BeginMainMenuBar())
		{
			ImGui::Dummy(ImVec2(15.0f, 20.0f));
			if (ImGui::BeginMenu("File"))
			{
				//ShowMenuFile();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}

			ImGui::Dummy(ImVec2(10.0f, 0.0f));

			if (ImGui::BeginTabBar("category_tabs"))
			{
				for (size_t i = 0; i < categories.size(); i++)
				{
					if (categories[i] == ShroomImguiView::Category::All)
					{
						auto category_views = GetByCategory(categories[i]);
						for (size_t i = 0; i < category_views.size(); i++)
						{
							category_views[i]->get()->Draw(app, &open);
						}
						continue;
					}

					if (ImGui::BeginTabItem(category_names[i].c_str()))
					{
						auto category_views = GetByCategory(categories[i]);

						for (size_t i = 0; i < category_views.size(); i++)
						{
							category_views[i]->get()->Draw(app, &open);
						}

						ImGui::EndTabItem();
					}
				}
				ImGui::EndTabBar();
			}

			ImGui::EndMainMenuBar();
		}
	}

	std::vector<std::unique_ptr<ShroomImguiView>> views;
};