#include "Scene.h"

#include "Engine/DeltaTime.h"
#include "Engine/Renderer2D.h"

#include "Entity.h"
#include "SceneRegistry.h"
#include "Game.h"

#include <iostream>

Scene::Scene(Game* game) noexcept
	: m_Game(game)
{
}

Scene::~Scene() noexcept
{
}

Entity Scene::CreateEntity() noexcept
{
	Entity entity(m_Registry.CreateEntity(), this);
	return entity;
}

void Scene::DestroyEntity(const Entity& entity) noexcept
{
	m_Registry.DestroyEntity(entity.GetID());
}

void Scene::OnBegin()
{
	Entity entity = CreateEntity();
	DestroyEntity(entity);
	entity = CreateEntity();

	SpriteRendererComponent* component = entity.AddComponent<SpriteRendererComponent>();

	component->Texture = m_Game->GetTextureLibrary().Load("Assets\\Textures\\Planks\\Planks012_4K_Color.png");
}

void Scene::OnUpdate(const DeltaTime& deltaT)
{
}

void Scene::OnRender()
{
	Renderer2D::RenderScene(this);
}

void Scene::OnEnd()
{
	for (EntityData& data : m_Registry.GetEntityData())
		m_Registry.DestroyEntity(data.EntityID);
}
