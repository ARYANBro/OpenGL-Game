#include "OpenGLRenderAPI.h"

#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

OpenGLRenderAPI::OpenGLRenderAPI(const Window& window)
	: m_Window(window)
{
	int gladInitResult = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	if (gladInitResult == 0)
		throw std::runtime_error("Could not load glad");

	m_Vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
	m_Renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	m_Version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				std::cerr << "OpenGL Severity High: " << message << std::endl;

			case GL_DEBUG_SEVERITY_MEDIUM:
				std::cerr << "OpenGL Severity Medium: " << message << std::endl;

			case GL_DEBUG_SEVERITY_LOW:
				std::cerr << "OpenGL Severity Low: " << message << std::endl;

			default:
				return;
		}

	}, nullptr);

	glfwSetFramebufferSizeCallback(window.GetGLFWwindow(), [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	});
}

void OpenGLRenderAPI::SetClearColor(float r, float g, float b, float a) const noexcept
{
	glClearColor(r, g, b, a);
}

void OpenGLRenderAPI::SetViewportSize(int x, int y, int width, int height) const noexcept
{
}

void OpenGLRenderAPI::Clear() const noexcept
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
