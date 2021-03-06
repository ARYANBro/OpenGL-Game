#pragma once

#include "ComponentPool.h"
#include <Defines.h>  

#include <bitset>
#include <stdexcept>
#include <vector>
#include <functional>

class Scene;

class ClassIDGenerator
{
public:
	static std::int_fast32_t GetID() noexcept { return s_ID++; }
	static void Reset() noexcept { s_ID = 0; }

private:
	static inline std::int_fast32_t s_ID = 0;
};

template<typename ClassType>
class GetID
{
public:
	static std::int_fast32_t Get()  noexcept { return s_ID; }

private:
	static inline std::int_fast32_t s_ID = ClassIDGenerator::GetID();
};

using ComponentMask = std::bitset<Defines::maxComponents>;
using EntityID = std::uint_fast32_t;

struct EntityData
{
	EntityID EntityID;
	ComponentMask Components;
};

class SceneRegistry
{
public:
	~SceneRegistry() noexcept;

	EntityID CreateEntity() noexcept;
	void DestroyEntity(EntityID entity) noexcept;

	bool IsEntityValid(EntityID entity) const noexcept;

	const std::vector<EntityData>& GetEntityData() const noexcept { return m_EntitiesData; }
	std::vector<EntityData>& GetEntityData() noexcept { return m_EntitiesData; }

	template<typename ComponentType>
	ComponentType* AddComponent(EntityID entity);

	template<typename ComponentType>
	bool HasComponent(EntityID entity) const noexcept;

	template<typename ComponentType>
	ComponentType* GetComponent(EntityID entity) const;
	
	template<typename ComponentType>
	void RemoveComponent(EntityID entity);

	template<typename ComponentType>
	void EachComponent(const std::function<void (EntityID,  ComponentType&)>& function);

	void EachEntity(const std::function<void (EntityID)>& function);

private:
	std::vector<EntityData> m_EntitiesData;
	std::vector<EntityID> m_FreedEntities;
	std::vector<ComponentPool*> m_ComponentPools;
};

template<typename ComponentType>
ComponentType* SceneRegistry::AddComponent(EntityID entity)
{
	if (!IsEntityValid(entity))
		throw std::runtime_error("Entity is not valid");

	if (HasComponent<ComponentType>(entity))
		throw std::runtime_error("Entity already has the component");

	std::int_fast32_t id = GetID<ComponentType>::Get();

	m_EntitiesData[entity].Components.set(id);
	if (m_ComponentPools.size() <= id)
		m_ComponentPools.resize(id + 1, nullptr);
		
	if (m_ComponentPools[id] == nullptr)
		m_ComponentPools[id] = new ComponentPool(sizeof(ComponentType));

	return m_ComponentPools[id]->Add<ComponentType>(entity);
}

template<typename ComponentType>
bool SceneRegistry::HasComponent(EntityID entity) const noexcept
{
	return m_EntitiesData[entity].Components.test(GetID<ComponentType>::Get());
}

template<typename ComponentType>
ComponentType* SceneRegistry::GetComponent(EntityID entity) const
{
	if (!IsEntityValid(entity))
		throw std::runtime_error("Entity is not valid");
	else if (!HasComponent<ComponentType>(entity))
		return nullptr;

	auto id = GetID<ComponentType>::Get();
	void* component = m_ComponentPools[id]->Get(entity);
	return reinterpret_cast<ComponentType*>(component);
}

template<typename ComponentType>
void SceneRegistry::RemoveComponent(EntityID entity)
{
	if (!HasComponent<ComponentType>(entity))
		throw std::runtime_error("Entity does not has the component");

	auto id = GetID<ComponentType>::Get();
	m_EntitiesData[entity].Components.reset(id);

	m_ComponentPools[id]->Remove(entity);
}

template<typename ComponentType>
void SceneRegistry::EachComponent(const std::function<void (EntityID, ComponentType&)>& function)
{
	auto id = GetID<ComponentType>::Get();

	if (m_ComponentPools.size() <= id)
		throw std::runtime_error("Component does not exist");

	for (EntityData& data : m_EntitiesData)
	{
		if (HasComponent<ComponentType>(data.EntityID) && IsEntityValid(data.EntityID))
		{
			ComponentType* component = reinterpret_cast<ComponentType*>(m_ComponentPools[id]->Get(data.EntityID));
			function(data.EntityID, *component);
		}
	}
}
