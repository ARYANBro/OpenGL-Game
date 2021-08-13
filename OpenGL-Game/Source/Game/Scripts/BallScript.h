#pragma once

#include <Engine/Script.h>
#include <Engine/DeltaTime.h>

#include <glm/glm.hpp>

class BallScript : public Script
{
public:
	BallScript(SpriteRendererComponent* spriteRenderer, float size) noexcept
		: m_SpriteRenderer(spriteRenderer), m_Size(size)
	{
	}

	virtual void OnBegin(Entity entity) override
	{
		m_Velocity = s_InitialVelocity;
		m_Transform = entity.GetComponent<TransformComponent>();
		auto windowWidth = Application::GetInstance().GetWindow().GetWidth();
		auto windowHeight = Application::GetInstance().GetWindow().GetHeight();

		m_Transform->Scale = { m_Size, m_SpriteRenderer->Texture->GetHeight() / static_cast<float>(m_SpriteRenderer->Texture->GetWidth()) * m_Size, 1.0f };
		m_TexRatio = static_cast<float>(m_SpriteRenderer->Texture->GetHeight()) / m_SpriteRenderer->Texture->GetWidth();
		m_Scale = Application::GetInstance().GetWindow().GetWidth() / m_Size;

		m_Transform->Translation.x = (windowWidth / 2.0f - m_Transform->Scale.x / 2.0f);
		m_Transform->Translation.y = (windowHeight - m_Transform->Scale.y) - 100.0f;
	}


	virtual void OnUpdate(Entity entity, const DeltaTime& dt) override
	{
		float fDt = dt;
		glm::vec3 d = glm::vec3(m_Velocity, 0.0f) * fDt;

		m_Transform->Translation.x += d.x;
		m_Transform->Translation.y += d.y;
		m_Transform->Translation.z += d.z;

		auto width = Application::GetInstance().GetWindow().GetWidth();
		auto height = Application::GetInstance().GetWindow().GetHeight();

		m_Transform->Translation.x = std::clamp(m_Transform->Translation.x, 0.0f, width - m_Transform->Scale.x);
		m_Transform->Translation.y = std::max(m_Transform->Translation.y, 0.0f);

		bool atRightBound = m_Transform->Translation.x == width - m_Transform->Scale.x;
		bool atLeftBound = m_Transform->Translation.x == 0.0f;
		bool atUpBound = m_Transform->Translation.y == 0.0f;

		if (atRightBound || atLeftBound || atUpBound)
		{
			glm::vec3 normal;
			if (atRightBound)
				normal = glm::vec3(-1.0f, 0.0f, 0.0f);
			else if (atLeftBound)
				normal = glm::vec3(1.0f, 0.0f, 0.0f);
			else if (atUpBound)
				normal = glm::vec3(0.0f, 1.0f, 0.0f);

			glm::vec3 reflected = glm::reflect(glm::vec3(m_Velocity, 0.0f), normal);
			m_Velocity = static_cast<glm::vec2>(reflected);
		}
	}

	virtual void OnEvent(Entity entity, const Event& event) override
	{
		if (event.GetType() == EventType::PhysicsEvent)
		{
			const PhysicsEvent& e = static_cast<const PhysicsEvent&>(event);
			Entity firstEntity = e.GetFirstEntity();
			Entity secondEntity = e.GetSecondEntity();

			auto tag = secondEntity.GetComponent<TagComponent>();
			CollisionInfo* ci = e.GetCollisionInfo();
			if (ci != nullptr)
			{
				if (firstEntity.GetID() == entity.GetID())
				{
					auto colider = firstEntity.GetComponent<PhysicsComponent>()->GetColider<CircleColider>();
				
					if (tag->Tag == "Player")
					{
						auto paddleTc = secondEntity.GetComponent<TransformComponent>();
						float paddleCenter = paddleTc->Translation.x + paddleTc->Scale.x / 2.0f;
						float distToBall = (m_Transform->Translation.x + colider.GetRadius()) - paddleCenter;
						float percentage = distToBall / (paddleTc->Scale.x / 2.0f);

						float strength = 1.0f;
						glm::vec2 prevVelocity = m_Velocity;
						m_Velocity.x = s_InitialVelocity.x * percentage * strength;
						m_Velocity.y = -std::abs(m_Velocity.y);
						m_Velocity = glm::normalize(m_Velocity) * glm::length(prevVelocity);
					}
					else
					{
						switch (ci->Direction)
						{
							case Direction::Up:
								m_Transform->Translation.y += ci->Penetration.y;
								break;
							
							case Direction::Down:
								m_Transform->Translation.y -= ci->Penetration.y;
								break;

							case Direction::Left:
								m_Transform->Translation.x += ci->Penetration.x;
								break;
							
							case Direction::Right:
								m_Transform->Translation.x -= ci->Penetration.x;
						}


						if (ci->Direction == Direction::Up || ci->Direction == Direction::Down)
						{
							if (ci->Direction == Direction::Up)
								m_Velocity = glm::reflect(m_Velocity, glm::vec2(0.0f, -1.0f));
							else
								m_Velocity = glm::reflect(m_Velocity, glm::vec2(0.0f, 1.0f));
						}
						else if (ci->Direction == Direction::Left || ci->Direction == Direction::Right)
						{
							if (ci->Direction == Direction::Left)
								m_Velocity = glm::reflect(m_Velocity, glm::vec2(-1.0f, 0.0f));
							else
								m_Velocity = glm::reflect(m_Velocity, glm::vec2(1.0f, 0.0f));
						}
					}
				}
			}
		}
	}

	glm::vec2 GetVelocity() const noexcept { return m_Velocity; }
	

private:
	TransformComponent* m_Transform;
	SpriteRendererComponent* m_SpriteRenderer;
	glm::vec2 m_Velocity = s_InitialVelocity;

	constexpr static glm::vec2 s_InitialVelocity = glm::vec2(350.0f, -350.0f);

	float m_TexRatio;
	float m_Scale;
	float m_Size;
};