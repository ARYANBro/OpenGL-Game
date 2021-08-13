#pragma once

#include <Scene/Entity.h>

#include <glm/glm.hpp>

#include <vector>

class DeltaTime;

struct Particle
{
	glm::vec2 Position = glm::vec2(0.0f);
	glm::vec4 Color = glm::vec4(0.0f);
	float Life = 0.0f;
	glm::vec2 Velocity = glm::vec2(0.0f);
};

class ParticleGenerator
{
public:
	ParticleGenerator(Entity emitter);

	void OnUpdate(Entity entity, const DeltaTime& dt);
	void OnRender() noexcept;

private:
	std::vector<Particle> m_Particles;
	int m_LastUnusedParticleIdx = 0;

private:
	Particle* GetFirstUnusedParticle() noexcept;
	void RespawnParticle(Particle& particle, glm::vec2 position, float life, glm::vec2 velocity) noexcept;
};