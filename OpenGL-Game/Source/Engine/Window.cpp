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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_Width = static_cast<int>(width);
	m_Height = static_cast<int>(height);
	m_InitialWidth = static_cast<std::int_fast32_t>(width);
	m_Window = glfwCreateWindow(m_Width, m_Height, name.c_str(), nullptr, nullptr);

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

void Window::OnEvent(const Event& event) noexcept
{
	if (event.GetType() == EventType::WindowResizeEvent)
	{
		const WindowResizeEvent& windowResizeE = static_cast<const WindowResizeEvent&>(event);

		m_WindowScale += (windowResizeE.GetWidth() - static_cast<float>(m_Width)) / m_InitialWidth;
		m_WindowScale = std::abs(m_WindowScale);

		m_Width = windowResizeE.GetWidth();
		m_Height = windowResizeE.GetHeight();

	}
}

float Window::GetWindowScale() const noexcept
{
	return m_WindowScale;
}