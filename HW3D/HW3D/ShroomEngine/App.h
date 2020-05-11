#pragma once

#include "Shroom.h"
#include "ShroomWindow.h"
#include "ShroomTimer.h"
#include <vector>

#include "SECS.h"

#include "Renderer.h";
#include "LightBinder.h"

#include "ShroomArcane3D/TextureRenderer.h"

//IMGUI
#include "ShroomArcane3D/ShroomArcaneImguiManager.h"

//Shroom imgui views in here
#include "ShroomImguiContainer.h"


#include "ShroomArcane3D/Surface.h"
#include "ShroomArcane3D/GDIPlusManager.h"


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

	//GDIPlusManager needs to be initialised to use Surface
	GDIPlusManager gdipm;

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
