//Main program inspired by previous labs (i.e. Lab 2)
#include "SDLGP_Galaga.hpp"

int main(int argc, char* argv[]){
	SDLGP_Galaga game;
    std::srand((unsigned) std::time(nullptr));
	game.loop();

	return 0;
}
