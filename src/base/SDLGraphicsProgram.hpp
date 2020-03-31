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


#include "Textbox.hpp"

#include "ResourceManager.hpp"

#include "Level.hpp"
#include "BreakoutLevel.hpp"

class SDLGraphicsProgram{
public:

	/*GameCodes:
	 * -1 = LevelEditor
	 * 1 = Breakout
	 * 2 = Platformer
	 * 3 = Galaga (To Do)
	 */

    // Constructor
    SDLGraphicsProgram(int gameCode);
    // Desctructor
    ~SDLGraphicsProgram();

    /**
     * Initializes the level and language config files in the resource manager
     */
    void initLevelLoading();

    void initLevelLoadingEditor();

    void initLevelLoadingGames();

    // Per frame update for Breakout
    void updateBreakout();
    // Per frame update for Platformer
    void updatePlatformer();
    // Per frame update for Editor
    void updateEditor();
    // Renders shapes to the screen for Breakout
    void renderBreakout();
    // Renders shapes to the screen for Platformer
    void renderPlatformer();
    // Renders shapes to the screen for Editor
    void renderEditor();
    // loop that runs forever for Breakout
    void loopBreakout();
    // loop that runs forever for Platformer
    void loopPlatformer();
    // loop that runs forever for the Editor
    void loopEditor();

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
     * Gets all the language files
     */
    void getLanguages();

    /**
     * Changes the language
     *
     * @param langIndex: Index of vector of language files to read from
     */
    void changeLanguage(int langIndex);

    /**
     * edits the tile at the given position to be of new type blockStr
     * @param blockPos the position of the tile we will edit.
     * @param blockStr a strig correlating the new type of block it will be.
     */
    void editTile(Vector3D blockPos, std::string blockStr);

    //helper for editing the editorTile files in real time.
    void levelHelper(int lvlInt);

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


    //level editor variables
    int edt_currLevelIndex;

    GameObject edt_cursor;
    Vector3D edt_cursorBlockPos = Vector3D(0,0);

    std::vector<Level *> edt_levels;
    std::vector<Textbox> edt_menuTexts;

    bool lvlSelectMode = true;

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
