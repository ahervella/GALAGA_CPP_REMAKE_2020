/*
 * SDLGraphicsProgram.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: Karina
 */

#include "SDLGraphicsProgram.hpp"

//May not use these, but this might be fun to play with
//const SDL_Color TEXT_COLOR_NEUTRAL = {255, 255, 255, 255};
//const SDL_Color TEXT_COLOR_LOSE = {255, 0, 0, 255};
//const SDL_Color TEXT_COLOR_WIN = {0, 255, 0, 255};

//Define SDLGraphicsProgram functions

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram() :
        gWindow(nullptr),
        gRenderer(nullptr) {

    // Initialization flag
    bool success = true;
    // String to hold any errors that occur.
    std::stringstream errorStream;

    // The window we'll be rendering to here
    gWindow = nullptr;
    // Render flag

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
        success = false;
    } else {
    	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
    		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    	    success = false;
    	} else {
        //Initialize TTF
    		if (TTF_Init() != 0) {
            errorStream << "SDL_TTF could not initialize! SDL Error: " << SDL_GetError() << "\n";
            success = false;
    		} else {

    			//Create window
    			gWindow = SDL_CreateWindow("Final Project",
                                       	   SDL_WINDOWPOS_UNDEFINED,
										   SDL_WINDOWPOS_UNDEFINED,
										   screenWidth,
										   screenHeight,
										   SDL_WINDOW_SHOWN);

    			// Check if Window did not create.
    			if (gWindow == nullptr) {
    				errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
    				success = false;
    			}

    			//Create a Renderer to draw on
    			//-1 flag places thing to be rendered in next available spot (index)
    			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    			// Check if Renderer did not create.
    			if (gRenderer == nullptr) {
    				errorStream << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
    				success = false;
    			}
        	}
    	}
    }

    // If initialization did not work, then print out a list of errors in the constructor.
    if (!success) {
        errorStream << "Failed to initialize!\n";
        std::string errors = errorStream.str();
        std::cout << errors << "\n";
    } else {
        std::cout << "No SDL, or OpenGL, errors Detected\n\n";
    }

    //Resource Manager initialization
    resourceManager = ResourceManager::getInstance();

    //Load background music
    backgroundMusic = resourceManager->getMusicResource(this->backgroundMusicFile);

    //Load background image
    this->backgroundImageFile = Constants::Platformer::TexturePath::BACKGROUND;
    backgroundImage = resourceManager->getTextureResource(getSDLRenderer(), this->backgroundImageFile);
}


// Proper shutdown and destroy initialized objects
//~ = destructor
SDLGraphicsProgram::~SDLGraphicsProgram() {
	//Stop and destroy music
	Mix_HaltMusic();
	backgroundMusic.reset();
	resourceManager->deleteMusicResource(backgroundMusicFile);
	// Destroy Renderer
    SDL_DestroyRenderer(gRenderer);
    //Destroy window
    SDL_DestroyWindow(gWindow);
    // Point gWindow to NULL to ensure it points to nothing.
    gRenderer = nullptr;
    gWindow = nullptr;
    //Quit SDL subsystems
    //TODO: Check if any other ones need to be quit

    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
    //Destroy resource manager
    resourceManager->shutDown();
}

// Get Pointer to Window
//(private) utility in case we want to get the window
SDL_Window *SDLGraphicsProgram::getSDLWindow() {
    return gWindow;
}

// Get Pointer to Renderer
//(private) utility in case we want to get the window
SDL_Renderer *SDLGraphicsProgram::getSDLRenderer() {
    return gRenderer;
}
