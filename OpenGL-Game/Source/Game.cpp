#include "Game.h"

#include <glad/glad.h>

#include <typeindex>
#include <iostream>

GameStateActive GameState::s_GameStateActive;
GameStateMenu GameState::s_GameStateMenu;
GameStateFinished GameState::s_GameStateFinished;

GameState* GameStateActive::Update(Game& game, const DeltaTime& deltaT)
{
	return nullptr;
}

void GameStateActive::OnUpdate(Game& game, const DeltaTime& deltaT)
{
}


GameState* GameStateMenu::Update(Game& game, const DeltaTime& deltaT)
{
	return nullptr;
}

void GameStateMenu::OnUpdate(Game& game, const DeltaTime& deltaT)
{
}


GameState* GameStateFinished::Update(Game& game, const DeltaTime& deltaT)
{
	return nullptr;
}

void GameStateFinished::OnUpdate(Game& game, const DeltaTime& deltaT)
{
}

void Game::OnBegin()
{
	m_GameState = &GameState::s_GameStateActive;

	m_Shader = m_ShaderLibrary.Load("Assets\\Shaders\\Shader.glsl");

	m_Texture = m_TextureLibrary.Load("Assets\\Textures\\Planks\\Planks012_4K_Color.png");
	
}

void Game::OnUpdate(const DeltaTime& deltaT)
{
	GameState* state = m_GameState->Update(*this, deltaT);

	if (state != nullptr)
		m_GameState = state;

	m_GameState->OnUpdate(*this, deltaT);

	GLuint vao;
	glCreateVertexArrays(1, &vao);

	GLuint vbo;
	glCreateBuffers(1, &vbo);

	float vertices[] = {
		-0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  1.0f, 0.0f
	};

	glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, 4 * sizeof(float));

	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_TRUE, 0);
	glVertexArrayAttribBinding(vao, 0, 0);

	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
	glVertexArrayAttribBinding(vao, 1, 0);

	GLuint ibo;
	glCreateBuffers(1, &ibo);

	unsigned int indices[] = { 
		2, 0, 1,
		2, 1, 3,
	};

	glNamedBufferData(ibo, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexArrayElementBuffer(vao, ibo);

	glBindVertexArray(vao);

	m_Shader->Bind();

	m_Texture->Bind(0);
	m_Shader->SetInt("u_Texture", 0);

	glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_INT, nullptr);

	m_Texture->Unbind();
	m_Shader->Unbind();

	glDeleteVertexArrays(1, &vao);
 	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Game::OnEnd()
{
}
