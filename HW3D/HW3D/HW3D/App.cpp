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
#include "MeshRenderer.h"
#include "Renderer.h"

//ECS
#include "SECS.h";
#include <cstdint>

#include "ShroomArcane3D/Surface.h"
#include "ShroomArcane3D/GDIPlusManager.h"

//GDIPlusManager needs to be initialised to use Surface
GDIPlusManager gdipm;

//APP DEF  //MAIN WINDOW
App::App() : root_wnd(900, 600, "Shroom", Window::SHROOM_WINDOW_TYPE::MAIN, nullptr), renderer(root_wnd.Gfx(), secs)
{
	AddCubes(5000);

	secs.AddSystem<Renderer>(&renderer);

	secs.Initialize();
	
	const Surface s = Surface::FromFile("test.png");
	root_wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
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
		//ECS TEST
		auto entity = secs.create();

		secs.assign<Transform>    (entity, rng, adist, ddist, odist, rdist, 3);
		auto& test = secs.assign<MeshRenderer> (entity, Plane::Make<Vertex>(), i + 1);
		test.test = i;

		auto& test2 = secs.get<MeshRenderer>(entity);
		
		//std::stringstream ss;
		//ss << test2.test;

		//MessageBox(nullptr, ss.str().c_str(), "moi2", MB_OK);

		//auto& test = secs.get<MeshRenderer>(entity);
	}
}

void App::RemoveCubes(int amount)
{
	if (boxes.size() > 0)
	{
		for (auto i = 0; i < amount; i++)
		{
			//this insted of release for unique_ptr, release wont destroy
			boxes.back().reset();
			boxes.pop_back();
		}
	}
}

int App::Go()
{
	BOOL result;
	MSG  msg;

	Window wnd(300, 500, "Shroom Tool", Window::SHROOM_WINDOW_TYPE::SECONDARY, root_wnd.handle);

	//This is our main loop
	while (true)
	{
		if (root_wnd.input.mouse.LeftIsPressed())
		{
			//AddCubes(1);
		}

		if (root_wnd.input.mouse.RightIsPressed())
		{
			//RemoveCubes(1);
		}

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

	root_wnd.Gfx().ClearBuffer(Color(0.05f,0.05f,0.1f));

	secs.Update(deltatime);

    root_wnd.Gfx().EndFrame();
}


