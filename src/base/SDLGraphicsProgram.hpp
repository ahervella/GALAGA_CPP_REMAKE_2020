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

//for getting files from directory
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include <algorithm>
#include <memory>

#include "GameObject.hpp"

#include "TinyMath.hpp"


#include "Textbox.hpp"

#include "ResourceManager.hpp"

#include "../platformer/Level.hpp"
#include "BreakoutLevel.hpp"
#include "GalagaLevel.hpp"

#define SCREEN_TICKS_PER_FRAME 1000/60

#define GAME_WON_TEXT "You won! Space for next Level!"
#define NA_TEXT "N/A"
#define DEFAULT_LIVES 3

/*


*/

class SDLGraphicsProgram{
public:

    // Constructor
    SDLGraphicsProgram();
    // Desctructor
    ~SDLGraphicsProgram();

    // Get Pointer to Window
    //Blah
    SDL_Window* getSDLWindow();
    // Get Pointer to Renderer
    SDL_Renderer* getSDLRenderer();

protected:

    //level count
    int levelCount = 0;

    // Screen dimension constants
    int screenWidth = 500;
    int screenHeight = 500;

    bool lvlSelectMode = true;

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

    std::map<std::string, std::string> gameTexts{
            {"LANG_MENU", NA_TEXT},
            {"DONE", NA_TEXT},
            {"START", NA_TEXT},
            {"LOSE", NA_TEXT},
            {"WIN", NA_TEXT},
            {"LIVES", NA_TEXT},
            {"SCORE", NA_TEXT}
    };

    int score = 0;
    int currLevelScore = 0;
    int currLevelIndex = 0;

    Textbox centerText = Textbox(NA_TEXT, 20, SCREEN_WIDTH / 2 - 175, SCREEN_HEIGHT / 2 - 100);
    CounterTextbox livesText = CounterTextbox(NA_TEXT, DEFAULT_LIVES, 15, 15, 3);
    CounterTextbox scoreText = CounterTextbox(NA_TEXT, 0, 15, SCREEN_WIDTH - 75, 3);

};




#endif /* SRC_BASE_SDLGRAPHICSPROGRAM_HPP_ */
