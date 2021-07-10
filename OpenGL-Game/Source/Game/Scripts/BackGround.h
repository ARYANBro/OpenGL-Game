#pragma once

#include <Engine/Script.h>
#include <Scene/Entity.h>
#include <Scene/Components.h>
#include <Engine/Application.h>

class BackGround : public Script
{
public:
	virtual void OnUpdate(Entity entity, const DeltaTime& dt) override
	{
		float right = Application::GetInstance().GetWindow().GetWidth();
		float top = Application::GetInstance().GetWindow().GetHeight();

		glm::mat4 projection = glm::ortho(0.0f, right, 0.0f, top);

		auto transform = entity.GetComponent<TransformComponent>();
		transform->Scale = { right, top, 0.0f };
	}
};
