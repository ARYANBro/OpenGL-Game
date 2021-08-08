#pragma once

#include <Engine/Script.h>
#include <Engine/Input.h>
#include <Engine/Application.h>
#include <Engine/DeltaTime.h>
#include <Scene/Entity.h>

#include <GLFW/glfw3.h>

class PlayerPaddle : public Script
{
public:
	PlayerPaddle(SpriteRendererComponent* spriteRenderer, float size, float speed) noexcept
		: m_SpriteRenderer(spriteRenderer), m_PxSize(Application::GetInstance().GetWindow().GetWidth() / size), m_Speed(speed)
	{
	}

	virtual void OnBegin(Entity entity) override
	{
		m_PaddleRatio = static_cast<float>(m_SpriteRenderer->Texture->GetHeight()) / m_SpriteRenderer->Texture->GetWidth();

		float windowWidth = Application::GetInstance().GetWindow().GetWidth();
		float windowHeight = Application::GetInstance().GetWindow().GetHeight();

		auto transform = entity.GetComponent<TransformComponent>();

		float scale = windowWidth / m_PxSize;
		transform->Scale = { scale, m_PaddleRatio * scale, 0.0f };

		transform->Translation.x = (windowWidth / 2.0f - transform->Scale.x / 2.0f);
		transform->Translation.y = (windowHeight - transform->Scale.y);
	}

	virtual void OnUpdate(Entity entity, const DeltaTime& dt) override
	{
		auto transform = entity.GetComponent<TransformComponent>();

		float speed = m_Speed;

		if (Input::IsKeyPressed(GLFW_KEY_A))
			transform->Translation.x -= speed * dt;
		else if (Input::IsKeyPressed(GLFW_KEY_D))
			transform->Translation.x += speed * dt;

		transform->Translation.x = std::clamp(transform->Translation.x, 0.0f, static_cast<float>(Application::GetInstance().GetWindow().GetWidth() - transform->Scale.x));
	}

private:
	SpriteRendererComponent* m_SpriteRenderer;
	float m_PaddleRatio;
	float m_PxSize;
	float m_Speed;
};