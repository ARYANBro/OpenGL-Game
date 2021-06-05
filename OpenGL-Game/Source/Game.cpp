#include "Game.h"

#include <typeindex>

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
}

void Game::OnUpdate(const DeltaTime& deltaT)
{
	GameState* state = m_GameState->Update(*this, deltaT);

	if (state != nullptr)
		m_GameState = state;

	m_GameState->OnUpdate(*this, deltaT);
}

void Game::OnEnd()
{
}
