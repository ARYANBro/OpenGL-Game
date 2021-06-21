#include "Game.h"

#include "Engine/Renderer2D.h"
#include "SceneSerializer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

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
	game.m_Scene.OnUpdate(deltaT);
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

	Renderer2D::Init(m_ShaderLibrary);
	m_Scene.OnBegin();
	// m_Scene = SceneSerializer::Deserialize("**file path**");
	// SceneSerializer::Serialize(m_Scene, "Assets\\Scenes\\Scene.yaml");
	SceneSerializer::Deserialize(m_Scene, "Assets\\Scenes\\Scene.yaml");
}

void Game::OnUpdate(const DeltaTime& deltaT)
{
	GameState* state = m_GameState->Update(*this, deltaT);

	if (state != nullptr)
		m_GameState = state;

	m_GameState->OnUpdate(*this, deltaT);
}

void Game::OnRender()
{
	float right = GetWindow().GetWidth();
	float top = GetWindow().GetHeight();

	glm::mat4 projection = glm::ortho(-right / 2, right / 2, -top / 2, top / 2);
	Renderer2D::RenderScene(m_Scene, projection);
}

void Game::OnEnd()
{
	m_Scene.OnEnd();
	Renderer2D::Deinit();
}