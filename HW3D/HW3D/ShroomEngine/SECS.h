#pragma once
#include "Shroom.h"

#include <entt/entt.hpp>

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

#include <fstream>

//include headers that implement a archive in simple text format

//------------------------------------------
// Serialization (JSON)
//------------------------------------------
#include "ShroomSerialize.h"
#include "ShroomString.h"
//------------------------------------------

#include <string>

#include "ShroomArcane3D/ShroomArcane3D.h"

typedef entt::registry ENTT_ECS;

class Transform;

//ENTT SHROOM LAYER
//*******************************************
//SECS MODELLING CLASS MASK FOR ENTT
//*******************************************
class SECS : public ENTT_ECS
{
	friend class boost::serialization::access;

//*******************************************
//SECS
//*******************************************
public:
	ShroomArcaneGraphics& gfx;
	SECS(ShroomArcaneGraphics& gfx) : gfx(gfx) {};

//*******************************************
//SECS COMPONENTS
//*******************************************
public:
	class ComponentBase 
	{ 
		protected: bool INITIALIZED = false; 
    };
	class Component : ComponentBase
	{
	public:
		template <typename Type>
		struct IS_BASE {
			constexpr static bool check(SECS::ComponentBase*) { return true; }
			constexpr static bool check(...)   { return false; }

			enum { value = check(static_cast<Type*>(0)) };
		};
	
		void SetInitialized() { INITIALIZED = true; };
		bool IsInitialized()  { return INITIALIZED; };

		virtual void Serialize(SHROOM_JSON_WRITER& writer)
		{ throw std::exception("Component serializer not implemented"); };
		virtual void Deserialize(SHROOM_JSON_DOCUMENT_ENTRY entry)
		{ throw std::exception("Component deserializer not implemented");};
	};

//*******************************************
//SECS ENTITIES
//*******************************************
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

			new_shroom_entity.get()->entity = secs.create();
			new_shroom_entity.get()->entityName = name;

			//ADD TRANSFORM COMPONENT MANUALLY
			new_shroom_entity.get()->transform = &secs.assign<Transform>(new_shroom_entity.get()->entity);	
			//new_shroom_entity.get()->AssignComponent<Transform>(rng, adist, ddist, odist, rdist, 1);
			return secs.AddEntity(new_shroom_entity);
		}
		
		//Assing SECS variant, this will be only and primary when derive entt private
		template<typename AddComponent, typename... Args>
		AddComponent* AssignComponent(Args &&... args) 
		{
			if (secs_ref.has<AddComponent>(entity))
			{
				return GetComponent<AddComponent>();
			}

			if (SECS::Component::IS_BASE<AddComponent>::value)
			{
				components.push_back((Component*)&this->secs_ref.assign<AddComponent>(this->entity, std::forward<Args>(args)...));
				
				std::vector<std::string> nameSplit;
				ShroomString::Split(std::string(typeid(AddComponent).name()), nameSplit);
				
				if (nameSplit.size() == 2)
				{
					componentNames.push_back(nameSplit[1]);
				}

				return GetComponent<AddComponent>();
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
//*******************************************
//SECS SYSTEMS
//*******************************************
public:
	class System
	{
	public:
		System(SECS* pSECS) : pSECS(pSECS) {};

		virtual void Initialize()     = 0;
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

//*******************************************
//SECS SCENE
//*******************************************

public:
	struct Scene
	{
		struct Entity
		{
			std::string name;
		};

		struct System
		{
			std::string name;
		};

		std::vector<Scene::Entity> entities;
		std::vector<Scene::System> systems;

		static void Save(SECS& secs, std::filesystem::path savepath);
		static void Load(SECS& secs, std::filesystem::path savepath);
	};

//*******************************************
//SECS MODELING STORAGES
//*******************************************
private:
	std::vector<std::unique_ptr<System>> systems;
	std::vector<std::unique_ptr<Entity>> entities;

public:
	void ClearEntities()
	{
		for (size_t i = 0; i < entities.size(); i++)
		{
			entities[i].release();
		}
		entities.clear();
		this->clear();
	};
};
