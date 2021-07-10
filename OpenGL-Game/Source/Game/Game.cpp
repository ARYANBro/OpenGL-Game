#include "Game.h"

#include <Engine/Renderer2D.h>
#include <Scene/Entity.h>
#include <Engine/Script.h>

#include "Scripts/BackGround.h"
#include "Scripts/PlayerPaddle.h"
#include "BlockAligner.h"

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

void GameStateActive::OnBegin(Game& game)
{
	std::vector<int> levelLayout = {
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4,
		4, 1, 4, 1, 4, 0, 0, 1, 0, 0, 4, 1, 4, 1, 4,
		3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
		3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
	};

	glm::vec2 levelExtents = { 15, 8 };

	m_Level = std::make_unique<GameLevel>(levelLayout, levelExtents);

	m_Level->SetColorScheme({
		{ 2, { 0.2f, 0.6f, 1.0f }},
		{ 3, { 0.0f, 0.7f, 0.0f }},
		{ 4, { 0.8f, 0.8f, 0.4f }},
		{ 5, { 1.0f, 0.5f, 0.0f }}
	});

	m_Level->SetBlockTexture(TextureLibrary::Get().Load("Assets\\Textures\\Block\\Block.png"));
	m_Level->SetSolidBlockTexture(TextureLibrary::Get().Load("Assets\\Textures\\Block\\Solid Block.png"));
	m_Level->SetBlockTexRatio(9.0f / 16.0f);

	m_Level->OnBegin(game.m_Scene);

	Entity entity = game.m_Scene.CreateEntity();
	auto spriteRenderer = entity.AddComponent<SpriteRendererComponent>();
	spriteRenderer->Texture = TextureLibrary::Get().Load("Assets\\Textures\\paddle.png");

	auto script = entity.AddComponent<ScriptComponent>();
	script->Script = std::make_unique<PlayerPaddle>(spriteRenderer, 130.0f, 500.0f);
}

void GameStateActive::OnUpdate(Game& game, const DeltaTime& dt)
{
	m_Level->OnUpdate(game.m_Scene, dt);
	game.m_Scene.OnUpdate(dt);
}

void GameStateActive::OnEnd(Game& game)
{
	m_Level->OnEnd(game.m_Scene);
}

GameState* GameStateMenu::Update(Game& game, const DeltaTime& deltaT)
{
	return nullptr;
}

void GameStateMenu::OnBegin(Game& game)
{
}

void GameStateMenu::OnUpdate(Game& game, const DeltaTime& deltaT)
{
}

void GameStateMenu::OnEnd(Game& game)
{
}

GameState* GameStateFinished::Update(Game& game, const DeltaTime& deltaT)
{
	return nullptr;
}

void GameStateFinished::OnBegin(Game& game)
{
}

void GameStateFinished::OnUpdate(Game& game, const DeltaTime& deltaT)
{
}

void GameStateFinished::OnEnd(Game& game)
{
}

void Game::OnBegin()
{
	m_GameState = &GameState::s_GameStateActive;
	m_GameState->OnBegin(*this);

	Renderer2D::Init();
	OpenGLRenderAPI::EnableBlending(GL_ONE_MINUS_SRC_ALPHA);

	m_Scene.OnBegin();
}

void Game::OnUpdate(const DeltaTime& deltaT)
{
	GameState* state = m_GameState->Update(*this, deltaT);

	if (state != nullptr)
		m_GameState = state;

	m_GameState->OnUpdate(*this, deltaT);
}

void Game::OnEvent(const Event& event)
{
	Application::OnEvent(event);
	m_Scene.OnEvent(event);
}

void Game::OnRender()
{
	float right = GetWindow().GetWidth();
	float top = GetWindow().GetHeight();

	glm::mat4 projection = glm::ortho(0.0f, right, top, 0.0f);
	Renderer2D::RenderScene(m_Scene, projection);
}

void Game::OnEnd()
{
	m_Scene.OnEnd();
	Renderer2D::Deinit();
}

std::unique_ptr<Application> CreateApplication(std::uint_fast32_t width, std::uint_fast32_t height, const std::string& title)
{
	return std::make_unique<Game>(width, height, title);
}
