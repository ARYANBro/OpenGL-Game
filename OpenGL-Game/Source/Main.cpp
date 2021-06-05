#include "Game.h"

#include <iostream>

int main()
{
	try
	{
		Game game(1280, 700, "OpenGL-Game");
		game.Start();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception occured in main():\n" << e.what() << std::endl;
	}
}