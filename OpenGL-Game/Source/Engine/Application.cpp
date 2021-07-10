#include "Application.h"

#include "DeltaTime.h"
#include "Input.h"

#include <GLFW/glfw3.h>

#include <iostream>

Application* Application::s_Instance = nullptr;

Application::Application(std::uint_fast32_t width, std::uint_fast32_t height, const std::string& title)
	: m_Window(width, height, title)
{
	s_Instance = this;
	m_RenderAPI.Init(m_Window);

	std::cout << "OpenGL info:\n"
			<< "Vendor: " << m_RenderAPI.GetVendorName() << std::endl
			<< "Renderer: " << m_RenderAPI.GetRendererName() << std::endl
			<< "Version: " << m_RenderAPI.GetVersionName() << std::endl;
	
	glfwSetWindowUserPointer(m_Window.GetGLFWwindow(), this);

	glfwSetFramebufferSizeCallback(m_Window.GetGLFWwindow(), [](GLFWwindow* window, int width, int height)
	{
		WindowResizeEvent event(width, height);
		Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
		app->OnEvent(event);
	});

	glfwSetKeyCallback(m_Window.GetGLFWwindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		KeyAction keyAction;

		switch (action)
		{
			case GLFW_PRESS:
				keyAction = KeyAction::Pressed;
				break;
			case GLFW_RELEASE:
				keyAction = KeyAction::Released;
				break;
			case GLFW_REPEAT:
				keyAction = KeyAction::Held;
				break;
		}

		KeyEvent event(key, scancode, keyAction);
		Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
		app->OnEvent(event);
	});

	Input::Init(&m_Window);
}

Application::~Application() noexcept
{
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
	m_Window.OnEvent(event);
	m_RenderAPI.OnEvent(event);
}
