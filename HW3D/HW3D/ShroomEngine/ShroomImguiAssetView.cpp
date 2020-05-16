#include "ShroomImguiAssetView.h"
#include <functional>
#include <filesystem>

#include "ShroomAssets.h"

#include <sstream>

#include <boost\filesystem.hpp>
#include "imgui/IconsFontAwesome5.h"
#include "ShroomImguiViewUtility.h"

void ShroomImguiAssetView::Draw(App * app, bool * open)
{
	ImGui::SetNextWindowSize(ImVec2(730, 250), ImGuiCond_FirstUseEver);
	ImGui::Begin("Shroom Assets", nullptr,  ImGuiWindowFlags_NoCollapse);

	static int selection_mask = (1 << 2);
	static int file_selected  = -1;
	
	int        node_clicked   = -1;
	
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize());

	std::vector<boost::filesystem::path> selected_directory_paths;

	std::function<void(const boost::filesystem::path &, unsigned int, unsigned int &, bool renderFiles)> functor = [&]
	(
		const boost::filesystem::path &path,
		unsigned int depth, unsigned int &idx, 
		bool renderFiles
	) 
	{
		for (auto &&p : boost::filesystem::directory_iterator(path)) {
			ImGuiTreeNodeFlags node_flags = 
				ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
				((selection_mask & (1 << idx)) ? ImGuiTreeNodeFlags_Selected : 0);

			if (boost::filesystem::is_directory(p.path())) {
				using namespace std::string_literals;

				std::string s = " "s + p.path().filename().string().c_str();

				if ((selection_mask & (1 << idx)))
					selected_directory_paths.push_back(p);

				if (ImGui::TreeNodeEx((void *)(intptr_t)idx, node_flags, "%s", s.c_str())) {
					if (ImGui::IsItemClicked() || ImGui::IsItemFocused())
						node_clicked = idx;
					functor(p, depth + 1, ++idx, renderFiles);
					ImGui::TreePop();
				}
			}
			else if(renderFiles){
				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				if (depth > 0) {
					ImGui::Indent();
				}
				ImGui::TreeNodeEx((void *)(intptr_t)idx, node_flags, "%s",
					p.path().filename().string().c_str());
				if (ImGui::IsItemClicked() || ImGui::IsItemFocused())
					node_clicked = idx;
				if (depth > 0) {
					ImGui::Unindent();
				}
			}
			++idx;
		}
		depth -= 1;
	};
	unsigned int idx = 0u; 

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	
	ImGui::Columns(2);

	functor(ShroomAssets::Path(), 0u, idx, false);

	if (node_clicked != -1) {
		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		if (ImGui::GetIO().KeyCtrl)
			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
			selection_mask  = (1 << node_clicked);           // Click to single-select
	}

	ImGui::NextColumn();

	auto directories = std::vector<boost::filesystem::path>();
	auto files = std::vector<boost::filesystem::path>();

	for (auto path : selected_directory_paths)
	{
		for (const auto & entry : boost::filesystem::directory_iterator(path))
		{
			if (boost::filesystem::is_directory(entry.path()))
				directories.push_back(entry.path());
			else
				files.push_back(entry.path());
		}
	}

	for (auto path : directories)
	{
		ShroomImguiViewUtility::IconByFileExtension("dir");
		ImGui::Text(path.filename().string().c_str());
	}

	int file_i = 0;

	for (auto path : files)
	{
		if (file_selected == file_i)
		{
			ShroomAssets::ResolveAsset(path);
		}

		ShroomImguiViewUtility::IconByFileExtension(path.filename().extension().string());
		ImGui::Selectable(path.filename().string().c_str(), file_selected == file_i);

		/*ImGui::TreeNodeEx
		(
			(void *)(intptr_t)0,
			0, 
			"%s", 
			path.filename().string().c_str()
		);*/

		//ImGui::TreePop();

		if (ImGui::IsItemClicked())
			file_selected = file_i;
		file_i++;
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::End();
}
