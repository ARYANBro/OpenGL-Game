#include "Renderer2D.h"

#include "Texture.h"
#include "Shader.h"
#include "Scene/Scene.h"
#include "OpenGLRenderAPI.h"
#include "VertexBuffer.h"

#include <GLFW/glfw3.h>

Renderer2D::SpriteData Renderer2D::s_SpriteData;

void Renderer2D::Init()
{
	s_SpriteData.VertexArray = new VertexArray();
	s_SpriteData.Shader = ShaderLibrary::Get().Load("Assets\\Shaders\\Shader.glsl");

	auto vb = std::make_shared<VertexBuffer>();

	float vertices[] = {
		0.0f, 1.0f,  0.0f, 1.0f,
		1.0f, 1.0f,  1.0f, 1.0f,
		0.0f, 0.0f,  0.0f, 0.0f,
		1.0f, 0.0f,  1.0f, 0.0f
	};

	vb->SetData(vertices, sizeof(vertices));

	BufferLayout layout = { 
		{ 0, 2, GL_FLOAT, true, 0 },
		{ 1, 2, GL_FLOAT, true, 2 * sizeof(float) }
	};

	s_SpriteData.VertexArray->AddVertexBuffer(vb, 4 * sizeof(float), layout);

	auto indexBuffer = std::make_shared<VertexBuffer>();

	unsigned int indices[] = {
		2, 0, 1,
		2, 1, 3
	};

	indexBuffer->SetData(indices, sizeof(indices));

	s_SpriteData.VertexArray->SetIndexBuffer(indexBuffer, std::size(indices));
}

void Renderer2D::Deinit() noexcept
{
	delete s_SpriteData.VertexArray;
}

void Renderer2D::DrawQuad(const glm::mat4& projection, const glm::mat4& transform, const Texture* texture, glm::vec4 color)
{
	s_SpriteData.Shader->Bind();

	if (texture)
	{
		texture->Bind(0);
		s_SpriteData.Shader->SetInt("u_Texture", 0);
	}

	s_SpriteData.Shader->SetFloat4("u_Color", color);
	s_SpriteData.Shader->SetMat4("u_ModelTransform", transform);
	s_SpriteData.Shader->SetMat4("u_ProjectionTransform", projection);

	OpenGLRenderAPI::Render(*s_SpriteData.VertexArray, *s_SpriteData.Shader);

	s_SpriteData.Shader->Unbind();

	if (texture)
		texture->Unbind();
}

void Renderer2D::RenderScene(Scene& scene, const glm::mat4& projection)
{
	scene.m_Registry.EachComponent<SpriteRendererComponent>([&](EntityID entity, const SpriteRendererComponent& component)
	{
		TransformComponent* transform = scene.m_Registry.GetComponent<TransformComponent>(entity);
		DrawQuad(projection, transform->GetTransform(), component.Texture, component.Color != glm::vec4(0.0f) ? component.Color : glm::vec4(1.0f));
	});
}