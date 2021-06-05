#include "Application.h"

#include "DeltaTime.h"

#include <GLFW/glfw3.h>

#include <iostream>

Application::Application(std::uint_fast32_t width, std::uint_fast32_t height, const std::string& title)
	: m_Window(width, height, title), m_RenderAPI(m_Window)
{
	std::cout << "OpenGL info:\n"
			<< "Vendor: " << m_RenderAPI.GetVendorName() << std::endl
			<< "Renderer: " << m_RenderAPI.GetRendererName() << std::endl
			<< "Version: " << m_RenderAPI.GetVersionName() << std::endl;
}

void Application::Start()
{
	OnBegin();

	DeltaTime deltaT;

	while (m_Window.IsRunning())
	{
		m_RenderAPI.SetClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		m_RenderAPI.Clear();

		m_Window.PollEvents();

		deltaT.Calculate();

		OnUpdate(deltaT);
		OnRender();

		m_Window.SwapBuffers();
	}

	OnEnd();
}

void Application::OnEvent(const Event& event)
{
}
