#pragma once

#include "Shroom.h"
#include "ShroomWindow.h"
#include "ShroomTimer.h"
#include <vector>

#include "SECS.h"

#include "Renderer.h";
#include "ShroomArcane3D/TextureRenderer.h"

//IMGUI
#include "ShroomInguiManager.h"

class App
{
public:
	    App();
	int Go();

	//IMGUI manager
	ShroomImguiManager imgui;

	//MAIN WINDOW
	Window  root_wnd;
	
	//SECS ECS
	SECS secs;
	
	//TEMP ECS SYSTEM;
	Renderer renderer;
private:
	void  DoFrame();
	void  AddCubes(int amount);
	void  RemoveCubes(int amount);
	float frameDelta;

	std::vector<std::unique_ptr<class Renderer>> boxes;
	std::vector<std::unique_ptr<class TextureRenderer>> tboxes;

private:
	ShroomTimer timer;
};
