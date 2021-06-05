#include "Window.h"

#include "ApplicationEvents.h"
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

	int iWidth = static_cast<int>(width);
	int iHeight = static_cast<int>(height);
	m_Window = glfwCreateWindow(iWidth, iHeight, name.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_Window);

	glfwSetWindowUserPointer(m_Window, &m_EventCallback);

	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
	{
		WindowResizeEvent event(width, height);
		EventCallback* callback = reinterpret_cast<EventCallback*>(glfwGetWindowUserPointer(window));

		(*callback)(event);
	});
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
