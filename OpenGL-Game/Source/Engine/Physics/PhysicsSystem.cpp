#include "PhysicsSystem.h"

#include "PhysicsColider.h"

#include <Scene/Components.h>
#include <Engine/Application.h>
#include <Scene/Scene.h>

static Direction GetNormalDirection(glm::vec2 target) noexcept
{
	static constexpr std::array<glm::vec3, 4> s_Compass = {
		glm::vec3{ 0.0f, -1.0f, 0.0f },
		glm::vec3{ 0.0f, 1.0f, 0.0f  },
		glm::vec3{ -1.0f, 0.0f, 0.0f },
		glm::vec3{ 1.0f, 0.0f, 0.0f  }
	};

	float max = 0.0f;
	unsigned int directionIndex = -1;
	for (unsigned int i = 0; i < s_Compass.size(); i++)
	{
		float dotProduct = glm::dot(glm::normalize(target), static_cast<glm::vec2>(s_Compass[i]));
		if (dotProduct > max)
		{
			max = dotProduct;
			directionIndex = i;
		}
	}

	return static_cast<Direction>(directionIndex);
}

void PhysicsSystem::Update(Scene& scene)
{
	scene.GetRegistry().EachComponent<PhysicsComponent>([&](EntityID kineticEntity, PhysicsComponent& physics)
	{
		if (physics.PhysicsType == PhysicsType::Kinetic)
		{
			scene.GetRegistry().EachComponent<PhysicsComponent>([&](EntityID staticEntity, PhysicsComponent& pc)
			{
				bool collisionOccured = false;

				if (staticEntity != kineticEntity)
				{
					std::unique_ptr<CollisionInfo> ci = nullptr;

					if (physics.Colider->GetType() == ColiderType::AABBColider)
					{
						if (pc.Colider->GetType() == ColiderType::AABBColider)
							collisionOccured = CheckAABBToAABB(physics.GetColider<AABBColider>(), pc.GetColider<AABBColider>());
						else if (pc.Colider->GetType() == ColiderType::CircleColider)
						{
							ci = std::make_unique<CollisionInfo>(CheckCircleToAABB(pc.GetColider<CircleColider>(), physics.GetColider<AABBColider>()));
							collisionOccured = ci->Colided;
						}
					}

					if (physics.Colider->GetType() == ColiderType::CircleColider)
					{
						if (pc.Colider->GetType() == ColiderType::AABBColider)
						{
							ci = std::make_unique<CollisionInfo>(CheckCircleToAABB(physics.GetColider<CircleColider>(), pc.GetColider<AABBColider>()));
							collisionOccured = ci->Colided;
						}
						else if (pc.Colider->GetType() == ColiderType::CircleColider)
						{
							// not implemented
						}
					}

					if (collisionOccured)
					{
						PhysicsEvent event(scene.GetEntity(kineticEntity), scene.GetEntity(staticEntity), ci.get());
						Application::GetInstance().OnEvent(event);
					}
				}
			});
		}
	});
}

bool PhysicsSystem::CheckAABBToAABB(const AABBColider& colider1, const AABBColider& colider2)
{
	glm::vec2 colider1TopLeft = colider1.GetPosition();
	glm::vec2 colider1BottomRight = colider1.GetPosition() + colider1.GetScale(); 

	glm::vec2 colider2TopLeft = colider2.GetPosition();
	glm::vec2 colider2BottomRight = colider2.GetPosition() + colider2.GetScale();

	bool xColision = colider1BottomRight.x >= colider2TopLeft.x && colider2BottomRight.x >= colider1TopLeft.x;
	bool yColision = colider1BottomRight.y >= colider2TopLeft.y && colider2BottomRight.y >= colider1TopLeft.y;

	return xColision && yColision;
}

CollisionInfo PhysicsSystem::CheckCircleToAABB(const CircleColider& colider1, const AABBColider& colider2)
{
	glm::vec2 circleCenter = colider1.GetPosition() + colider1.GetRadius();

	glm::vec2 aabbHalfExtents = colider2.GetScale() / 2.0f;
	glm::vec2 aabbCenter = colider2.GetPosition() + aabbHalfExtents;

	glm::vec2 difference = circleCenter - aabbCenter;
	glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);
	glm::vec2 closest = aabbCenter + clamped;

	difference = closest - circleCenter;

	CollisionInfo ci;
	ci.Colided = glm::length(difference) <= colider1.GetRadius();
	ci.Penetration = glm::length(difference) < colider1.GetRadius() ? colider1.GetRadius() - glm::abs(difference) : glm::vec2(0.0f);
	ci.Direction = GetNormalDirection(difference);

	return ci;
}