#include "Input.h"

#include "Window.h"

#include <GLFW/glfw3.h>

#include <cassert>

Window* Input::s_Window;

void Input::Init(Window* window) noexcept
{
	assert(window);
	s_Window = window;
}

bool Input::IsKeyPressed(int key) noexcept
{
	return glfwGetKey(s_Window->GetGLFWwindow(), key) == GLFW_PRESS;
}

bool Input::IsKeyReleased(int key) noexcept
{
	return glfwGetKey(s_Window->GetGLFWwindow(), key) == GLFW_RELEASE;
}