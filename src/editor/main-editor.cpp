//
// Created by Alejandro Hervella on 3/23/20.
//

//Main program inspired by previous labs (i.e. Lab 2)
//#include "SDLGraphicsProgram.hpp"
#include <iostream>
#include "SDLGP_Editor.hpp"

//will need to overwrite:
//loop()
//update()


int main(int argc, char* argv[]){
	/*
    // Create an instance of an object for a SDLGraphicsProgram
    SDLGraphicsProgram myedt_SDLGraphicsProgram(-2);
    // Run our program forever
    myedt_SDLGraphicsProgram.loop();
    // When our program ends, it will exit scope, the
    // destructor will then be called and clean up the program.
    */

	std::cout << "Please enter the number corresponding to the game you wish to edit." << std::endl;
	std::cout << "1 = Breakout" << std::endl;
	std::cout << "2 = Platformer" << std::endl;
	std::cout << "3 = Galaga" << std::endl;

	int e;
	std::cin >> e;

	if (e != 1 && e != 2 && e != 3)
	{
		std::cout << "Invalid ID. Terminating program." << std::endl;
	}

	else
	{
		SDLGP_Editor editor(e);
		editor.loop();
	}

    return 0;
}
