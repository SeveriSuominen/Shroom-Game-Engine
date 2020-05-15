#pragma once

#include "ShroomImguiView.h"
#include "TransformEditor.h"
#include "ShroomArcane3D/ShroomArcaneCamera.h"

#include "PointLight.h"

#include "App.h"

class ShroomImguiSECSEntityEditor
{
public:
	static bool DrawEditor(App* app, SECS::Entity* entity)
	{
		if (ImGui::Begin(ICON_FA_ROBOT " SECS Entity"))
		{
			//ENTITY EXISTING?
			if (entity == nullptr)
			{
				ImGui::End(); //REMEMBER TO END
				return false;
			}
			//-------------------------------------------------------------
			// Transform
			//-------------------------------------------------------------
			ShroomImguiViewUtility::IconByComponentType("Transform");
			ImGui::Separator();

			auto transform = entity->GetComponent<Transform>();
			TransformEditor::EditTransformMatrix(app->root_wnd.Gfx(), *transform);
	
			//-------------------------------------------------------------
			// Light
			//-------------------------------------------------------------
			auto pointlight = entity->GetComponent<PointLight>();
			if (pointlight != nullptr)
			{
				ImGui::Separator();
				ShroomImguiViewUtility::IconByComponentType("PointLight");
				ImGui::Separator();

				ImGui::ColorPicker4("##picker", (float*)&pointlight->diffuseColor, ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);

				ImGui::SliderFloat("Intensity", &pointlight->diffuseIntensity, 0, 3);

				ImGui::SliderFloat("Const", &pointlight->attConst, 0, 5);
				ImGui::SliderFloat("Linear", &pointlight->attLin, 0, 5);
				ImGui::SliderFloat("Quad", &pointlight->attQuad, 0, 5);
			}
			ImGui::End();
		}
		return true;
	}
};