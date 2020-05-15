#pragma once

#include "ShroomArcane3D/ShroomArcane3D.h"
#include <DirectXMath.h>

class TransformEditor
{
public:
	static void EditTransformMatrix(ShroomArcaneGraphics& gfx, Transform& transform)
	{
		DirectX::XMMATRIX& matrix = transform.GetTransformXM();

		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		if (ImGui::IsKeyPressed(90))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(69))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(82)) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;

		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents((float*)&matrix, matrixTranslation, matrixRotation, matrixScale);

		ImGui::InputFloat3("Tr", matrixTranslation, 3);
		ImGui::InputFloat3("Rt", matrixRotation, 3);
		ImGui::InputFloat3("Sc", matrixScale, 3);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, (float*)&matrix);

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}
		static bool useSnap(false);
		if (ImGui::IsKeyPressed(83))
			useSnap = !useSnap;
		ImGui::Checkbox("", &useSnap);
		ImGui::SameLine();
		//vec_t snap;

		float snap;

		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			snap = 0.5f;
			ImGui::InputFloat3("Snap", &snap);
			break;
		case ImGuizmo::ROTATE:
			snap = 0.5f;
			ImGui::InputFloat("Angle Snap", &snap);
			break;
		case ImGuizmo::SCALE:
			snap = 0.5f;
			ImGui::InputFloat("Scale Snap", &snap);
			break;
		}
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

		const auto cam_matrix = gfx.camera.GetMatrix();
		const auto projection_matrix = gfx.GetProjection();

		ImGuizmo::Manipulate((float*)&cam_matrix, (float*)&projection_matrix, mCurrentGizmoOperation, mCurrentGizmoMode, (float*)&matrix, NULL, useSnap ? &snap : NULL);

		//DECOMPOSE
		float DmatrixTranslation[3], DmatrixRotation[3], DmatrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents((float*)&matrix, DmatrixTranslation, DmatrixRotation, DmatrixScale);
	}
};