#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>

class Script;
class Texture;
class ScriptableEntity;

struct SpriteRendererComponent
{
	Texture* Texture;
	glm::vec3 Color;
};

struct TransformComponent
{
	glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Rotation = { 0.0f, 0.0f , 0.0f };
	glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	glm::mat4 GetTransform() const noexcept
	{
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, Translation);

		transform = glm::rotate(transform, Rotation.x, { 1.0f, 0.0f, 0.0f });
		transform = glm::rotate(transform, Rotation.y, { 0.0f, 1.0f, 0.0f });
		transform = glm::rotate(transform, Rotation.z, { 0.0f, 0.0f, 1.0f });

		transform = glm::scale(transform, Scale);

		return transform;
	}
};

struct TagComponent
{
	std::string Tag;
};

struct ScriptComponent
{
	std::unique_ptr<Script> Script;
};