#include "Renderer2D.h"

#include "Texture.h"
#include "Shader.h"
#include "OpenGLRenderAPI.h"
#include "VertexBuffer.h"

Renderer2D::SpriteData Renderer2D::s_SpriteData;

void Renderer2D::Init(ShaderLibrary& shaderLibrary)
{
	s_SpriteData.VertexArray = new VertexArray();
	s_SpriteData.Shader = shaderLibrary.Load("Assets\\Shaders\\Shader.glsl");

	auto vb = std::make_shared<VertexBuffer>();

	float vertices[] = {
		-0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  1.0f, 0.0f
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

void Renderer2D::DrawSprite(const glm::mat4& transform, const Texture& texture, glm::vec3 color)
{
	s_SpriteData.Shader->Bind();

	texture.Bind(0);
	s_SpriteData.Shader->SetInt("u_Texture", 0);

	OpenGLRenderAPI::Render(*s_SpriteData.VertexArray, *s_SpriteData.Shader);

	texture.Unbind();
}
