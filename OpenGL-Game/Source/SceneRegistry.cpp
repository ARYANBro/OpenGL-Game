#include "SceneRegistry.h"

#include "Scene.h"

SceneRegistry::~SceneRegistry() noexcept
{
	for (ComponentPool* componentPool : m_ComponentPools)
		delete componentPool;
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
	for (int i = 0; i < MAX_COMPONENTS; i++)
	{
		if (m_EntitiesData[entity].Components.test(i))
			m_ComponentPools[i]->Remove(entity);
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
