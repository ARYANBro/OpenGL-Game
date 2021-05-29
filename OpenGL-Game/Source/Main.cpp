#include "Engine/Application.h"

#include <iostream>

int main()
{
	try
	{
		Application app(1280, 700, "OpenGL-Game");
		app.Start();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception occured in main(): " << e.what() << std::endl;
	}
}