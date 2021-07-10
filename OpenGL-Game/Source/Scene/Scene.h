#pragma once

#include "Components.h"
#include "ComponentPool.h"
#include "SceneRegistry.h"

class DeltaTime;
class BlockAligner;
class Event;

class Scene
{
public:
	Scene() noexcept;
	~Scene() noexcept;

	Entity CreateEntity() noexcept;
	void DestroyEntity(const Entity& entity) noexcept;

	void OnBegin();
	void OnUpdate(const DeltaTime& deltaT);
	void OnEvent(const Event& event);
	void OnEnd();

	SceneRegistry& GetRegistry() noexcept { return m_Registry; }

private:
	friend class Entity;
	friend class Renderer2D;
	friend class SceneSerializer;

private:
	SceneRegistry m_Registry;
};
