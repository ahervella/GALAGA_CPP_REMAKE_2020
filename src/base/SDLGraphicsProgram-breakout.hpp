//
// Created by Alejandro Hervella on 2/6/20.
//

#ifndef PROJ1_BREAKOUT_MIKEY_BREAKOUT_SDLGRAPHICSPROGRAM_HPP
#define PROJ1_BREAKOUT_MIKEY_BREAKOUT_SDLGRAPHICSPROGRAM_HPP

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


// This class sets up a full graphics program
class SDLGraphicsProgram{
public:

    // Constructor
    SDLGraphicsProgram(int w, int h, std::string backgroundMusicFile);
    // Desctructor
    ~SDLGraphicsProgram();
    // Per frame update
    void update();
    // Renders shapes to the screen
    void render();
    // loop that runs forever
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

#endif //PROJ1_BREAKOUT_MIKEY_BREAKOUT_SDLGRAPHICSPROGRAM_H
