#pragma once

#include <Scene/Entity.h>

class CollisionInfo;

enum class EventType
{
	Event, WindowResizeEvent, KeyEvent, PhysicsEvent
};

class Event
{
public:
	virtual EventType GetType() const noexcept = 0;
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(std::uint_fast32_t width, std::uint_fast32_t height) noexcept
		: m_Width(width), m_Height(height) {}

	virtual EventType GetType() const noexcept override { return EventType::WindowResizeEvent; }

	std::uint_fast32_t GetWidth() const noexcept { return m_Width; }
	std::uint_fast32_t GetHeight() const noexcept { return m_Height; }

private:
	std::uint_fast32_t m_Width, m_Height;
};

enum class KeyAction
{
	Pressed, Released, Held
};

class KeyEvent : public Event
{
public:
	KeyEvent(int key, int scanCode, KeyAction action) noexcept	
		: m_Key(key), m_ScanCode(scanCode), m_Action(action) {}

	virtual EventType GetType() const noexcept override { return EventType::KeyEvent; }

	int GetKey() const noexcept { return m_Key; }
	int GetScanCode() const noexcept { return m_ScanCode; }
	KeyAction GetAction() const noexcept { return m_Action; }

private:
	int m_Key;
	int m_ScanCode;
	KeyAction m_Action;
};

class PhysicsEvent : public Event
{
public:
	PhysicsEvent(Entity firstEntity, Entity secondEntity, CollisionInfo* ci) noexcept
		: m_FirstEntity(firstEntity), m_SecondEntity(secondEntity), m_Ci(ci) {}

	virtual EventType GetType() const noexcept override { return EventType::PhysicsEvent; }

	Entity GetFirstEntity() const noexcept { return m_FirstEntity; }
	Entity GetSecondEntity() const noexcept { return m_SecondEntity; }
	CollisionInfo* GetCollisionInfo() const noexcept { return m_Ci; }

private:
	Entity m_FirstEntity;
	Entity m_SecondEntity;

	CollisionInfo* m_Ci;
};