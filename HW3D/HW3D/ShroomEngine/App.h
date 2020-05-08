#pragma once

#include "Shroom.h"
#include "ShroomWindow.h"
#include "ShroomTimer.h"
#include <vector>

#include "SECS.h"

#include "Renderer.h";
#include "SolidRenderer.h"

#include "ShroomArcane3D/TextureRenderer.h"

//IMGUI
#include "ShroomArcane3D/ShroomArcaneImguiManager.h"

//Shroom imgui views in here
#include "ShroomImguiContainer.h"

class App
{
public:
	    App();
	int Go();

	//IMGUI manager
	ShroomArcaneImguiManager imgui;
	ShroomImguiContainer	 imguiContainer;

	//MAIN WINDOW
	Window  root_wnd;
	
	//SECS ECS
	SECS secs;
	
	//TEMP ECS SYSTEM;
	Renderer renderer;
	SolidRenderer solid_renderer;

public:

	;

private:
	void  DoFrame();
	void  DrawImguiViews();
	void  AddCubes(int amount);
	void  RemoveCubes(int amount);
	float frameDelta;

	std::vector<std::unique_ptr<class Renderer>> boxes;
	std::vector<std::unique_ptr<class TextureRenderer>> tboxes;

private:
	ShroomTimer timer;
};
