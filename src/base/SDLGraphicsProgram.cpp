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

void SDLGraphicsProgram::renderTexts() {
	//render lives left texture
	SDL_Texture *livesTextImage = renderText(livesText.text, livesText.textResPath, livesText.clr, livesText.textSize,
	                                             getSDLRenderer());
	renderTexture(livesTextImage, getSDLRenderer(), livesText.x, livesText.y);

    SDL_DestroyTexture(livesTextImage);

	//render center texture
	//Trying to skip new line characters here with strtok. There are better ways, but this will work
	int y = centerText.y;
	bool first = true;
	char * origText = new char[centerText.text.length() + 1];
	strcpy(origText, centerText.text.c_str());
	//Loop for writing new lines on a seaparate line
	char * line = strtok(origText, "\\");
	while (line != NULL) {
	   	//A hack for skipping the 'n' in '\n' after strtok skips the '\'.
	   	//The first time around, the beginning of the string is the start of the string and not the trailing 'n'
	   	if(first) {
	   		first = false;
	   	}
	   	//After the first strtok call, the string will start with 'n'. Skip that.
	   	else {
	   		//Make sure there is a character afterwards
	   		if(*(line + 1) != '\0') {
	   			line += 1;
	   		}
	   	}
	   	SDL_Texture *centerTextImage = renderText(line, livesText.textResPath, centerText.clr,
	 	                                              centerText.textSize, getSDLRenderer());
	 	 //TODO: Get centered position here. It fits, but it could look better, y'know?
	 	 renderTexture(centerTextImage, getSDLRenderer(), centerText.x, y);

	 	 SDL_DestroyTexture(centerTextImage);
     	 line = strtok(NULL, "\\");
     	 y += centerText.textSize;
	}

    delete [] origText;

	//render score texture
	SDL_Texture *scoreTextImage = renderText(scoreText.text, scoreText.textResPath, scoreText.clr, scoreText.textSize,
	                                             getSDLRenderer());
	renderTexture(scoreTextImage, getSDLRenderer(), scoreText.x, scoreText.y);

    SDL_DestroyTexture(scoreTextImage);
}

void SDLGraphicsProgram::loadBackgroundMusicAndImage() {

    //Resource Manager initialization
    resourceManager = ResourceManager::getInstance();

    //Load background music
    backgroundMusic = resourceManager->getMusicResource(this->backgroundMusicFile);

    //Load background image
    //this->backgroundImageFile = Constants::Platformer::TexturePath::BACKGROUND;
    backgroundImage = resourceManager->getTextureResource(getSDLRenderer(), this->backgroundImageFile);

}
