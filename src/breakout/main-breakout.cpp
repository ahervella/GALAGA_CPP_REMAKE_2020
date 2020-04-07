//Main program inspired by previous labs (i.e. Lab 2)
//#include "SDLGraphicsProgram.hpp"

#include "SDLGP_Breakout.hpp"

int main(int argc, char* argv[]){
	/*
  // Create an instance of an object for a SDLGraphicsProgram
  SDLGraphicsProgram mySDLGraphicsProgram(1);
  // Run our program forever
  mySDLGraphicsProgram.loop();
  // When our program ends, it will exit scope, the
  // destructor will then be called and clean up the program.
   */

	SDLGP_Breakout game;
	game.loop();
	return 0;
}
