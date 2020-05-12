#pragma once

#include "ShroomArcane3D/ShroomArcane3D.h"
#include "ShroomArcane3D/Bindable.h"
#include <memory>;
#include <algorithm>;
#include <utility>;

#include "ShroomSerialize.h"

#include "SECS.h"
#include "Cube.h"

struct MeshRenderer : SECS::Component
{
public:
	//----------------------------------
	//CONSTRUCT
	//----------------------------------
	MeshRenderer() : Component() 
	{
		model = Cube::MakeIndependent<Vertex>();
	}
	//----------------------------------

	//MeshRenderer & operator=(const MeshRenderer &) = delete;
	//MeshRenderer() = default;

	//----------------------------------
	//NON COPY & MOVEABLE CONST
	//----------------------------------
	MeshRenderer(MeshRenderer &&other)
		: binds{ std::move(other.binds) },
		  model{ std::move(other.model) },
		  Component()
	{}

	MeshRenderer & operator=(MeshRenderer &&other) {
		auto tmp{ std::move(other) };
		std::swap(binds, other.binds);
		std::swap(model, other.model);
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

	//-----------------------------------------------
	// SERIALIZE
	//-----------------------------------------------
	void Serialize(SHROOM_JSON_WRITER& writer) override
	{
		writer.Key("Name");
		writer.String("MeshRenderer");

		writer.Key("PathToModel");
		writer.String("yolonki");
	}

	void Deserialize(SHROOM_JSON_DOCUMENT_ENTRY entry) override
	{
		//TEMP, NOT IMPLEMENTED YET
		model = Cube::MakeIndependent<Vertex>();
	}
	//-----------------------------------------------
};

