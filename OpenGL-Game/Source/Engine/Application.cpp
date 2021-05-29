#include "Application.h"

#include <iostream>

Application::Application(std::uint_fast32_t width, std::uint_fast32_t height, const std::string& title)
	: m_Window(width, height, title), m_OpenGLContext(m_Window)
{
	std::cout << "OpenGL info:\n"
			<< "Vendor: " << m_OpenGLContext.GetVendorName() << std::endl
			<< "Renderer: " << m_OpenGLContext.GetRendererName() << std::endl
			<< "Version: " << m_OpenGLContext.GetVersionName() << std::endl;
}

void Application::Start()
{
	while (m_Window.IsRunning())
	{
		m_Window.PollEvents();
		m_Window.SwapBuffers();
	}
}
