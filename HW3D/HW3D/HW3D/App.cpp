#include "App.h"
#include "ShroomWindow.h"
#include <sstream>

#include <random>
#include <memory>
#include <vector>

//APP DEF  //MAIN WINDOW
App::App() : root_wnd(900, 600, "Shroom")
{
	AddCubes(10);
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
		boxes.push_back(std::make_unique<Box>(
			root_wnd.Gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
}

void App::RemoveCubes(int amount)
{
	if (boxes.size() - amount > 0)
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
	
	//This is our main loop
	while (true)
	{
		if (root_wnd.input.mouse.LeftIsPressed())
		{
			AddCubes(20);
		}

		if (root_wnd.input.mouse.RightIsPressed())
		{
			RemoveCubes(20);
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

	root_wnd.Gfx().ClearBuffer(Color(0.5f,0.5f,1.0f));

	for (auto& b : boxes )
	{
		b->Update(deltatime);
		b->Draw(root_wnd.Gfx());
	}
	root_wnd.Gfx().EndFrame();
}
