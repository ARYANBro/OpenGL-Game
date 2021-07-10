#pragma once

#include "Window.h"
#include "OpenGLRenderAPI.h"
#include "Event.h"

class DeltaTime;

class Application
{
public:
	Application(std::uint_fast32_t width, std::uint_fast32_t height, const std::string& title);
	~Application() noexcept;

	void Start();

	virtual void OnBegin() {}
	virtual void OnUpdate(const DeltaTime& deltaT) {}
	virtual void OnEvent(const Event& event);
	virtual void OnRender() {}
	virtual void OnEnd() {}

	const Window& GetWindow() const noexcept { return m_Window; }
	const OpenGLRenderAPI& GetOpenGLRenderAPI() const noexcept { return m_RenderAPI; }


	Window& GetWindow() noexcept { return m_Window; }
	OpenGLRenderAPI& GetOpenGLRenderAPI() noexcept { return m_RenderAPI; }

	static Application& GetInstance() noexcept { return *s_Instance; }

private:
	Window m_Window;
	static Application* s_Instance;
	OpenGLRenderAPI m_RenderAPI;
};
