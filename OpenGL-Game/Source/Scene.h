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

	Game* GetGame() const noexcept { return m_Game; }

private:
	friend class Entity;
	friend class Renderer2D;
	friend class SceneSerializer;

private:
	SceneRegistry m_Registry;

	Game* m_Game;
};
