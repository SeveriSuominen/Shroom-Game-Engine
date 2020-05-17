#pragma once

#include <map>
#include <boost\filesystem.hpp>

#include "App.h"

#include "imgui/IconsFontAwesome5.h"

class ShroomAssetResolver
{
public:
	static void ResolveAssetEditor(App* app, boost::filesystem::path path, std::string& type)
	{
		std::map<std::string, unsigned int> asset_type_map
		{
			{".shroom_mat", 0}
		};

		std::map<std::string, unsigned int>::iterator it = asset_type_map.find(type);
		unsigned int type_id;
		bool found = false;

		if (it != asset_type_map.end())
		{
			type_id = it->second;
			found = true;
		}

		std::stringstream ss;
		ss << type_id;

		if (found)
		{
			if (type_id == asset_type_map[".shroom_mat"])
			{
				MaterialEditor(app, path);
				return;
			}
		}
	}

private:
	static void MaterialEditor(App* app, boost::filesystem::path path)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		if (ImGui::Begin("Shroom Material"))
		{
			imnodes::BeginNodeEditor();

			imnodes::BeginNode(1);

			imnodes::BeginOutputAttribute(0);
			// in between Begin|EndAttribute calls, you can call ImGui
			// UI functions
			ImGui::Text(ICON_FA_MAP);
			ImGui::Dummy(ImVec2(80.0f, 45.0f));
			imnodes::EndAttribute();
			imnodes::EndNode();

			imnodes::BeginNode(2);
			//ImGui::Dummy(ImVec2(80.0f, 45.0f));
			imnodes::BeginInputAttribute(1);
			// in between Begin|EndAttribute calls, you can call ImGui
			// UI functions
			ImGui::Text(ICON_FA_MAP);
			ImGui::Dummy(ImVec2(80.0f, 45.0f));
			imnodes::EndAttribute();
			imnodes::EndNode();


			imnodes::EndNodeEditor();

			ImGui::End();
		}
		ImGui::PopStyleVar();
	}
};