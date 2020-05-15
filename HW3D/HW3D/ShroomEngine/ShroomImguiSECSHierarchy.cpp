#include "App.h"
#include "ShroomImguiSECSHierarchy.h"
#include "ShroomImguiViewUtility.h"

#include "ShroomArcane3D/Transform.h"
#include "ShroomImguiSECSEntityEditor.h"

#include "PointLight.h"
#include <string>

void ShroomImguiSECSHierarchy::Draw(App * app, bool * open)
{
	static int clicked = -1;
	static bool contextMenuOpen;

	struct FUNCS
	{
		static void LetterMarker(std::string m, ImVec4 col = ImVec4(1.0f, 1.0, 1.0f, 1.0f))
		{
			ImGui::Text("[");
			ImGui::SameLine(0.0f, 0.0f);
			ImGui::TextColored(col, m.c_str());
			ImGui::SameLine(0.0f, 0.0f);
			ImGui::Text("]");
		}

		static void ContextMenu(App * app)
		{
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Add Entity"))
				{
					SECS::Entity::Create("New Entity", app->secs);
				}
				ImGui::EndPopup();
			}
		}

		static void ShowEntityComponents(App * app, std::unique_ptr<SECS::Entity>& entity, int& clicked, int uid)
		{
			ImGui::PushID(uid);                // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
			ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
			
			LetterMarker("E", ImVec4(0.5f, 0.7f, 1.0f, 1.0f));
			ImGui::SameLine();
			
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (clicked == uid ? ImGuiTreeNodeFlags_Selected : 0);
	
			//bool node_open = ImGui::TreeNode("Object", "%s_%u", entity.get()->entityName.c_str(), uid, node_flags);
			bool node_open = ImGui::TreeNodeEx("Object", node_flags, "%s_%u",  entity.get()->entityName.c_str(), uid);

			if (ImGui::IsItemClicked() || ImGui::IsItemFocused())
				clicked = uid;
			
			if (node_open)
			{
				for (size_t i = 0; i < entity.get()->components.size(); i++)
				{	
					ShroomImguiViewUtility::IconByComponentType(entity.get()->componentNames[i].c_str());
					ImGui::SameLine(0.0f, 0.0f);
					ImGui::PushItemWidth(50);
					ImGui::TextColored(ImVec4(1.0f, 0.95, 0.1f, 1.0f), entity.get()->componentNames[i].c_str());
					//ImGui::Separator();
				}

				if (ImGui::BeginMenu(ICON_FA_PLUS))
				{
					if (ImGui::MenuItem(ICON_FA_PEN " Mesh Renderer"))
					{
						entity.get()->AssignComponent<MeshRenderer>();
					}
				
					if (ImGui::MenuItem(ICON_FA_LIGHTBULB " Point Light"))
					{
						entity.get()->AssignComponent<PointLight>(app->root_wnd.Gfx());
					}
					ImGui::EndMenu();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
	};

	ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("SECS Hierarchy"))
	{
		ImGui::End();
		return;
	}

	if (contextMenuOpen && ImGui::GetIO().MouseClicked[1])
	{
		contextMenuOpen = false;
	}

	if (!contextMenuOpen && ImGui::GetIO().MouseClicked[1])
	{
		contextMenuOpen = true;
	}

	if (contextMenuOpen)
	{	
		FUNCS::ContextMenu(app);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	ImGui::Columns(1);

	for (size_t i = 0; i < app->secs.GetEntitiesSize(); i++)
		FUNCS::ShowEntityComponents(app, app->secs.GetEntity(i), clicked, i);

	ImGui::PopStyleVar();
	ImGui::End();

	if (clicked != -1) 
	{
		if( ! (ShroomImguiSECSEntityEditor::DrawEditor(app, app->secs.GetEntity(clicked).get())))
			clicked = -1;

		if (ImGui::GetIO().MouseClicked[1])
		{
			clicked = -1;
		}
	}
}
