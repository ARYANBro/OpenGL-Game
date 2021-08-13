#include "Game.h"

#include <Engine/Renderer2D.h>
#include <Engine/Physics/PhysicsSystem.h>
#include <Scene/Entity.h>
#include <Engine/Script.h>

#include "Scripts/BackGround.h"
#include "Scripts/PlayerPaddle.h"
#include "Scripts/BallScript.h"
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

	Entity player = game.m_Scene.CreateEntity();
	auto spriteRenderer = player.AddComponent<SpriteRendererComponent>();
	spriteRenderer->Texture = TextureLibrary::Get().Load("Assets\\Textures\\paddle.png");

	auto playerTag = player.GetComponent<TagComponent>();
	playerTag->Tag = "Player";

	auto playerSc = player.AddComponent<ScriptComponent>();
	playerSc->Script = std::make_unique<PlayerPaddle>(spriteRenderer, 130.0f, 500.0f);

	auto playerTc = player.GetComponent<TransformComponent>();
	auto playerPc = player.AddComponent<PhysicsComponent>();
	playerPc->Colider = std::make_unique<AABBColider>();
	playerPc->PhysicsType = PhysicsType::Kinetic;
	playerPc->GetColider<AABBColider>().SetPosition(playerTc->Translation);
	playerPc->GetColider<AABBColider>().SetScale(playerTc->Scale);

	Entity ball = game.m_Scene.CreateEntity();

	auto BallTgc = ball.GetComponent<TagComponent>();
	BallTgc->Tag = "Ball";

	auto ballSp = ball.AddComponent<SpriteRendererComponent>();
	ballSp->Texture = TextureLibrary::Get().Load("Assets\\Textures\\Ball.png");

	auto ballSc = ball.AddComponent<ScriptComponent>();
	ballSc->Script = std::make_unique<BallScript>(ballSp, 30.0f);

	auto ballTc = ball.GetComponent<TransformComponent>();
	auto ballPc = ball.AddComponent<PhysicsComponent>();
	ballPc->Colider = std::make_unique<CircleColider>();
	ballPc->GetColider<CircleColider>().SetPosition(ballTc->Translation);
	ballPc->GetColider<CircleColider>().SetRadius(15.0f);

	ballPc->PhysicsType = PhysicsType::Kinetic;

	auto ballParticleComponent = ball.AddComponent<ParticleComponent>();
	ballParticleComponent->Life = 1.0f;
	ballParticleComponent->MaxParticles = 500.0f;
	ballParticleComponent->ParticlesPerFrame = 2;

	m_ParticleGenerator = std::make_unique<ParticleGenerator>(ball);
}

void GameStateActive::OnUpdate(Game& game, const DeltaTime& dt)
{
	game.m_Scene.GetRegistry().EachEntity([&](EntityID e)
	{
		Entity entity = game.m_Scene.GetEntity(e);

		if (entity.GetComponent<TagComponent>()->Tag == "Player")
		{
			auto transform = entity.GetComponent<TransformComponent>();
			auto physicsComponent = entity.GetComponent<PhysicsComponent>();

			physicsComponent->GetColider<AABBColider>().SetPosition(transform->Translation);
			physicsComponent->GetColider<AABBColider>().SetScale(transform->Scale);
		}
		else if (entity.GetComponent<TagComponent>()->Tag == "Ball")
		{
			auto ballTransform = entity.GetComponent<TransformComponent>();
			auto physicsComponent = entity.GetComponent<PhysicsComponent>();
			auto particleComponent = entity.GetComponent<ParticleComponent>();
			auto scriptComponent = entity.GetComponent<ScriptComponent>();

			physicsComponent->GetColider<CircleColider>().SetPosition(ballTransform->Translation);

			if (ballTransform->Translation.y > (game.GetWindow().GetHeight() - ballTransform->Scale.y))
				Reset(game);

			BallScript& ballScript = static_cast<BallScript&>(*scriptComponent->Script);
			particleComponent->Velocity = ballScript.GetVelocity() * 0.1f;
		}
	});

	PhysicsSystem::Update(game.m_Scene);

	game.m_Scene.GetRegistry().EachComponent<ParticleComponent>([&](EntityID e, ParticleComponent& particleComponent)
	{
		m_ParticleGenerator->OnUpdate(game.m_Scene.GetEntity(e), dt);
	});
	
	m_Level->OnUpdate(game.m_Scene, dt);
	game.m_Scene.OnUpdate(dt);
}

void GameStateActive::OnRender(Game& game)
{
	m_ParticleGenerator->OnRender();
}

void GameStateActive::OnEvent(Game& game, const Event& event)
{
	if (event.GetType() == EventType::PhysicsEvent)
	{
		const PhysicsEvent& e = static_cast<const PhysicsEvent&>(event);

		Entity staticEntity = e.GetSecondEntity();

		auto tag = staticEntity.GetComponent<TagComponent>();

		if (tag->Tag == "Block")
		{
			staticEntity.RemoveComponent<SpriteRendererComponent>();
			staticEntity.RemoveComponent<PhysicsComponent>();
			tag->Tag = "AirBlock";
		}
	}
}

void GameStateActive::OnEnd(Game& game)
{
	m_Level->OnEnd(game.m_Scene);
	game.m_Scene.OnEnd();
}

void GameStateActive::Reset(Game& game)
{
	m_Level->Load(game.m_Scene);

	game.m_Scene.GetRegistry().EachEntity([&](EntityID e)
	{
		Entity entity = game.m_Scene.GetEntity(e);
		auto tc = entity.GetComponent<TagComponent>();

		if (tc->Tag == "Player")
		{
			auto sc = entity.GetComponent<ScriptComponent>();
			sc->Script->OnBegin(entity);

			auto tc = entity.GetComponent<TransformComponent>();
			auto pc = entity.GetComponent<PhysicsComponent>();
			pc->PhysicsType = PhysicsType::Kinetic;
			pc->GetColider<AABBColider>().SetPosition(tc->Translation);
			pc->GetColider<AABBColider>().SetScale(tc->Scale);
		}
		else if (tc->Tag == "Ball")
		{
			auto sp = entity.GetComponent<SpriteRendererComponent>();
			sp->Texture = TextureLibrary::Get().Load("Assets\\Textures\\Ball.png");

			auto sc = entity.GetComponent<ScriptComponent>();
			sc->Script->OnBegin(entity);

			auto tc = entity.GetComponent<TransformComponent>();
			auto pc = entity.GetComponent<PhysicsComponent>();
			pc->Colider = std::make_unique<CircleColider>();
			pc->GetColider<CircleColider>().SetPosition(tc->Translation);
			pc->GetColider<CircleColider>().SetRadius(15.0f);

			pc->PhysicsType = PhysicsType::Kinetic;
		}
	});
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

void GameStateMenu::OnEvent(Game& game, const Event& event)
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

void GameStateFinished::OnEvent(Game& game, const Event& event)
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
	m_GameState->OnEvent(*this, event);
}

void Game::OnRender()
{
	float right = GetWindow().GetWidth();
	float top = GetWindow().GetHeight();

	glm::mat4 projection = glm::ortho(0.0f, right, top, 0.0f);
	Renderer2D::RenderScene(m_Scene, projection);
	m_GameState->OnRender(*this);
}

void Game::OnEnd()
{
	m_GameState->OnEnd(*this);
	m_Scene.OnEnd();
	Renderer2D::Deinit();
}

void Game::Reset() noexcept
{
}

std::unique_ptr<Application> CreateApplication(std::uint_fast32_t width, std::uint_fast32_t height, const std::string& title)
{
	return std::make_unique<Game>(width, height, title);
}