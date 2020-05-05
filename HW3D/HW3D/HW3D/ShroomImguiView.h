#pragma once

//IMGUI
#include "ShroomArcane3D/imgui/imgui.h"
#include "ShroomArcane3D/imgui/imgui_impl_dx11.h"
#include "ShroomArcane3D/imgui/imgui_impl_win32.h"

//FORWARD DEC
class App;

class ShroomImguiView
{
public:
	enum Category
	{
		All		  = 0u,
		General   = 1u,
		Rendering = 2u,
		Network   = 3u,
	};

	ShroomImguiView(Category category) : category(category) {}
	Category category;

	virtual void Draw(App* app, bool* open) = 0;
};
