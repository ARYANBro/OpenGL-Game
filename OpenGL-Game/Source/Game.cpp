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

	m_Texture = m_TextureLibrary.Load("Assets\\Textures\\Planks\\Planks012_4K_Color.png");

	Renderer2D::Init(m_ShaderLibrary);
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
	Renderer2D::DrawSprite(glm::mat4(1.0f), *m_Texture);
}

void Game::OnEnd()
{
	Renderer2D::Deinit();
}
