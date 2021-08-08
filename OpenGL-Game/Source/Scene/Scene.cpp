#include "Scene.h"

#include "Entity.h"
#include "SceneRegistry.h"

#include <Engine/DeltaTime.h>
#include <Engine/Script.h>
#include <Engine/Renderer2D.h>

#include <iostream>

Scene::Scene() noexcept
{
}

Scene::~Scene() noexcept
{
}

Entity Scene::CreateEntity() noexcept
{
	Entity entity(m_Registry.CreateEntity(), this);

	auto transform =  entity.AddComponent<TransformComponent>();
	transform->Translation = { 0.0f, 0.0f, 0.0f };
	transform->Scale = glm::vec3(500.0f);

	auto tag = entity.AddComponent<TagComponent>();
	tag->Tag = "Entity";

	return entity;
}

void Scene::DestroyEntity(const Entity& entity) noexcept
{
	m_Registry.DestroyEntity(entity.GetID());
}

Entity Scene::GetEntity(EntityID entityID) noexcept
{
	return Entity(entityID, this);
}

void Scene::OnBegin()
{
	m_Registry.EachComponent<ScriptComponent>([this](EntityID entity, ScriptComponent& script)
	{
 		script.Script->OnBegin(Entity(entity, this));
	});
};

void Scene::OnUpdate(const DeltaTime& deltaT)
{
	m_Registry.EachComponent<ScriptComponent>([this, dt = deltaT](EntityID entity, ScriptComponent& script)
	{
		script.Script->OnUpdate(Entity(entity, this), dt);
	});
}

void Scene::OnEvent(const Event& event)
{
	m_Registry.EachComponent<ScriptComponent>([&, this](EntityID entity, ScriptComponent& script)
	{
		script.Script->OnEvent(Entity(entity, this), event);
	});
}

void Scene::OnEnd()
{
	m_Registry.EachComponent<ScriptComponent>([this](EntityID entity, ScriptComponent& script)
	{
		script.Script->OnEnd(Entity(entity, this));
	});

	m_Registry.EachEntity([this](EntityID entityID)
	{
		DestroyEntity(GetEntity(entityID));
	});
}
