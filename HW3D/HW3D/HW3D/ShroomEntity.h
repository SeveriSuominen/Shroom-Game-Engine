#pragma once
#include "SECS.h"
#include <random>

#include "ShroomArcane3D/Transform.h"

class ShroomEntity 
{
public:
	entt::entity Get; //GETTER

	static ShroomEntity Create(SECS& secs)
	{
		ShroomEntity new_shroom_entity = ShroomEntity();

		//TEMP
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
		std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
		std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
		std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

		new_shroom_entity.Get = secs.create();
		secs.assign<Transform>(new_shroom_entity.Get, rng, adist, ddist, odist, rdist, 3);

		return new_shroom_entity;
	}
private:
	ShroomEntity(){};
};

