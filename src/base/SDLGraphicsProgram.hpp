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

#include "../platformer/Level.hpp"
#include "BreakoutLevel.hpp"
#include "GalagaLevel.hpp"

/**
 * The class that runs our games/level editors. Maintains the code to run each game/editor, and the game/editor is determined by a
 * game code that is given as a parameter to the SDLGraphicsProgram constructor. The game codes are as follows:
 * -3 = LevelEditor Galaga (To Do)
 * -2 = LevelEditor Platformer
 * -1 = LevelEditor Breakout
 * 1 = Breakout
 * 2 = Platformer
 * 3 = Galaga (To Do)
 *
 */
class SDLGraphicsProgram{
public:

	/*GameCodes:
	 * -3 = LevelEditor Galaga
	 * -2 = LevelEditor Platformer
	 * -1 = LevelEditor Breakout
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

    /**
     * Initializes the level and config files in the resource manager (used in a level editor)
     */
    void initLevelLoadingEditor();

    /**
     * Initializes the level and config files in the resource manager (used for game levels during gameplay)
     */
    void initLevelLoadingGames();

    // Per frame update for Breakout
    void updateBreakout();
    // Per frame update for Platformer
    void updatePlatformer();
    // Per frame update for Galaga
    void updateGalaga();
    // Per frame update for Editor
    void updateEditor();
    // Renders shapes to the screen for Breakout
    void renderBreakout();
    // Renders shapes to the screen for Platformer
    void renderPlatformer();
    // Renders shapes to the screen for Galaga
    void renderGalaga();
    // Renders shapes to the screen for Editor
    void renderEditor();
    // loop that runs forever for Breakout
    void loopBreakout();
    // loop that runs forever for Platformer
    void loopPlatformer();
    // loop that runs forever for the Galaga
    void loopGalaga();
    // loop that runs forever for the Editor
    void loopEditor();

    /**
     * Edits a tile in the breakout level editor
     */
    void editTileBreakout(Vector3D blockPos, std::string blockStr);
    /**
     * Edits a tile in the platformer level editor
     */
    void editTilePlatformer(Vector3D blockPos, std::string blockStr);
    /**
     * Edits a tile in the galaga level editor
     */
    void editTileGalaga(Vector3D blockPos, std::string blockStr);
    /**
     * edits the tile at the given position to be of new type blockStr
     * @param blockPos the position of the tile we will edit.
     * @param blockStr a strig correlating the new type of block it will be.
     */
    void editTile(Vector3D blockPos, std::string blockStr);

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
     * Handles the loss of a life in the games supported by SDLGraphicsProgram
     */
    void loseLife();

    /**
     * Handles the game ending in the games supported by SDLGraphicsProgram
     */
    void gameOver();


    //helper for editing the editorTile files in real time.
    void levelHelper(int lvlInt);

    //breakout methods
    void resetToLevel();
    void checkBrickCollision(float &newAngle, int &collisionID, int &brickCollisionIndex);
    void removeBrick(int rmIndex);
    void updateBallCollisions();

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

    std::vector<Level *> edt_levels_platformer;
    std::vector<BreakoutLevel *> edt_levels_breakout;
    std::vector<GalagaLevel *> edt_levels_galaga;
    std::vector<Textbox> edt_menuTexts;

    bool lvlSelectMode = true;

    //levels for each type of game
    std::vector<BreakoutLevel> BreakoutLevels;
    std::vector<Level> PlatformerLevels;
    std::vector<GalagaLevel> GalagaLevels;

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
