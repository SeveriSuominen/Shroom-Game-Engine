#pragma once

#include "ShroomArcane3D/ShroomArcane3D.h"
#include "ShroomArcane3D/Bindable.h"
#include <memory>;
#include <algorithm>;
#include <utility>;

#include "SECS.h"

struct MeshRenderer : SECS::Component
{
public:
	//----------------------------------
	//CONSTRUCT
	//----------------------------------
	MeshRenderer
	(
		IndexedTriangleList<Vertex> model,
		int test
	)
	: model(model), test(test), Component() {}
	//----------------------------------

	int test = 0;

	//MeshRenderer & operator=(const MeshRenderer &) = delete;
	//MeshRenderer() = default;

	//----------------------------------
	//NON COPY & MOVEABLE CONST
	//----------------------------------
	MeshRenderer(MeshRenderer &&other)
		: binds{ std::move(other.binds) },
		  model{ std::move(other.model) },
		  test { std::move(other.test) },
		  Component()
	{}

	MeshRenderer & operator=(MeshRenderer &&other) {
		auto tmp{ std::move(other) };
		std::swap(binds, other.binds);
		std::swap(model, other.model);
		std::swap(test,  other.test);
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
};

