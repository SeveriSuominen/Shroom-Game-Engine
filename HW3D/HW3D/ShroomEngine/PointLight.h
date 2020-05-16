#pragma once
#pragma once

#include "ShroomArcane3D/ShroomArcane3D.h"
#include "ShroomArcane3D/Bindable.h"
#include <memory>;
#include <algorithm>;
#include <utility>;

#include "SECS.h"

#include "Sphere.h"
#include "ShroomArcane3D/Imgui/imgui.h"

#include "ShroomSerialize.h"

struct PointLight : SECS::Component
{
public:
	//----------------------------------
	//CONSTRUCT
	//----------------------------------
	PointLight(ShroomArcaneGraphics& gfx) : model(Sphere::Make<Vertex>()), cbuf(gfx), Component() {}
	//----------------------------------

	//PointLight & operator=(const PointLight &) = delete;
	//PointLight() = default;

	void SpawnControlWindow() noexcept
	{
		/*if (ImGui::Begin("Light"))
		{
			ImGui::Text("Position");
			ImGui::SliderFloat("X", &pos.x, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Y", &pos.y, -60.0f, 60.0f, "%.1f");
			ImGui::SliderFloat("Z", &pos.z, -60.0f, 60.0f, "%.1f");
			if (ImGui::Button("Reset"))
			{
				//Reset();
			}
		}
		ImGui::End();*/
	}
	//DirectX::XMFLOAT3 pos = { 5000.0f,0.0f,0.0f };

	//----------------------------------
	//NON COPY & MOVEABLE CONST
	//----------------------------------
	PointLight(PointLight &&other)
		: binds{ std::move(other.binds) },
		  model{ std::move(other.model) },
		  cbuf{ std::move(other.cbuf) },
		  Component()
	{}

	PointLight & operator=(PointLight &&other) {
		auto tmp{ std::move(other) };
		std::swap(binds, other.binds);
		std::swap(model, other.model);
		std::swap(cbuf,  other.cbuf);
		return *this;
	}
	//----------------------------------

	IndexedTriangleList<Vertex> model;
	std::vector<std::unique_ptr<Bindable>> binds;

	void AddBind(std::unique_ptr<Bindable> bind) noexcept
	{
		binds.push_back(std::move(bind));
	};

	const class IndexBuffer* pIndexBuffer = nullptr;

	void AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
	{
		assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get();
		binds.push_back(std::move(ibuf));
	}

	struct PSLightConstants
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float  diffuseIntensity = 1.0f;
		float  attConst = 1.0f;    //Light constant aka enviromental light
		float  attLin   = 0.045f;   //Light linear scaling by range
		float  attQuad  = 0.0075f; // Light exponential scaling by range

		//float padding;		
	};

	mutable PixelConstantBuffer<PSLightConstants> cbuf;


	//Light color
	float  diffuseColor[4] = { 1.0f, 0.5f, 1.0f, 1.0f };
	float  diffuseIntensity = 1.0f;

	float  attConst = 1.0f; 
	float  attLin   = 0.045f; 
	float  attQuad  = 0.0075f; 

	//-----------------------------------------------
	// SERIALIZE
	//-----------------------------------------------
	void Serialize(SHROOM_JSON_WRITER& writer) override
	{
		writer.Key("Name");
		writer.String("PointLight");
		
		writer.Key("DiffuseColor");
		writer.StartArray();
		writer.Double(diffuseColor[0]);
		writer.Double(diffuseColor[1]);
		writer.Double(diffuseColor[2]);
		writer.Double(diffuseColor[3]);
		writer.EndArray();

		writer.Key("DiffuseIntensity");
		writer.Double(diffuseIntensity);
		
		writer.Key("AttCon");
		writer.Double(attConst);
		writer.Key("AttLin");
		writer.Double(attLin);
		writer.Key("AttQua");
		writer.Double(attQuad);
	};

	void Deserialize(SHROOM_JSON_DOCUMENT_ENTRY entry) override
	{		 
		diffuseColor[0] = entry["DiffuseColor"][0].GetDouble();
		diffuseColor[1] = entry["DiffuseColor"][1].GetDouble();
		diffuseColor[2] = entry["DiffuseColor"][2].GetDouble();
		diffuseColor[3] = entry["DiffuseColor"][3].GetDouble();
		
		diffuseIntensity = entry["DiffuseIntensity"].GetDouble();

		attConst = entry["AttCon"].GetDouble();
		attLin   = entry["AttLin"].GetDouble();
		attQuad  = entry["AttQua"].GetDouble();
	};
	//-----------------------------------------------
};

