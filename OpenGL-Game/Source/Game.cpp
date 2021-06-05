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

	// Shader shader;
	m_Shader.Load("Assets\\Shaders\\Shader.glsl");

	// shader.Unbind();
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
		-0.5f,  0.5f, 
		 0.5f,  0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f
	};

	glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, 2 * sizeof(float));

	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_TRUE, 0);
	glVertexArrayAttribBinding(vao, 0, 0);
	
	GLuint ibo;
	glCreateBuffers(1, &ibo);

	unsigned int indices[] = { 
		2, 0, 1,
		2, 1, 3,
	};

	glNamedBufferData(ibo, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexArrayElementBuffer(vao, ibo);

	glBindVertexArray(vao);

	m_Shader.Bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	m_Shader.Unbind();

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Game::OnEnd()
{
}