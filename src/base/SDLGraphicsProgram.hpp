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

#include "Level.hpp"
#include "BreakoutLevel.hpp"

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
     * Per frame update.
     */
    void update();

    /**
     * Renders shapes and text to the screen.
     */
    void render();

    /**
     * Loop that runs forever for Platformer.
     */
     void loop();

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
    int screenWidth = 500;
    int screenHeight = 500;

    //level count for breakout
    int levelCount = 0;

    //levels for each type of game
    std::vector<BreakoutLevel> BreakoutLevels;
    std::vector<Level> PlatformerLevels;

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
    //Background image
    std::shared_ptr<SDL_Texture*> backgroundImage;
    //Background image file
    std::string backgroundImageFile;

};




#endif /* SRC_BASE_SDLGRAPHICSPROGRAM_HPP_ */
