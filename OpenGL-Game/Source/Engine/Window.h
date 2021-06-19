#pragma once

#include <cstdint>
#include <string>

class GLFWwindow;

class Window
{
public:
	Window(std::uint_fast32_t width, std::uint_fast32_t height, const std::string& title);
	~Window() noexcept;

	bool IsRunning() const noexcept;
	void PollEvents() const noexcept;
	void SwapBuffers() const noexcept;

	std::uint_fast32_t GetWidth() const noexcept { return m_Width; }
	std::uint_fast32_t GetHeight() const noexcept { return m_Height; }

	GLFWwindow* GetGLFWwindow() const noexcept { return m_Window; }

private:
	GLFWwindow* m_Window;
	std::int_fast32_t m_Width, m_Height;
};