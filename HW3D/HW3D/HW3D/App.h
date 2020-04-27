#pragma once

#include "Shroom.h"
#include "ShroomWindow.h"
#include "ShroomTimer.h"

class App
{
public:
	    App();
	int Go();

private:
	void  DoFrame();
	float frameDelta;

private:
	ShroomTimer timer;
	Window   root_wnd;
};
