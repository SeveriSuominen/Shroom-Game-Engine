#include "ShroomArcaneImguiManager.h"
#include "imgui/imgui.h"

ShroomArcaneImguiManager::ShroomArcaneImguiManager()
{
	//IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ShroomArcaneImguiManager::~ShroomArcaneImguiManager()
{
	ImGui::DestroyContext();
}