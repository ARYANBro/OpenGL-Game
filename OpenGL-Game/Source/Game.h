#pragma once

#include "Engine/Application.h"

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
	virtual void OnUpdate(Game& game, const DeltaTime& dt) = 0;

public:
	static GameStateActive s_GameStateActive;
	static GameStateMenu s_GameStateMenu;
	static GameStateFinished s_GameStateFinished;
};

class GameStateActive : public GameState
{
	virtual GameState* Update(Game& game, const DeltaTime& dt) override;
	virtual void OnUpdate(Game& game, const DeltaTime& dt) override;
};

class GameStateMenu : public GameState
{
	virtual GameState* Update(Game& game, const DeltaTime& dt) override;
	virtual void OnUpdate(Game& game, const DeltaTime& dt) override;
};

class GameStateFinished : public GameState
{
	virtual GameState* Update(Game& game, const DeltaTime& dt) override;
	virtual void OnUpdate(Game& game, const DeltaTime& dt) override;
};

class Game : public Application
{
public:
	GameState* m_GameState;

	virtual void OnBegin() override;
	virtual void OnUpdate(const DeltaTime& deltaT) override;
	virtual void OnEnd() override;
};
