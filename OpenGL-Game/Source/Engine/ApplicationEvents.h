#pragma once

#include <cstdint>

#include "Event.h"

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(std::uint_fast32_t x, std::uint_fast32_t y, std::uint_fast32_t width, std::uint_fast32_t height) noexcept
		: m_X(x), m_Y(y), m_Width(width), m_Height(height)
	{
	}

	std::uint_fast32_t GetX() const noexcept { return m_X; }
	std::uint_fast32_t GetY() const noexcept { return m_Y; }
	std::uint_fast32_t GetWidth() const noexcept { return m_Width; }
	std::uint_fast32_t GetHeight() const noexcept { return m_Height; }

	EVENT_CLASS(WidowResizeEvent);

private:
	std::uint_fast32_t m_X, m_Y;
	std::uint_fast32_t m_Width, m_Height;
};