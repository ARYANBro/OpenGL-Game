#include "SceneRegistry.h"

#include "Scene.h"

#include <iostream>

SceneRegistry::~SceneRegistry() noexcept
{
	for (auto& edata : m_EntitiesData)
		DestroyEntity(edata.EntityID);

	for (int i = 0; i < m_ComponentPools.size(); i++)
		delete m_ComponentPools[i];
	}

EntityID SceneRegistry::CreateEntity() noexcept
{
	if (m_FreedEntities.empty())
	{
		EntityID entity = m_EntitiesData.size();
		m_EntitiesData.push_back({ entity, ComponentMask() });
		
		return entity;
	}
	else
	{
		EntityID entity = m_FreedEntities.back();
		m_FreedEntities.pop_back();
		return entity;
	}
}

void SceneRegistry::DestroyEntity(EntityID entity) noexcept
{
	for (auto& pool : m_ComponentPools)
	{
		if (pool->Contains(entity))
			pool->Remove(entity);
	}

	m_FreedEntities.push_back(entity);
}

bool SceneRegistry::IsEntityValid(EntityID entity) const noexcept
{
	auto found = std::find_if(m_FreedEntities.begin(), m_FreedEntities.end(), [&entity](EntityID e)
	{
		return e == entity;
	});

	return found == m_FreedEntities.end() && entity != -1;
}

void SceneRegistry::EachEntity(const std::function<void (EntityID)>& function)
{
	for (EntityData& data : m_EntitiesData)
		function(data.EntityID);
}
