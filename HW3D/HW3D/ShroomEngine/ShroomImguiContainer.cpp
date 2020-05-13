#include "ShroomImguiContainer.h"
#include "ShroomWin32Tools.h"

#include "App.h"

std::vector<std::unique_ptr<ShroomImguiView>*> ShroomImguiContainer::GetByCategory(ShroomImguiView::Category category)
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

void ShroomImguiContainer::DrawContainerByCategory(App* app)
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
			ShowMenuFile(app);
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

void ShroomImguiContainer::ShowMenuFile(App* shroom_app)
{
	ImGui::MenuItem("(dummy menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) { New(shroom_app); }
	if (ImGui::MenuItem("Open", "Ctrl+O")) { Load(shroom_app); }
	if (ImGui::BeginMenu("Open Recent"))
	{
		/*ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowMenuFile(shroom_app);
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}*/
		ImGui::EndMenu();
	}
	//if (ImGui::MenuItem("Load", "Ctrl+L")) { Load(shroom_app); }
	if (ImGui::MenuItem("Save", "Ctrl+S")) { Save(shroom_app); }
	if (ImGui::MenuItem("Save As..")) { SaveAs(shroom_app); }
	ImGui::Separator();
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		static bool b = true;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::Checkbox("Check", &b);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Colors"))
	{
		float sz = ImGui::GetTextLineHeight();
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
			ImGui::Dummy(ImVec2(sz, sz));
			ImGui::SameLine();
			ImGui::MenuItem(name);
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true)) {}
	if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}

void ShroomImguiContainer::New(App* shroom_app)
{
	shroom_app->cache.ResetCache();
	shroom_app->secs.ClearEntities();
}

void ShroomImguiContainer::Load(App* shroom_app)
{
	auto path = ShroomWin32Tools::OpenFileDialog();

	if (path.empty())
	{
		return;
	}

	shroom_app->cache.ResetCache();
	shroom_app->secs.ClearEntities();

	SECS::Scene::Load(shroom_app->secs, path);

	shroom_app->cache.SetCurrentSave(path);

	shroom_app->secs.Initialize();
}

void ShroomImguiContainer::Save(App* shroom_app)
{
	auto path = shroom_app->cache.GetCurrentSave();

	if (path.empty())
	{
		SaveAs(shroom_app);
		return;
	}
	SECS::Scene::Save(shroom_app->secs, path);
}

void ShroomImguiContainer::SaveAs(App* shroom_app)
{
	auto path = ShroomWin32Tools::SaveAsDialog();

	if (path.empty())
	{
		return;
	}
	SECS::Scene::Save(shroom_app->secs, path);
	shroom_app->cache.SetCurrentSave(path);
}
