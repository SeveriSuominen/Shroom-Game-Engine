#include "App.h"
#include "ShroomWindow.h"
#include <sstream>

#include "Sphere.h"
#include "Cube.h"
#include "Cone.h"
#include "Plane.h"

#include <random>
#include <memory>
#include <vector>

#include "ShroomArcane3D/Transform.h"
#include "PointLight.h"
#include "MeshRenderer.h"

#include "imgui/IconsFontAwesome5.h"

#include <filesystem>

//ECS
#include "SECS.h";
#include <cstdint>

//IMGUI
#include "ShroomArcane3D/imgui/imgui_impl_dx11.h"
#include "ShroomArcane3D/imgui/imgui_impl_win32.h"

//LUA
//#include "ShroomScript/ShroomLua.h"

//APP DEF  //MAIN WINDOW
App::App() : root_wnd(1920, 1080, "Shroom", Window::SHROOM_WINDOW_TYPE::MAIN, nullptr), secs(root_wnd.Gfx()) /*, solid_renderer(root_wnd.Gfx(), secs), renderer(root_wnd.Gfx(), secs)*/
{
	//SECS::Scene::Load(secs);

	/*auto& shroomentity = SECS::Entity::Create("Point light", this->secs);
	shroomentity.get()->AssignComponent<PointLight>(root_wnd.Gfx());

	AddCubes(1000);*/
	
	secs.AddSystem<LightBinder>();
	secs.AddSystem<Renderer>();

	//INITIALIZE ALL SYSTEMS
	secs.Initialize();
	
	//const Surface s = Surface::FromFile("test.png");

	root_wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f/*Min clip*/, 400.0f/*Max clip*/));

	//SECS::Scene::Save(secs);
	//SECS::Scene::Load(secs);
}

void App::AddCubes(int amount)
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

	for (auto i = 0; i < amount; i++)
	{
		auto& shroomentity  = SECS::Entity::Create("entiteetti", this->secs);
		shroomentity.get()  -> AssignComponent<MeshRenderer>();
	}
}

int App::Go()
{
	BOOL result;
	MSG  msg;

	//--------------------------------------------
	//IMGUI FONTS
	//--------------------------------------------
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;

	io.Fonts->AddFontFromFileTTF(std::filesystem::current_path().append("fa-solid-900.ttf").string().c_str(), 16.0f, &icons_config, icons_ranges);
	//--------------------------------------------

	//This is our main loop
	while (true)
	{
		//Peeking messages from all windows with static ProcessMessages
		if (const auto ecode = Window::ProcessMessages()) 
		{
			return *ecode;
		}

		//Do frames when no messages EQ: No input data transmission, do stuff based 
		//current data else stop and wait;
		DoFrame();
	}

	if (result == -1)
	{
		throw SWND_EXC_LAST();
	}

	return msg.wParam;
}

void App::DoFrame()
{
	auto time      = timer.Peek();
	auto deltatime = timer.Mark();

	root_wnd.Gfx().ClearBuffer(Color(0.05f, 0.05f, 0.1f));
	root_wnd.Gfx().BeginFrame();

	secs.Update(deltatime);

	auto view = secs.view<Transform, MeshRenderer>();

	//---------------------------
	//IMGUI
	//---------------------------
	/*static bool show_demo_window = true;
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}*/
	DrawImguiViews();
	//---------------------------

	//present, swap
    root_wnd.Gfx().EndFrame();
}

void App::DrawImguiViews()
{
	static bool open = true;
	imguiContainer.DrawContainerByCategory(this);
}


