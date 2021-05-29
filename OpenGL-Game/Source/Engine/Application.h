#pragma once

#include "Window.h"
#include "OpenGLContext.h"

class Application
{
public:
	Application(std::uint_fast32_t width, std::uint_fast32_t height, const std::string& title);

	void Start();
	
private:
	Window m_Window;
	OpenGLContext m_OpenGLContext;
};