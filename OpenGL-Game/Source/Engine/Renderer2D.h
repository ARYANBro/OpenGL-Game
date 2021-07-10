#pragma once

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Scene;

class Renderer2D
{
public:
	static void Init();
	static void Deinit() noexcept;

	static void DrawSprite(const glm::mat4& projection, const glm::mat4& transform, const Texture& texture, glm::vec3 color = glm::vec3(1.0f));
	static void RenderScene(Scene& scene, const glm::mat4& projection);

private:
	struct SpriteData
	{
		VertexArray* VertexArray;
		Shader* Shader;
	};

private:
	static SpriteData s_SpriteData;
};
