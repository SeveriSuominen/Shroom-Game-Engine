#include "App.h"
#include "ShroomImguiCameraControl.h"

void ShroomImguiCameraControl::Draw(App * app, bool * open)
{
	auto& camera = app->root_wnd.Gfx().camera;

	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("R", &camera.r, 0.1f, 80.0f, "%.1f");
		ImGui::SliderAngle("Theta", &camera.theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &camera.phi, -89.0f, 89.0f);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &camera.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &camera.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &camera.yaw, -180.0f, 180.0f);
		
		if (ImGui::Button("Reset"))
		{
			camera.Reset();
		}
		ImGui::End();
	}
}
