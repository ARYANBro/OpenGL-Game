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
	auto transform =  entity.AddComponent<TransformComponent>();
	transform->Translation = { 0.0f, 0.0f, -1.0f };
	transform->Scale = glm::vec3(500.0f);
	return entity;
}

void Scene::DestroyEntity(const Entity& entity) noexcept
{
	m_Registry.DestroyEntity(entity.GetID());
}

void Scene::OnBegin()
{
	// Entity entity = CreateEntity();
	// SpriteRendererComponent* component = entity.AddComponent<SpriteRendererComponent>();

	// component->Texture = m_Game->GetTextureLibrary().Load("Assets\\Textures\\Planks\\Planks012_4K_Color.png");
}

void Scene::OnUpdate(const DeltaTime& deltaT)
{
}

void Scene::OnEnd()
{
	// m_Registry.EachEntity([this](EntityID entity)
	// {
		// m_Registry.DestroyEntity(entity);
	// });
}
