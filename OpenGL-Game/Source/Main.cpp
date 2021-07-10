#include "Engine/Application.h"

#include <iostream>
#include <memory>

extern std::unique_ptr<Application> CreateApplication(std::uint_fast32_t width, std::uint_fast32_t height, const std::string& title);

int main()
{
	try
	{
		auto app = CreateApplication(1280, 700, "OpenGL-Game");
		app->Start();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception occured in main():\n" << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Exception occured in main()\n";
	}
}
