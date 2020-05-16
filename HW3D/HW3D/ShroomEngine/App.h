#pragma once

#include "Shroom.h"
#include "ShroomWindow.h"
#include "ShroomTimer.h"
#include <vector>

#include "SECS.h"

#include "Renderer.h";
#include "LightBinder.h"

#include "ShroomApplicationCache.h"

//IMGUI
#include "ShroomArcane3D/ShroomArcaneImguiManager.h"

//NODE EDITOR
#include "ShroomNodeEditor.h"

//Shroom imgui views in here
#include "ShroomImguiContainer.h"

#include "ShroomArcane3D/Surface.h"
#include "ShroomArcane3D/GDIPlusManager.h"


class App
{
public:
	    App();
	int Go();

	//APP CACHE
	ShroomApplicationCache cache;

	//IMGUI manager
	ShroomArcaneImguiManager imgui;
	ShroomImguiContainer	 imguiContainer;

	//MAIN WINDOW
	Window  root_wnd;
	
	//SECS ECS
	SECS secs;

	//GDIPlusManager needs to be initialised to use Surface
	GDIPlusManager gdipm;

	//Nodes
	ShroomNodeEditor nodeEditor;

private:
	void  DoFrame();
	void  DrawImguiViews();
	void  AddCubes(int amount);
	float frameDelta;

private:
	ShroomTimer timer;
};
