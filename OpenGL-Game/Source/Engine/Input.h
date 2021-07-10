#pragma once

class Window;

class Input
{
public:
	static void Init(Window* window) noexcept;

	static bool IsKeyPressed(int key) noexcept;
	static bool IsKeyReleased(int key) noexcept;

private:
	static Window* s_Window;
};