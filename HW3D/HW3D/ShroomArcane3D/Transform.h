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

		pos.x = 0;
		pos.y = 0;
		pos.z = 0;

		droll  = ddist(rng);
		dpitch = ddist(rng);
		dyaw   = ddist(rng);
		dphi   = odist(rng);
		dtheta = odist(rng);
		dchi   = odist(rng);
		//chi    = adist(rng);
		//theta  = adist(rng);
		//phi    = adist(rng);
	}

	DirectX::XMMATRIX GetTransformXM() const noexcept
	{
		return  DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			    DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z) *
			    DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
	}

	int test;

	DirectX::XMFLOAT3 pos;

	//SERIALIZE
	void Serialize(Writer<StringBuffer>& writer) 
	{
		writer.Key("Name");
		writer.String("Transform");

		writer.Key("Pos X");
		writer.Double(pos.x);
		writer.Key("Pos Y");
		writer.Double(pos.y);
		writer.Key("Pos Z");
		writer.Double(pos.z);

		writer.Key("Roll");
		writer.Double(roll);
		writer.Key("Pitch");
		writer.Double(pitch);
		writer.Key("Yaw");
		writer.Double(yaw);

		writer.Key("Theta");
		writer.Double(theta);
		writer.Key("Phi");
		writer.Double(phi);
		writer.Key("Chi");
		writer.Double(chi);
	}

	virtual void Deserialize(rapidjson::Value& json)
	{
		if (json.IsObject())
		{
			pos.x = json["Pos X"].GetDouble();
			pos.y = json["Pos Y"].GetDouble();
			pos.z = json["Pos Z"].GetDouble();

			roll  = json["Roll"] .GetDouble();
			pitch = json["Pitch"].GetDouble();
			yaw   = json["Yaw"]  .GetDouble();

			theta = json["Theta"].GetDouble();
			phi   = json["Phi"]  .GetDouble();
			chi   = json["Chi"]  .GetDouble();
		}
		else
		{
			throw std::exception("yolonki");
		}
	};

	// positional
	float r;

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
	float dchi;
};