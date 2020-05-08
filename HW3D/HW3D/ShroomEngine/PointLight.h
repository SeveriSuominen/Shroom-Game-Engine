#pragma once
#pragma once

#include "ShroomArcane3D/ShroomArcane3D.h"
#include "ShroomArcane3D/Bindable.h"
#include <memory>;
#include <algorithm>;
#include <utility>;

#include "SECS.h"

#include "Sphere.h"

struct PointLight : SECS::Component
{
public:
	//----------------------------------
	//CONSTRUCT
	//----------------------------------
	PointLight() : model(Sphere::Make<Vertex>()),  Component() {}
	//----------------------------------

	//PointLight & operator=(const PointLight &) = delete;
	//PointLight() = default;

	//----------------------------------
	//NON COPY & MOVEABLE CONST
	//----------------------------------
	PointLight(PointLight &&other)
		: binds{ std::move(other.binds) },
		  model{ std::move(other.model) },
		Component()
	{}

	PointLight & operator=(PointLight &&other) {
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
};

