#pragma once

#include <glm/glm.hpp>

enum class ColiderType
{
	AABBColider, CircleColider
};

class Colider
{
public:
	Colider() noexcept = default;
	virtual ~Colider() noexcept = default;

	virtual ColiderType GetType() const noexcept = 0;
};

class AABBColider : public Colider
{
public:
	AABBColider() noexcept
		: m_Position(0.0f), m_Scale(0.0f) {}

	AABBColider(glm::vec2 position, glm::vec2 scale) noexcept
		: m_Position(position), m_Scale(scale) {}

	virtual ColiderType GetType() const noexcept override { return ColiderType::AABBColider; }

	void SetPosition(glm::vec2 position) noexcept { m_Position = position; }
	void SetScale(glm::vec2 scale) noexcept { m_Scale = scale; }

	glm::vec2 GetPosition() const noexcept { return m_Position; }
	glm::vec2 GetScale() const noexcept { return m_Scale; }

private:
	glm::vec2 m_Position;
	glm::vec2 m_Scale;
};

class CircleColider : public Colider
{
public:
	CircleColider() noexcept = default;

	CircleColider(glm::vec3 position, float radius) noexcept
		: m_Position(position), m_Radius(radius) {}

	virtual ColiderType GetType() const noexcept { return ColiderType::CircleColider; }

	void SetPosition(glm::vec3 position) noexcept { m_Position = position; }
	void SetRadius(float radius) noexcept { m_Radius = radius; }

	glm::vec3 GetPosition() const noexcept { return m_Position; }
	float GetRadius() const noexcept { return m_Radius; }

private:
	glm::vec3 m_Position;
	float m_Radius = 1.0f;
};