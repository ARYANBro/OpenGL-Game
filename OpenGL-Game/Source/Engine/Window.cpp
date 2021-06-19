#include "Window.h"

#include "Application.h"

#include <GLFW/glfw3.h>

#include <stdexcept>

Window::Window(std::uint_fast32_t width, std::uint_fast32_t height, const std::string& name)
{
	int glfwInitResult = glfwInit();

	if (glfwInitResult == GLFW_FALSE) throw std::runtime_error("Could not initialize glfw");

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	m_Width = static_cast<int>(width);
	m_Height = static_cast<int>(height);
	m_Window = glfwCreateWindow(m_Width, m_Height, name.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_Window);
}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

bool Window::IsRunning() const noexcept
{
	return !glfwWindowShouldClose(m_Window);
}

void Window::PollEvents() const noexcept
{
	glfwPollEvents();
}

void Window::SwapBuffers() const noexcept
{
	glfwSwapBuffers(m_Window);
}
