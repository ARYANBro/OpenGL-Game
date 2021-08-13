#include "ParticleGenerator.h"

#include <Engine/DeltaTime.h>
#include <Engine/Application.h>
#include <Engine/Renderer2D.h>
#include <Engine/OpenGLRenderAPI.h>

#include <stdexcept>

ParticleGenerator::ParticleGenerator(Entity emitter)
{
	auto particleComponent = emitter.GetComponent<ParticleComponent>();

	if (!particleComponent)
		throw std::runtime_error("Particle Component Does not exist");

	m_Particles.resize(particleComponent->MaxParticles);
}

void ParticleGenerator::OnUpdate(Entity entity, const DeltaTime& dt)
{
	auto particleComponent = entity.GetComponent<ParticleComponent>();

	if (!particleComponent)
		throw std::runtime_error("Particle Component Does not exist");

	auto transformComponent = entity.GetComponent<TransformComponent>();

	for (unsigned int i = 0; i < particleComponent->ParticlesPerFrame; i++)
	{
		Particle* particle =  GetFirstUnusedParticle();
		RespawnParticle(*particle, transformComponent->Translation, 1.0f, particleComponent->Velocity);
	}

	for (auto& particle : m_Particles)
	{
		particle.Life -= dt;

		if (particle.Life > 0.0f)
		{
			particle.Position -= particle.Velocity * static_cast<float>(dt);
			particle.Color.a -= dt * 2.5f;
		}
	}
}

void ParticleGenerator::OnRender() noexcept
{
	float right = Application::GetInstance().GetWindow().GetWidth();
	float top = Application::GetInstance().GetWindow().GetHeight();

	glm::mat4 projection = glm::ortho(0.0f, right, top, 0.0f);

	for (auto& particle : m_Particles)
	{
		if (particle.Life > 0.0f)
		{
			glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(particle.Position, 0.0f));
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(15.0f));

			glm::mat4 transformed = translate * scale;

			Renderer2D::DrawQuad(projection, transformed, TextureLibrary::Get().Load("Assets\\Textures\\particle.png"), particle.Color);
		}
	}
}

Particle* ParticleGenerator::GetFirstUnusedParticle() noexcept
{
	for (int i = m_LastUnusedParticleIdx; i < m_Particles.size(); i++)
	{
		if (Particle& particle = m_Particles[i]; particle.Life <= 0.0f)
		{
			m_LastUnusedParticleIdx = i;
			return &particle;
		}
	}

	for (int i = 0; i < m_LastUnusedParticleIdx; i++)
	{
		if (Particle& particle = m_Particles[i]; particle.Life <= 0.0f)
		{
			m_LastUnusedParticleIdx = i;
			return &particle;
		}
	}

	m_LastUnusedParticleIdx = 0;
	return nullptr;
}

void ParticleGenerator::RespawnParticle(Particle& particle, glm::vec2 position, float life, glm::vec2 velocity) noexcept
{
	float random = ((rand() % 100) - 50) / 10.0f;
	glm::vec3 color = glm::vec3(0.5f + ((rand() % 100) / 100.0f));

	particle.Position = position + random;
	particle.Color = glm::vec4(color, 1.0f);
	particle.Life = life;
	particle.Velocity = velocity;
}
