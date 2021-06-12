#include "Game.h"

#include "Engine/Renderer2D.h"

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
}

void Game::OnUpdate(const DeltaTime& deltaT)
{
	GameState* state = m_GameState->Update(*this, deltaT);

	if (state != nullptr)
		m_GameState = state;

	m_GameState->OnUpdate(*this, deltaT);
	m_Scene.OnUpdate(deltaT);
}

void Game::OnRender()
{
	m_Scene.OnRender();
}

void Game::OnEnd()
{
	m_Scene.OnEnd();
	Renderer2D::Deinit();
}
