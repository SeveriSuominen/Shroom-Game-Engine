#pragma once

#include "Shroom.h"
#include "ShroomWindow.h"
#include "ShroomTimer.h"
#include <vector>
#include "ShroomArcane3D/Box.h";

class App
{
public:
	    App();
	int Go();

private:
	void  DoFrame();
	void  AddCubes(int amount);
	void  RemoveCubes(int amount);
	float frameDelta;
	std::vector<std::unique_ptr<class Box>> boxes;

private:
	ShroomTimer timer;
	Window   root_wnd;
};
