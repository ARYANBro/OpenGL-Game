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

		float speed = m_Speed * m_WindowSpeedRatio;

		if (Input::IsKeyPressed(GLFW_KEY_A))
			transform->Translation.x -= speed * dt;
		else if (Input::IsKeyPressed(GLFW_KEY_D))
			transform->Translation.x += speed * dt;

		transform->Translation.x = std::clamp(transform->Translation.x, 0.0f, static_cast<float>(Application::GetInstance().GetWindow().GetWidth() - transform->Scale.x));
	}

	virtual void OnEvent(Entity entity, const Event& event) override
	{
		if (event.GetType() == EventType::WindowResizeEvent)
		{
			const WindowResizeEvent& windowE = static_cast<const WindowResizeEvent&>(event);
			MaintainScale(entity, windowE.GetWidth(), windowE.GetHeight());
		}
	}

	virtual void OnEnd(Entity entity) override
	{
		Script::OnEnd(entity);
	}

	void MaintainScale(Entity entity, std::uint_fast32_t windowWidth, std::uint_fast32_t windowHeight) noexcept
	{
		auto transform = entity.GetComponent<TransformComponent>();
		float scale = windowWidth / m_PxSize;
		transform->Scale = { scale, m_PaddleRatio * scale, 0.0f };

		float posRatio = transform->Translation.x / m_PrevWindowSize;

		transform->Translation.x = posRatio * windowWidth;
		transform->Translation.y += (windowHeight - transform->Scale.y) - transform->Translation.y;

		m_WindowSpeedRatio += (windowWidth - m_PrevWindowSize) / m_InitialWindowWidth;
		m_WindowSpeedRatio = std::abs(m_WindowSpeedRatio);
		m_PrevWindowSize = windowWidth;
	}

private:
	SpriteRendererComponent* m_SpriteRenderer;
	float m_PaddleRatio;
	float m_WindowSpeedRatio = 1.0f;
	float m_PrevWindowSize = Application::GetInstance().GetWindow().GetWidth();
	const float m_InitialWindowWidth = Application::GetInstance().GetWindow().GetWidth();
	float m_PxSize;
	float m_Speed;
};