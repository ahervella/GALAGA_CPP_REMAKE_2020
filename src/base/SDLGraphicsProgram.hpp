/*
 * SDLGraphicsProgram.hpp
 *
 *  Created on: Mar 30, 2020
 *      Author: Karina
 */

#ifndef SRC_BASE_SDLGRAPHICSPROGRAM_HPP_
#define SRC_BASE_SDLGRAPHICSPROGRAM_HPP_

#include <SDL.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>

#include "ResourceManager.hpp"

class SDLGraphicsProgram{
public:

	/*GameCodes:
	 * 1 = Breakout
	 * 2 = Platformer
	 * 3 = Galaga (To Do)
	 */

    // Constructor
    SDLGraphicsProgram(int gameCode);
    // Desctructor
    ~SDLGraphicsProgram();
    // Per frame update for Breakout
    void updateBreakout();
    // Per frame update for Platformer
    void updatePlatformer();
    // Renders shapes to the screen for Breakout
    void renderBreakout();
    // Renders shapes to the screen for Platformer
    void renderPlatformer();
    // loop that runs forever for Breakout
    void loopBreakout();
    // loop that runs forever for Platformer
    void loopPlatformer();

    /**
     * Initializes the level and language config files in the resource manager
     */
    void initLevelLoading();

    /**
     * Gets all the language files
     */
    void getLanguages();

    /**
     * Changes the language
     *
     * @param langIndex: Index of vector of language files to read from
     */
    void changeLanguage(int langIndex);
    // Get Pointer to Window
    //Blah
    SDL_Window* getSDLWindow();
    // Get Pointer to Renderer
    SDL_Renderer* getSDLRenderer();



private:
    //Game Code
    int gc;
    // Screen dimension constants
    int screenWidth;
    int screenHeight;
    // The window we'll be rendering to
    SDL_Window* gWindow;
    // SDL Renderer
    SDL_Renderer* gRenderer;
    //Resource Manager
    ResourceManager* resourceManager;
    //Background music
    std::shared_ptr<Mix_Music*> backgroundMusic;
    //Background music file
    std::string backgroundMusicFile;
    //Vector for language files
    std::vector<std::string> languageFiles;
    //Vector for managed file resources
    std::shared_ptr<std::ifstream*> currLanguageStream;

};




#endif /* SRC_BASE_SDLGRAPHICSPROGRAM_HPP_ */
