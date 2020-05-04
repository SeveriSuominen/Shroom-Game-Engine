#include "ShroomInguiManager.h"
#include "ShroomArcane3D/imgui/imgui.h"

ShroomImguiManager::ShroomImguiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ShroomImguiManager::~ShroomImguiManager()
{
	ImGui::DestroyContext();
}