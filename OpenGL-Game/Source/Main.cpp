#include "Engine/Application.h"

#include <iostream>
#include <memory>

extern std::unique_ptr<Application> CreateApplication(std::uint_fast32_t width, std::uint_fast32_t height, const std::string& title);

int main()
{
	auto app = CreateApplication(1280, 700, "OpenGL-Game");
	app->Start();
}
