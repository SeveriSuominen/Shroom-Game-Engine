#include "App.h"
#include "ShroomImguiSECSHierarchy.h"
#include "ShroomImguiViewUtility.h"

void ShroomImguiSECSHierarchy::Draw(App * app, bool * open)
{
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

		static void ShowDummyObject(std::unique_ptr<SECS::Entity>& entity, int uid)
		{
			ImGui::PushID(uid);                      // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
			ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
			
			LetterMarker("E", ImVec4(0.5f, 0.7f, 1.0f, 1.0f));
			ImGui::SameLine();
			
			bool node_open = ImGui::TreeNode("Object", "%s_%u", entity.get()->entityName.c_str(), uid);

			/*ImGui::NextColumn();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("my sailor is rich");
			ImGui::NextColumn();*/
			
			if (node_open)
			{
				for (size_t i = 0; i < entity.get()->components.size(); i++)
				{
					/*if (i == 0)
					{
						LetterMarker("C", ImVec4(0.0f, 0.95, 0.3f, 1.0f));
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(0.0f, 0.95, 0.1f, 1.0f), entity.get()->componentNames[i].c_str());
						continue;
					}*/
				
					ShroomImguiViewUtility::IconByComponentType(entity.get()->componentNames[i].c_str());
					ImGui::SameLine(0.0f, 0.0f);
					ImGui::PushItemWidth(50);
					ImGui::TextColored(ImVec4(1.0f, 0.95, 0.1f, 1.0f), entity.get()->componentNames[i].c_str());
					ImGui::Separator();
					
				}
				static float dummy_members[8] = { 0.0f,0.0f,1.0f,3.1416f,100.0f,999.0f };

				/*for (int i = 0; i < 8; i++)
				{
					ImGui::PushID(i); // Use field index as identifier.
					if (i < 2)
					{
						ShowDummyObject("Child", 424242);
					}
					else
					{
						// Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
						ImGui::AlignTextToFramePadding();
						ImGui::TreeNodeEx("Field", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, "Field_%d", i);
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						if (i >= 5)
							ImGui::InputFloat("##value", &dummy_members[i], 1.0f);
						else
							ImGui::DragFloat("##value", &dummy_members[i], 0.01f);
						ImGui::PopItemWidth();
						ImGui::NextColumn();
					}
					ImGui::PopID();
				}*/
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

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	ImGui::Columns(1);
	//ImGui::Separator();
	
	for (size_t i = 0; i < app->secs.GetEntitiesSize(); i++)
		FUNCS::ShowDummyObject(app->secs.GetEntity(i), i);

	//ImGui::Columns(1);
	//ImGui::Separator();
	ImGui::PopStyleVar();
	ImGui::End();
}
