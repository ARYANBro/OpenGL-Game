#pragma once

#include <Engine/Application.h>
#include <Engine/Shader.h>
#include <Engine/Texture.h>

#include "BlockAligner.h"
#include "GameLevel.h"

#include <Scene/Scene.h>
#include <Scene/Entity.h>

enum class GameStateType
{
	Active, Menu, Finished
};

class Game;
class GameStateActive;
class GameStateMenu;
class GameStateFinished;

class GameState
{
public:
	virtual GameState* Update(Game& game, const DeltaTime& dt) = 0;

	virtual void OnBegin(Game& game) = 0;
	virtual void OnUpdate(Game& game, const DeltaTime& dt) = 0;
	virtual void OnEnd(Game& game) = 0;

public:
	static GameStateActive s_GameStateActive;
	static GameStateMenu s_GameStateMenu;
	static GameStateFinished s_GameStateFinished;
};

class GameStateActive : public GameState
{
	virtual GameState* Update(Game& game, const DeltaTime& dt) override;

	virtual void OnBegin(Game& game) override;
	virtual void OnUpdate(Game& game, const DeltaTime& dt) override;
	virtual void OnEnd(Game& game) override;

private:
	std::unique_ptr<GameLevel> m_Level;
};

class GameStateMenu : public GameState
{
	virtual GameState* Update(Game& game, const DeltaTime& dt) override;
	virtual void OnBegin(Game& game) override;
	virtual void OnUpdate(Game& game, const DeltaTime& dt) override;
	virtual void OnEnd(Game& game) override;
};

class GameStateFinished : public GameState
{
	virtual GameState* Update(Game& game, const DeltaTime& dt) override;

	virtual void OnBegin(Game& game) override;
	virtual void OnUpdate(Game& game, const DeltaTime& dt) override;
	virtual void OnEnd(Game& game) override;
};

class Game : public Application
{
public:
	Game(std::uint_fast32_t width, std::uint_fast32_t height, const std::string& title)
		: Application(width, height, title), m_Scene() {}

	virtual void OnBegin() override;
	virtual void OnUpdate(const DeltaTime& deltaT) override;
	virtual void OnEvent(const Event& event) override;
	virtual void OnRender() override;
	virtual void OnEnd() override;

	ShaderLibrary& GetShaderLibrary() noexcept { return m_ShaderLibrary; }
	TextureLibrary& GetTextureLibrary() noexcept { return m_TextureLibrary; }

private:
	friend class GameStateActive;
	friend class GameStateMenu;
	friend class GameStateFinished;

private:
	Scene m_Scene;
	// Scene* m_Scene;

	GameState* m_GameState;
	ShaderLibrary m_ShaderLibrary;
	TextureLibrary m_TextureLibrary;
};
