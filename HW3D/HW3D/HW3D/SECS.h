#pragma once
#include "Shroom.h"

#include <entt/entt.hpp>
//#include <entt/entity/registry.hpp>
#include <vector>
#include <sstream>

typedef entt::registry ENTT_ECS;

class SECS : public ENTT_ECS
{
public:
	class System
	{
	public:
		System(SECS* pSECS) : pSECS(pSECS) {};

		virtual void Initialize() = 0;
		virtual void Update(float dt) = 0;
	private:
		SECS* pSECS;
	};
	//API
public:
	//SECS() : systems(std::vector<System>()), ENTT_ECS() {};

	template<class SystemT>
	inline SystemT* AddSystem(SystemT* system)
	{
		systems.push_back(system);
		return (SystemT*)systems.back();
	}

	void Initialize()
	{
		std::stringstream ss;
		ss << systems.size();

		
		for (size_t i = 0; i < systems.size(); i++)
		{
			systems[i]->Initialize();
		}
	}

	void Update(float dt)
	{
		for (size_t i = 0; i < systems.size(); i++)
		{
			systems[i]->Update(dt);
		}
	}
private:
	std::vector<System*> systems;
};
