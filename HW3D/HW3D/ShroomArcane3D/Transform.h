#pragma once

//#include "ShroomArcane3D/ShroomArcane3D.h"
#include <DirectXMath.h>
#include <random>

//------------------------------------------
// JSON
//------------------------------------------
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include "rapidjson/writer.h"

using namespace rapidjson;
//------------------------------------------

//IMGUIZMO
#include "ImGuizmo\ImGuizmo.h"

struct Transform
{
public:
	Transform() 
	{
		//TEMP
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
		std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
		std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
		std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

		//TEMP
		//pos.z = rdist(rng);

	  /*pos.x = 0;
		pos.y = 0;
		pos.z = 0;*/

		/*droll  = ddist(rng);
		dpitch = ddist(rng);
		dyaw   = ddist(rng);
		dphi   = odist(rng);
		dtheta = odist(rng);
		dchi   = odist(rng);*/
		//chi    = adist(rng);
		//theta  = adist(rng);
		//phi    = adist(rng);

		transformMatrix =
			DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0) *
			DirectX::XMMatrixTranslation(0, 0, 0) *
			DirectX::XMMatrixRotationRollPitchYaw(1, 1, 1);
		
		float DmatrixTranslation[3] = { 0.0f, 0.0f, 0.0f };
		float DmatrixRotation[3]    = { 0.0f, 0.0f, 0.0f };
		float DmatrixScale[3]       = { 1.0f, 1.0f, 1.0f };
		
		DecomposeMatrix
		(
			DmatrixTranslation,
			DmatrixRotation,
			DmatrixScale
		);
	}

	//TRANSFORM!!
	mutable DirectX::XMMATRIX transformMatrix;

	//DX
	DirectX::XMMATRIX& GetTransformXM() const noexcept
	{
		/*transformMatrix = 
			DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) *
			DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);*/

		return transformMatrix;
	}

	void RecomposeMatrix(float* translation, float* rotation, float* scale)
	{
		ImGuizmo::RecomposeMatrixFromComponents
		(
			translation,
			rotation,
			scale,
			(float*)&transformMatrix  //<<< MATRIX
		);
	}

	void DecomposeMatrix(float* translation, float* rotation, float* scale)
	{
		ImGuizmo::DecomposeMatrixToComponents
		(
			(float*)&transformMatrix, //<<< MATRIX
			translation,
			rotation,
			scale
		);
	}

	//SERIALIZE
	void Serialize(Writer<StringBuffer>& writer) 
	{
		writer.Key("Name");
		writer.String("Transform");

		float DmatrixTranslation[3], DmatrixRotation[3], DmatrixScale[3];
		DecomposeMatrix
		(
			DmatrixTranslation,
			DmatrixRotation,
			DmatrixScale
		);

		writer.Key("Pos X");
		writer.Double(DmatrixTranslation[0]);
		writer.Key("Pos Y");
		writer.Double(DmatrixTranslation[1]);
		writer.Key("Pos Z");
		writer.Double(DmatrixTranslation[2]);

		writer.Key("Roll");
		writer.Double(DmatrixRotation[0]);
		writer.Key("Pitch");
		writer.Double(DmatrixRotation[1]);
		writer.Key("Yaw");
		writer.Double(DmatrixRotation[2]);

		writer.Key("Scale X");
		writer.Double(DmatrixScale[0]);
		writer.Key("Scale Y");
		writer.Double(DmatrixScale[1]);
		writer.Key("Scale Z");
		writer.Double(DmatrixScale[2]);
	}

	virtual void Deserialize(rapidjson::Value& json)
	{
		if (json.IsObject())
		{
			float RmatrixTranslation[3] = 
			{ 
				json["Pos X"].GetDouble(),
				json["Pos Y"].GetDouble(),
				json["Pos Z"].GetDouble()		
			};

			float RmatrixRotation[3] = 
			{ 
				json["Roll"] .GetDouble(),
				json["Pitch"].GetDouble(),
				json["Yaw"]  .GetDouble()
			};

			float RmatrixScale[3] = 
			{ 
				json["Scale X"].GetDouble(),
				json["Scale Y"].GetDouble(),
				json["Scale Z"].GetDouble(),
			};

			RecomposeMatrix
			(
				RmatrixTranslation,
				RmatrixRotation,
				RmatrixScale
			);
		}
		else
		{
			throw std::exception("yolo");
		}
	};

	// positional
	/*float r;

	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta = 0.0f;
	float phi = 0.0f;
	float chi = 0.0f;

	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;*/
};