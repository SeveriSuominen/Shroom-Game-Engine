#include "App.h"
#include "ShroomImguiSECSHierarchy.h"
#include "ShroomImguiViewUtility.h"

#include "ShroomArcane3D/Transform.h"

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
				ImGui::Text("TERVE");
				if (ImGui::MenuItem("Add Entity"))
				{
					SECS::Entity::Create("New Entity", app->secs);
				}
				ImGui::EndPopup();
			}
		}

		static void ShowEntityComponents(std::unique_ptr<SECS::Entity>& entity, int& clicked, int uid)
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
					ImGui::Separator();
					
				}

				if (ImGui::BeginMenu(ICON_FA_PLUS))
				{
					if (ImGui::MenuItem("MeshRenderer")) 
					{
						entity.get()->AssignComponent<MeshRenderer>();
					}
					ImGui::EndMenu();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}

		static void ShowEntityEditor(std::unique_ptr<SECS::Entity>& entity)
		{
			auto name = entity.get()->entityName.c_str();
			std::stringstream ss;
			ss << "SECS Entity Editor";
			
			ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
			if (!ImGui::Begin(ss.str().c_str()))
			{
				ImGui::End();
				return;
			}

			//ENTITY EXISTING?
			if (entity.get() == nullptr)
			{
				clicked = -1;
				ImGui::End(); //REMEMBER TO END
				return;
			}

			//TRANSFORM
			auto transform = entity.get()->GetComponent<Transform>();

			auto size = ImGui::GetWindowSize();

			ImGui::BeginChild("too big" /*ImVec2(size.x -15, 260),true*/);
			ImGui::GetFont()->DisplayOffset.y = 0;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

			ShroomImguiViewUtility::IconByComponentType("Transform");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.3f, 1), "Transform");
			ImGui::Text("");

			ImGui::Columns(2);

			ImGui::TextColored(ImVec4(1.0f, 0.8f, 0, 1), "Position");
			ImGui::SliderFloat("X", &transform->pos.x, -100, 100);
			ImGui::SliderFloat("Y", &transform->pos.y, -100, 100);
			ImGui::SliderFloat("Z", &transform->pos.z, -100, 100);
	
			ImGui::NextColumn();

			ImGui::TextColored(ImVec4(1.0f, 0.8f, 0, 1), "Rotation");
			ImGui::SliderFloat("RX", &transform->roll, -180, 180);
			ImGui::SliderFloat("RY", &transform->pitch, -180, 180);
			ImGui::SliderFloat("RZ", &transform->yaw, -180, 180);
			
			ImGui::TextColored(ImVec4(1.0f, 0.8f, 0, 1), "Rotation over Pivot");
			ImGui::SliderFloat("T", &transform->theta, -180, 180);
			ImGui::SliderFloat("P", &transform->phi, -180, 180);
			ImGui::SliderFloat("C", &transform->chi, -180, 180);
	
			ImGui::PopStyleVar();
			ImGui::EndChild();

			//ImGui::EndChild();
			ImGui::End();
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
	{	//MessageBox(nullptr, "terve", "yolonki", MB_OK);
		FUNCS::ContextMenu(app);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	ImGui::Columns(1);
	//ImGui::Separator();

	for (size_t i = 0; i < app->secs.GetEntitiesSize(); i++)
		FUNCS::ShowEntityComponents(app->secs.GetEntity(i), clicked, i);
	//ImGui::Columns(1);
	//ImGui::Separator();
	ImGui::PopStyleVar();
	ImGui::End();

	if (clicked != -1) 
	{
		FUNCS::ShowEntityEditor(app->secs.GetEntity(clicked));

		if (ImGui::GetIO().MouseClicked[1])
		{
			clicked = -1;
		}
	}
}
