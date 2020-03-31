//
// Created by Alejandro Hervella on 3/23/20.
//

//Main program inspired by previous labs (i.e. Lab 2)
//#include "SDLGraphicsProgram.hpp"
#include "SDLGraphicsProgram.hpp"

//will need to overwrite:
//loop()
//update()


int main(int argc, char* argv[]){
    // Create an instance of an object for a SDLGraphicsProgram
    SDLGraphicsProgram myedt_SDLGraphicsProgram(-1);
    // Run our program forever
    myedt_SDLGraphicsProgram.loop();
    // When our program ends, it will exit scope, the
    // destructor will then be called and clean up the program.
    return 0;
}