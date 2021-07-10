#pragma once

#include <cstdint>

#include "Scene.h"
#include "SceneRegistry.h"

class Entity
{
public:
	Entity(std::uint_fast32_t entityID, Scene* scene) noexcept;

	template<typename ComponentType>
	ComponentType* AddComponent() const
	{
		return m_Scene->m_Registry.AddComponent<ComponentType>(m_EntityID);
	}

	template<typename ComponentType>
	bool HasComponent() const noexcept
	{
		return m_Scene->m_Registry.HasComponent<ComponentType>(m_EntityID);
	}

	template<typename ComponentType>
	void RemoveComponent() const
	{
		m_Scene->m_Registry.RemoveComponent<ComponentType>(m_EntityID);
	}

	template<typename ComponentType>
	ComponentType* GetComponent() const
	{
		return m_Scene->m_Registry.GetComponent<ComponentType>(m_EntityID);
	}

	std::uint_fast32_t GetID() const noexcept { return m_EntityID; }
	
	Scene* GetScene() const noexcept { return m_Scene; }

private:
	std::uint_fast32_t m_EntityID;
	Scene* m_Scene;
};
