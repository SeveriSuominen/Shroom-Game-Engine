#pragma once
#include "Shroom.h"

#include <entt/entt.hpp>
//#include <entt/entity/registry.hpp>
#include <vector>
#include <sstream>
#include <memory>
#include <random>
#include <utility>

#include <cstddef>
#include <iterator>
#include <algorithm>
#include <type_traits>
#include <cstring>
#include "Sphere.h"

#include "ShroomArcane3D/Transform.h"
#include "ShroomArcane3D/Components.h"

typedef entt::registry ENTT_ECS;

//ENTT SHROOM LAYER

class Transform;

class SECS : public ENTT_ECS
{

public:
	ShroomArcaneGraphics& gfx;

	SECS(ShroomArcaneGraphics& gfx) : gfx(gfx) {};

	class ComponentBase {};
	class Component : ComponentBase
	{
	public:
		template <typename Type>
		struct IS_BASE {
			constexpr static bool check(SECS::ComponentBase*) { return true; }
			constexpr static bool check(...)   { return false; }

			enum { value = check(static_cast<Type*>(0)) };
		};
	};
public:
	class Entity
	{

	private:
		SECS& secs_ref;
	public:
		Entity(SECS& secs) : secs_ref(secs) {};
	   ~Entity() {};

	    //Transform
	    Transform* transform = nullptr;

	    std::vector<Component*>  components;
		std::vector<std::string> componentNames;

		std::string  entityName;
		entt::entity entity; //GETTER

		template<typename ComponentType>
		ComponentType* GetComponent()
		{
			return &secs_ref.get<ComponentType>(entity);
		}

		static std::unique_ptr<Entity>& Create(const std::string name, SECS& secs)
		{
			auto new_shroom_entity = std::make_unique<Entity>(secs);

			//TEMP
			std::mt19937 rng(std::random_device{}());
			std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
			std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
			std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
			std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
		
			new_shroom_entity.get()->entity = secs.create();
			new_shroom_entity.get()->entityName = name;

			//ADD TRANSFORM COMPONENT MANUALLY
			new_shroom_entity.get()->transform = &secs.assign<Transform>
			(   
				new_shroom_entity.get()->entity, 
				rng, adist, ddist, odist, rdist, 1
			);	
			//new_shroom_entity.get()->AssignComponent<Transform>(rng, adist, ddist, odist, rdist, 1);
			return secs.AddEntity(new_shroom_entity);
		}
		
		//Assing SECS variant, this will be only and primary when derive entt private
		template<typename AddComponent, typename... Args>
		AddComponent* AssignComponent(Args &&... args) 
		{
			if (SECS::Component::IS_BASE<AddComponent>::value)
			{
				components.push_back((Component*)&this->secs_ref.assign<AddComponent>(this->entity, std::forward<Args>(args)...));
				componentNames.push_back(std::string(typeid(AddComponent).name()));

				return (AddComponent*)&components.back();
			}
			else if (typeid(Transform) == typeid(AddComponent) /*SPECIAL CASE!!!*/)
			{
				//WE DONT LET ASSING OR REMOVE TRANSFORM COMPONENT
				return (AddComponent*)transform;
			}
			else
			{
				//SECS EXCEPTION HERE
				return nullptr;
			}
		}

		//Assing SECS variant, this will be only and primary when derive entt private
		template<typename RemoveComponent>
		bool RemoveComponent()
		{			
			if (SECS::Component::IS_BASE<RemoveComponent>::value /*NOTICE!: CANT REMOVE TRANSFORM COMPONENT*/)
			{	 //WE DONT LET ASSING OR REMOVE TRANSFORM COMPONENT
				for (size_t i = 0; i < components.size(); i++)
				{
					if (typeid(components[i]) == typeid(RemoveComponent))
					{
						components.erase(components.begin() + i);
					}	
				}
				&this->secs_ref.remove<RemoveComponent>(this->entity);
				return true;
			}
			else
			{
				return false;
			}
		}
	};

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
	template<class SystemT>
	inline SystemT* AddSystem()
	{	
		systems.push_back(std::make_unique<SystemT>(gfx, *this));
		return (SystemT*)systems.back().get();
	}

	void Initialize()
	{
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

	std::unique_ptr<Entity>& AddEntity(std::unique_ptr<Entity>& entity)
	{
		entities.push_back(std::move(entity));
		return entities.back();
	}

	std::unique_ptr<Entity>& GetEntity(int i)
	{
		return entities[i];
	}

	size_t GetEntitiesSize()
	{
		return entities.size();
	}

private:
	std::vector<std::unique_ptr<System>> systems;
	std::vector<std::unique_ptr<Entity>> entities;
};
