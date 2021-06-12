#pragma once

#include "VertexArray.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Texture;
class Shader;
class ShaderLibrary;
class Scene;

class Renderer2D
{
public:
	static void Init(ShaderLibrary& shaderLibrary);
	static void Deinit() noexcept;

	static void DrawSprite(const glm::mat4& transform, const Texture& texture, glm::vec3 color = glm::vec3(0.0f));
	static void RenderScene(Scene* scene);

private:
	struct SpriteData
	{
		VertexArray* VertexArray;
		Shader* Shader;
	};

private:
	static SpriteData s_SpriteData;
};
