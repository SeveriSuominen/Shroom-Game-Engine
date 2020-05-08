#pragma once
#include <boost\filesystem.hpp>
#include "imgui/IconsFontAwesome5.h"

//IMGUI
#include "ShroomArcane3D/imgui/imgui.h"
#include "ShroomArcane3D/imgui/imgui_impl_dx11.h"
#include "ShroomArcane3D/imgui/imgui_impl_win32.h"


#include <vector>
#include <tuple>

typedef std::tuple<std::string, std::string, ImVec4> EXTENSION_ICON;

class ShroomImguiViewUtility
{
public:
	
	static void IconByFileExtension(std::string exc)
	{
		
		auto types = std::vector<EXTENSION_ICON>();
	
		types.push_back
		(
			EXTENSION_ICON(".shroom_material", ICON_FA_PEN " ", ImVec4(0.0f, 0.95f, 0.2f, 1.0f))		
		);

		types.push_back
		(
			EXTENSION_ICON(".lua", ICON_FA_CODE " ", ImVec4(0.95f, 0.0f, 0.55f, 1.0f))
		);
		
		if (exc == "dir")
		{
			ImGui::TextColored(ImVec4(0.9f, 0.6f, 0, 1), ICON_FA_FOLDER " ");
			ImGui::SameLine(0.0f, 0.0f);
			return;
		}

		for (size_t i = 0; i < types.size(); i++)
		{
			if (exc == std::get<0>(types[i]))
			{
				ImGui::TextColored(std::get<2>(types[i]), std::get<1>(types[i]).c_str());
				ImGui::SameLine(0.0f, 0.0f);
				
				return;
			}
		}
		ImGui::TextColored(ImVec4(1,1,1,1), ICON_FA_FILE " ");
		ImGui::SameLine(0.0f, 0.0f);
	}

	static void IconByComponentType(std::string exc)
	{

		auto types = std::vector<EXTENSION_ICON>();

		types.push_back
		(
			EXTENSION_ICON("struct Transform", ICON_FA_ARROWS_ALT " ", ImVec4(0.0f, 0.95f, 0.35f , 1.0f))
		);

		types.push_back
		(
			EXTENSION_ICON("struct PointLight",  ICON_FA_LIGHTBULB  " ", ImVec4(0.95f, 0.75f, 0.0f, 1.0f))
		);

		types.push_back
		(
			EXTENSION_ICON("struct MeshRenderer",  ICON_FA_DRAW_POLYGON " ", ImVec4(0.0f, 0.35f, 0.95f, 1.0f))
		);

		if (exc == "dir")
		{
			ImGui::TextColored(ImVec4(0.9f, 0.6f, 0, 1), ICON_FA_FOLDER " ");
			ImGui::SameLine(0.0f, 0.0f);
			return;
		}

		for (size_t i = 0; i < types.size(); i++)
		{
			if (exc == std::get<0>(types[i]))
			{
				ImGui::TextColored(std::get<2>(types[i]), std::get<1>(types[i]).c_str());
				

				return;
			}
		}
		ImGui::TextColored(ImVec4(1, 1, 1, 1), ICON_FA_FILE " ");
	}
};