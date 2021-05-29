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

	GLFWwindow* GetGLFWwindow() const noexcept { return m_Window; }

private:
	GLFWwindow* m_Window;
};