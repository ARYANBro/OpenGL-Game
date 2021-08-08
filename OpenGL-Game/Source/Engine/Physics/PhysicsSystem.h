#pragma once

#include <glm/glm.hpp>

class Scene;
class AABBColider;
class CircleColider;

enum class Direction
{
	Up, Down, Left, Right
};

struct CollisionInfo
{
	bool Colided;
	glm::vec2 Penetration;
	Direction Direction;	
};

class PhysicsSystem
{
public:
	static void Update(Scene& scene);

	static bool CheckAABBToAABB(const AABBColider& colider1, const AABBColider& colider);
	static CollisionInfo CheckCircleToAABB(const CircleColider& colider1, const AABBColider& colider2);
};
