#pragma once

#include <Scene/Entity.h>

class DeltaTime;
class Event;

class Script
{
public:
	Script() noexcept = default;

	virtual void OnBegin(Entity entity) {}
	virtual void OnUpdate(Entity entity, const DeltaTime& dt) {}
	virtual void OnEvent(Entity entity, const Event& event) {}
	virtual void OnEnd(Entity entity) {}
};