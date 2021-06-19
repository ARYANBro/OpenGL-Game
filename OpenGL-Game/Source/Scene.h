#pragma once

#include "Engine/Texture.h"

#include "Components.h"
#include "ComponentPool.h"
#include "SceneRegistry.h"

class DeltaTime;
class Game;

class Scene
{
public:
	Scene(Game* game) noexcept;
	~Scene() noexcept;

	Entity CreateEntity() noexcept;
	void DestroyEntity(const Entity& entity) noexcept;

	void OnBegin();
	void OnUpdate(const DeltaTime& deltaT);
	void OnEnd();

private:
	friend class Entity;
	friend class Renderer2D;

private:
	SceneRegistry m_Registry;

	Game* m_Game;
};
