//Main program inspired by previous labs (i.e. Lab 2)
#include "SDLGraphicsProgram.hpp"

int main(int argc, char* argv[]){
  // Create an instance of an object for a SDLGraphicsProgram
  SDLGraphicsProgram mySDLGraphicsProgram(1);
  // Run our program forever
  mySDLGraphicsProgram.loopBreakout();
  // When our program ends, it will exit scope, the
  // destructor will then be called and clean up the program.
  return 0;
}
