#include "App.h"
#include "ShroomWindow.h"
#include <sstream>

//APP DEF  //MAIN WINDOW
App::App() : root_wnd(900, 600, "Shroom"){}

int App::Go()
{
	BOOL result;
	MSG  msg;
	
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
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	root_wnd.Gfx().ClearBuffer(Color(c,c,1.0f));	//GPU VIEWPORT RANGE IS -1 to 1 in all axels, + y increasing towards top
	root_wnd.Gfx().DrawTestTriangle(timer.Peek(), root_wnd.input.mouse.GetPosX() / (900.0f * 0.5f) -1.0f, - root_wnd.input.mouse.GetPosY() / (600.0f * 0.5f) + 1.0f);
	root_wnd.Gfx().DrawTestTriangle(0, 0.0f, 0.5f);
	root_wnd.Gfx().EndFrame();
}
