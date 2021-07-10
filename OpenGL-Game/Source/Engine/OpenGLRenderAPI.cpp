#include "OpenGLRenderAPI.h"

#include "Window.h"
#include "Shader.h"
#include "Engine/Event.h"
#include "VertexArray.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <iostream>

OpenGLRenderAPI::OpenGLRenderAPI(Window& window)
{
	Init(window);
}

void OpenGLRenderAPI::Init(Window& window)
{
	m_Window = &window;
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
}

void OpenGLRenderAPI::OnEvent(const Event& event) const noexcept
{
	if (event.GetType() == EventType::WindowResizeEvent)
	{
		WindowResizeEvent windowResizeE = static_cast<const WindowResizeEvent&>(event);
		glViewport(0, 0, windowResizeE.GetWidth(), windowResizeE.GetHeight());
	}
}

void OpenGLRenderAPI::SetClearColor(float r, float g, float b, float a) noexcept
{
	glClearColor(r, g, b, a);
}

void OpenGLRenderAPI::EnableBlending(GLenum blendEquation) noexcept
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderAPI::Clear() noexcept
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderAPI::Render(const VertexArray& vertexArray, const Shader& shader) noexcept
{
	vertexArray.Bind();

	shader.Bind();

	glDrawElements(GL_TRIANGLES, vertexArray.GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	vertexArray.Unbind();
}
