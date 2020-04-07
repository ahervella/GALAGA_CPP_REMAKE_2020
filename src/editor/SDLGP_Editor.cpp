/*
 * SDLGP_Editor.cpp
 *
 *  Created on: Apr 6, 2020
 *      Author: Karina
 */

#include "SDLGP_Editor.hpp"

Textbox mainMenuText;

//editor variables:
#define TEXT_SIZE 20

#define TEXT_X_ANCHOR_BREAKOUT Constants::Breakout::Game::SCREEN_UNIT_WIDTH * Constants::Breakout::Game::UNIT / 2 - 175
#define TEXT_Y_ANCHOR_BREAKOUT Constants::Breakout::Game::SCREEN_UNIT_HEIGHT * Constants::Breakout::Game::UNIT / 2 - 100


#define TEXT_X_ANCHOR_PLATFORMER Constants::Platformer::Game::SCREEN_UNIT_WIDTH * Constants::Platformer::Game::UNIT / 2 - 175
#define TEXT_Y_ANCHOR_PLATFORMER Constants::Platformer::Game::SCREEN_UNIT_HEIGHT * Constants::Platformer::Game::UNIT / 2 - 100

#define TEXT_X_ANCHOR_GALAGA Constants::Galaga::Game::SCREEN_UNIT_WIDTH * Constants::Galaga::Game::UNIT / 2 - 175
#define TEXT_Y_ANCHOR_GALAGA Constants::Galaga::Game::SCREEN_UNIT_HEIGHT * Constants::Galaga::Game::UNIT / 2 - 100

SDLGP_Editor::SDLGP_Editor(int gc)
{
	gameCode = gc;
}

SDLGP_Editor::~SDLGP_Editor()
{

}

void SDLGP_Editor::update() {
    //render background
    SDL_SetRenderDrawColor(gRenderer, 0x22, 0x22, 0x22, 0xFF);
    SDL_RenderClear(gRenderer);

    switch(gameCode){
        case 1:
            //break;
            if (!lvlSelectMode) {
                edt_cursor.pos = Vector3D(
                        edt_cursorBlockPos.x * Constants::Breakout::Game::UNIT,
                        edt_cursorBlockPos.y * Constants::Breakout::Game::UNIT);
            }
            break;
        case 2:
            if (!lvlSelectMode) {
                edt_cursor.pos = Vector3D(
                        edt_cursorBlockPos.x * Constants::Platformer::Game::UNIT,
                        edt_cursorBlockPos.y * Constants::Platformer::Game::UNIT);
            }
            break;
        case 3:
            if (!lvlSelectMode) {
                edt_cursor.pos = Vector3D(
                        edt_cursorBlockPos.x * Constants::Galaga::Game::UNIT,
                        edt_cursorBlockPos.y * Constants::Galaga::Game::UNIT);
            }
            break;
    }

}

void SDLGP_Editor::render() {

	//TODO: See if this needs to be removed to get plain black image for Breakout Editor and kept for Platformer editor
    SDL_RenderCopy(getSDLRenderer(), *backgroundImage, NULL, NULL);


    //"Main menu" text
    SDL_Texture *mainMenuTextImg = renderText(mainMenuText.text, mainMenuText.textResPath,
                                              mainMenuText.clr, mainMenuText.textSize, getSDLRenderer());
    //selectable textboxes below
    std::vector < SDL_Texture * > txtboxs;

    //if in menu select, render the mainMenuText and the level texts
    if (lvlSelectMode) {

        renderTexture(mainMenuTextImg, getSDLRenderer(), mainMenuText.x, mainMenuText.y);

        //std::cout<edt_menuTexts.size()<<std:endl;
        for (Textbox txtbox : edt_menuTexts) {
            SDL_Texture *textImg = renderText(txtbox.text, txtbox.textResPath, txtbox.clr, txtbox.textSize,
                                              getSDLRenderer());
            renderTexture(textImg, getSDLRenderer(), txtbox.x, txtbox.y);

            txtboxs.push_back(textImg);
        }
    } else {

        //render current level
        switch(gameCode){
            case 1:
                edt_levels_breakout[edt_currLevelIndex]->render(getSDLRenderer());
                break;
            case 2:
                edt_levels_platformer[edt_currLevelIndex]->render(getSDLRenderer());
                break;
            case 3:
                edt_levels_galaga[edt_currLevelIndex]->render(getSDLRenderer());
                break;
        }

        edt_cursor.render(getSDLRenderer());
    }

    SDL_RenderPresent(getSDLRenderer());
    SDL_DestroyTexture(mainMenuTextImg);

    for (SDL_Texture *txtBoxTexture : txtboxs) {
        SDL_DestroyTexture(txtBoxTexture);
    }

}

void SDLGP_Editor::loop() {

    //start of program

    switch(gameCode){
        case 1:
            edt_cursor = GameObject(Vector3D(0,
                                             (Constants::Breakout::Game::SCREEN_UNIT_HEIGHT) * Constants::Breakout::Game::UNIT, 0),
                                    Vector3D(Constants::Breakout::Game::BLOCK_UNIT_DIM.x * Constants::Breakout::Game::UNIT,
                                             Constants::Breakout::Game::BLOCK_UNIT_DIM.y * Constants::Breakout::Game::UNIT, 0),
                                    -1,
                                    {0, 0, 250, 100},
                                    {255, 0, 0, 255},
                                    Constants::Breakout::TexturePath::CLEAR,
                                    gRenderer);
            break;
        case 2:
            edt_cursor = GameObject(Vector3D(0,
                                             (Constants::Platformer::Game::SCREEN_UNIT_HEIGHT) * Constants::Platformer::Game::UNIT, 0),
                                    Vector3D(Constants::Platformer::Game::BLOCK_UNIT_DIM.x * Constants::Platformer::Game::UNIT,
                                             Constants::Platformer::Game::BLOCK_UNIT_DIM.y * Constants::Platformer::Game::UNIT, 0),
                                    -1,
                                    {0, 0, 250, 100},
                                    {255, 0, 0, 255},
                                    Constants::Platformer::TexturePath::CLEAR,
                                    gRenderer);
            break;
        case 3:
            edt_cursor = GameObject(Vector3D(0,
                                             (Constants::Galaga::Game::SCREEN_UNIT_HEIGHT) * Constants::Galaga::Game::UNIT, 0),
                                    Vector3D(Constants::Galaga::Game::BADY_UNIT_DIM.x * Constants::Galaga::Game::UNIT,
                                             Constants::Galaga::Game::BADY_UNIT_DIM.y * Constants::Galaga::Game::UNIT, 0),
                                    -1,
                                    {0, 0, 250, 100},
                                    {255, 0, 0, 255},
                                    Constants::Galaga::TexturePath::CLEAR,
                                    gRenderer);
            break;
    }


    //set alpha channel on
    SDL_SetRenderDrawBlendMode(getSDLRenderer(), SDL_BLENDMODE_BLEND);

    //doing this here because can't call virtual function in constructor!
    initLevelLoadingEditor();


    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    Mix_PlayMusic(*(backgroundMusic), -1);

    //std::cout<<"working3"<<std::endl;

    // While application is running
    while (!quit) {


        //Start timer to find out how much time this frame takes
        unsigned int startFrame = SDL_GetTicks();
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if (e.type == SDL_QUIT) {
                quit = true;

            }
            //logic for pressing keys here makes it so the latest left or right key pressed
            //down takes priority for direction (so holding both down will not make it stay still).

            if (e.type == SDL_KEYDOWN ) {

                switch( e.key.keysym.sym ){

                    case SDLK_UP:
                        if(lvlSelectMode){break;}
                        //up arrow key shtuff

                        if(edt_cursorBlockPos.y > 0){
                            edt_cursorBlockPos = Vector3D(edt_cursorBlockPos.x, edt_cursorBlockPos.y - 1);
                        }

                        break;

                    case SDLK_DOWN:
                        if(lvlSelectMode){break;}

                        switch(gameCode){
                            case 1:
                                if(edt_cursorBlockPos.y < Constants::Breakout::Game::SCREEN_UNIT_HEIGHT){
                                    edt_cursorBlockPos = Vector3D(edt_cursorBlockPos.x, edt_cursorBlockPos.y + 1);
                                }
                                break;
                            case 2:
                                if(edt_cursorBlockPos.y < Constants::Platformer::Game::SCREEN_UNIT_HEIGHT){
                                    edt_cursorBlockPos = Vector3D(edt_cursorBlockPos.x, edt_cursorBlockPos.y + 1);
                                }
                                break;
                            case 3:
                                //galaga stuff
                                break;
                        }

                        //left
                        break;

                    case SDLK_RIGHT:
                        if(lvlSelectMode){break;}

                        switch(gameCode){
                            case 1:
                                if(edt_cursorBlockPos.x < Constants::Breakout::Game::SCREEN_UNIT_WIDTH){
                                    edt_cursorBlockPos = Vector3D(edt_cursorBlockPos.x + 1, edt_cursorBlockPos.y);
                                }
                                break;
                            case 2:
                                if(edt_cursorBlockPos.x < Constants::Platformer::Game::SCREEN_UNIT_WIDTH){
                                    edt_cursorBlockPos = Vector3D(edt_cursorBlockPos.x + 1, edt_cursorBlockPos.y);
                                }
                                break;
                            case 3:
                                //galaga stuff
                                break;
                        }


                        //left
                        break;
                    case SDLK_LEFT:
                        if(lvlSelectMode){break;}

                        if(edt_cursorBlockPos.x > 0){
                            edt_cursorBlockPos = Vector3D(edt_cursorBlockPos.x - 1, edt_cursorBlockPos.y);
                        }

                        //right
                        break;
                    case SDLK_1:
                        if(!lvlSelectMode){break;}
                        //doshit
                        edt_currLevelIndex = 0;

                        switch(gameCode){
                            case 1:
                                edt_levels_breakout[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case 2:
                                edt_levels_platformer[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case 3:
                                //galaga stuff
                                break;
                        }


                        lvlSelectMode = false;
                        //1 key
                        break;
                    case SDLK_2:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 1;

                        levelHelper(2);

                        switch(gameCode){
                            case 1:
                                edt_levels_breakout[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case 2:
                                edt_levels_platformer[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case 3:
                                //galaga stuff
                                break;
                        }


                        lvlSelectMode = false;
                        //2 key
                        break;


                    case SDLK_3:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 2;

                        levelHelper(3);

                        switch(gameCode){
                            case 1:
                                edt_levels_breakout[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case 2:
                                edt_levels_platformer[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case 3:
                                //galaga stuff
                                break;
                        }

                        lvlSelectMode = false;
                        break;

                    case SDLK_4:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 3;

                        levelHelper(4);

                        switch(gameCode){
                            case 1:
                                edt_levels_breakout[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case 2:
                                edt_levels_platformer[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case 3:
                                //galaga stuff
                                break;
                        }

                        lvlSelectMode = false;
                        break;

                    case SDLK_5:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 4;

                        levelHelper(5);

                        switch(gameCode){
                            case 1:
                                edt_levels_breakout[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case 2:
                                edt_levels_platformer[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case 3:
                                //galaga stuff
                                break;
                        }

                        lvlSelectMode = false;
                        break;

                    case SDLK_6:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 5;

                        levelHelper(6);

                        switch(gameCode){
                            case 1:
                                edt_levels_breakout[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case 2:
                                edt_levels_platformer[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case 3:
                                //galaga stuff
                                break;
                        }

                        lvlSelectMode = false;
                        break;

                    case SDLK_c:
                        editTile(edt_cursorBlockPos, "c");
                        //edt_levels_platformer[edt_currLevelIndex].writeToCfgFile("s");
                        //do shit for collectible
                        break;
                    case SDLK_b:
                        editTile(edt_cursorBlockPos, "b");
                        //do shit for block
                        break;

                    case SDLK_p:
                        editTile(edt_cursorBlockPos, "p");
                        //do shit for player
                        break;

                    case SDLK_g:
                        editTile(edt_cursorBlockPos, "g");
                        //do shit for goal
                        break;

                    case SDLK_PERIOD:
                        editTile(edt_cursorBlockPos, ".");
                        //do shit for goal
                        break;

                    case SDLK_q:

                        quit = true;
                        break;
                    case SDLK_r:
                        //levels[currLevelIndex].constructLevel(getSDLRenderer());
                        break;
                    case SDLK_m:

                        if(lvlSelectMode){break;}
                        lvlSelectMode = true;
                        initLevelLoadingEditor();
                        break;
                }
            }

        }

        // Update our scene
        update();

        // Render
        render();

        //Cap frame rate
        unsigned int endFrame = SDL_GetTicks();
        unsigned int frameTick = endFrame - startFrame;
        if (frameTick < SCREEN_TICKS_PER_FRAME) {
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTick);
        }
    }

    //Disable text input
    SDL_StopTextInput();

    for(Level* lvl : edt_levels_platformer){
        delete lvl;
    }
}

void SDLGP_Editor::initLevelLoadingEditor() {

    std::cout<<"Greetings user! Welcome to the level editor!" <<std::endl<< std::endl<<
             "Create up to 6 unique levels!"<<std::endl<< std::endl<<
             "To navigate within the main menu, press the corresponding ROW number (from the top) on the keyboard to select a level to edit. For example, if level 3 is on the 5th line, pressing 5"
             "will edit level number 3."<<std::endl<< std::endl<<
             "To create more levels by simply using your number keys on your keyboard (up to 6) to create the desired amount levels, and to begin editing that level number as well." <<std::endl<<std::endl<<
             "Once in the editor, navigate with the arrow keys, to highlight a different tile." <<std::endl<< std::endl<<
             "Press the b key to make the desirable tile a block" <<std::endl<< std::endl<<
             "Press the c key to make the tile a coin!" <<std::endl<< std::endl<<
             "Press the p key to make the tile the starting point of a player" <<std::endl<< std::endl<<
             "Press the e key to make the tile an enemy starting point"
             "Press the . key to clear the tile" <<std::endl<< std::endl<<
             "Press the m key to return to the main menu from the editor." <<std::endl<< std::endl<<
             "Press the q key to quit at any time!"<<std::endl<< std::endl;

    std::cout<<"the pipe is leaking"<<std::endl;
    switch(gameCode){
        case 1:
            mainMenuText = Textbox("MAIN MENU: ", TEXT_SIZE, TEXT_X_ANCHOR_BREAKOUT, TEXT_Y_ANCHOR_BREAKOUT);

            for(BreakoutLevel* lvl : edt_levels_breakout){
                delete lvl;
            }

            edt_levels_breakout.clear();

            break;
        case 2:
            mainMenuText = Textbox("MAIN MENU: ", TEXT_SIZE, TEXT_X_ANCHOR_PLATFORMER, TEXT_Y_ANCHOR_PLATFORMER);

            for(Level* lvl : edt_levels_platformer){
                delete lvl;
            }

            edt_levels_platformer.clear();
            break;
        case 3:
            mainMenuText = Textbox("MAIN MENU: ", TEXT_SIZE, TEXT_X_ANCHOR_GALAGA, TEXT_Y_ANCHOR_GALAGA);

            for(GalagaLevel* lvl : edt_levels_galaga){
                delete lvl;
            }

            edt_levels_platformer.clear();
            break;
    }

    //getLanguages();
    //Default to English
    //changeLanguage(0);

    std::ifstream fin;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;

    std::string resourceConfigsPath = "";

    //TODO: don't hard code to only work for editor here
    switch(gameCode){
        case 1:
            resourceConfigsPath = getResourcePath("breakout/level_config");
            break;
        case 2:
            resourceConfigsPath = getResourcePath("platformer/level_config");
            break;
        case 3:
            //galaga resourceConfigsPath...
            break;
    }

    //open directory path
    dp = opendir(resourceConfigsPath.c_str());

    if (dp == NULL)
    {
        std::cout << "Error opening " << resourceConfigsPath << std::endl;
        return;
    }


    int textYPosIncrementer = 30;

    //load level configs
    while ((dirp = readdir( dp )))
    {

        std::string contents;
        std::string skipStringMessage = "Skipped: " + (std::string(dirp->d_name));


        std::string filePath = resourceConfigsPath + dirp->d_name;

        if ((std::string(dirp->d_name)).find(".") == 0){
            //std::cout<< skipStringMessage <<std::endl<<std::endl;
            continue;}

        //prevent from getting other hidden non text file paths
        if (stat( filePath.c_str(), &filestat )) {
            //std::cout<<skipStringMessage<<std::endl<<std::endl;
            continue;}

        if (S_ISDIR( filestat.st_mode ))  {
            //std::cout<<skipStringMessage<<std::endl<<std::endl;
            continue;}

        std::size_t lastSlash = filePath.find_last_of("/");

        std::string filename = filePath.substr(lastSlash+1);

        switch(gameCode){
            case 1:
                edt_menuTexts.push_back(Textbox(filename, TEXT_SIZE/1.5, TEXT_X_ANCHOR_BREAKOUT, TEXT_Y_ANCHOR_BREAKOUT + textYPosIncrementer));
                edt_levels_breakout.push_back(new BreakoutLevel(filePath));
                break;
            case 2:
                edt_menuTexts.push_back(Textbox(filename, TEXT_SIZE/1.5, TEXT_X_ANCHOR_PLATFORMER, TEXT_Y_ANCHOR_PLATFORMER + textYPosIncrementer));
                edt_levels_platformer.push_back(new Level(filePath));
                break;
            case 3:
                //galaga stuff
                break;
        }


        textYPosIncrementer += 30;

        //std::cout << "File pathhhh: " << filePath << std::endl;
    }

}

void SDLGP_Editor::editTile(Vector3D blockPos, std::string blockStr) {

    switch(gameCode){
        case 1:
            editTileBreakout(blockPos, blockStr);
            break;
        case 2:
            editTilePlatformer(blockPos, blockStr);
            break;
        case 3:
            //galaga stuff
            break;
    }

}

void SDLGP_Editor::editTileBreakout(Vector3D blockPos, std::string blockStr) {


    BreakoutLevel* lvl = edt_levels_breakout[edt_currLevelIndex];

    std::vector<std::string> contents;// = lvl->contents;

    //bool clearBlock = false;
    //bool newBlock = true;

    for (std::string blah : lvl->contents){
        contents.push_back(blah);
    }


    switch(blockStr[0]){
        case 'b':
        case 'B':

            contents[blockPos.y].replace(blockPos.x, 1, "X");
            break;
    }

    std::string newContents;

    for(size_t i = 0; i < contents.size(); i++){
        newContents += contents[i];
        if (i != contents.size() -1){
            newContents += "\n";
        }
    }

    lvl->writeToCfgFile(newContents, gRenderer);

}

void SDLGP_Editor::editTilePlatformer(Vector3D blockPos, std::string blockStr) {

    Level* lvl = edt_levels_platformer[edt_currLevelIndex];

    std::vector<std::string> contents;// = lvl->contents;

    //bool clearBlock = false;
    //bool newBlock = true;

    for (std::string blah : lvl->contents){
        contents.push_back(blah);
    }


    switch(blockStr[0]){
        case 'b':
        case 'B':

            contents[blockPos.y].replace(blockPos.x, 1, "X");
            break;

        case 'c':
        case 'C':
            contents[blockPos.y].replace(blockPos.x, 1, "C");
            break;

        case '.':
            contents[blockPos.y].replace(blockPos.x, 1, ".");
            break;

        case 'G':
        case 'g':
            contents[blockPos.y].replace(blockPos.x, 1, "G");


        case 'p':
        case 'P':
            for (size_t i = 0; i < contents.size(); i++){
                for (size_t j = 0; j < contents[i].length(); j++){
                    if (contents[i][j] == 'P' || contents[i][j] == 'p'){
                        contents[i].replace(j, 1, ".");
                    }
                }
            }

            contents[blockPos.y].replace(blockPos.x, 1, "P");
            break;

    }

    std::string newContents;

    for(size_t i = 0; i < contents.size(); i++){
        newContents += contents[i];
        if (i != contents.size() -1){
            newContents += "\n";
        }
    }

    lvl->writeToCfgFile(newContents, gRenderer);

    }

void SDLGP_Editor::editTileGalaga(Vector3D blockPos, int badyTypeInt) {

    GalagaLevel* lvl = edt_levels_galaga[edt_currLevelIndex];

    std::vector<std::string> contents;

    for (std::string blah : lvl->contents){
        contents.push_back(blah);
    }


            contents[blockPos.y].replace(blockPos.x, 1, std::to_string(badyTypeInt));



    std::string newContents;

    for(size_t i = 0; i < contents.size(); i++){
        newContents += contents[i];
        if (i != contents.size() -1){
            newContents += "\n";
        }
    }

    lvl->writeToCfgFile(newContents, gRenderer);

}

void SDLGP_Editor::levelHelper(int lvlInt) {

    //std::cout<<edt_levels_platformer.size()<<"  level sizessss"<<std::endl;

    int currLevel = 0;
    int sizeCompare = 0;
    switch(gameCode){
        case 1:
            sizeCompare = edt_levels_breakout.size();
            currLevel = edt_levels_breakout.size() + 1;
            break;
        case 2:
            sizeCompare = edt_levels_platformer.size();
            currLevel = edt_levels_platformer.size() + 1;
            break;
        case 3:
            sizeCompare = edt_levels_galaga.size();
            currLevel = edt_levels_galaga.size() + 1;
            break;
    }

    while ((size_t)sizeCompare < (size_t)lvlInt) {

        //make new file
        std::ofstream outfile(getResourcePath("level_config") + "lvl0" + std::to_string(currLevel) + ".cfg");

        std::string conts;

        //this is for adding any default levels if the editor user decides to edit say level 4 and currently
        //only level 1 and 2 exist. Will loop through all and by default put the player at the top left
        switch(gameCode){
            case 1:
                for (int i = 0; i < Constants::Breakout::Game::SCREEN_UNIT_HEIGHT; i++) {

                    for (int k = 0; k < Constants::Breakout::Game::SCREEN_UNIT_WIDTH; k++) {

                        if (i == 0 && k == 0) {
                            conts += "P";
                            continue;
                        }

                        conts += ".";
                    }
                    if (i != Constants::Breakout::Game::SCREEN_UNIT_HEIGHT - 1) {
                        conts += "\n";
                    }
                }
                break;

            case 2:
                for (int i = 0; i < Constants::Platformer::Game::SCREEN_UNIT_HEIGHT; i++) {

                    for (int k = 0; k < Constants::Platformer::Game::SCREEN_UNIT_WIDTH; k++) {

                        if (i == 0 && k == 0) {
                            conts += "P";
                            continue;
                        }

                        conts += ".";
                    }
                    if (i != Constants::Platformer::Game::SCREEN_UNIT_HEIGHT - 1) {
                        conts += "\n";
                    }
                }
                break;

            case 3:
                //galaga doesn't need to know the player starting position
                for (int i = 0; i < Constants::Platformer::Game::SCREEN_UNIT_HEIGHT; i++) {

                    for (int k = 0; k < Constants::Platformer::Game::SCREEN_UNIT_WIDTH; k++) {


                        conts += ".";
                    }
                    if (i != Constants::Platformer::Game::SCREEN_UNIT_HEIGHT - 1) {
                        conts += "\n";
                    }
                }
                break;
        }

        outfile << conts << std::endl;

        outfile.close();

        switch(gameCode){
            case 1:
                edt_levels_breakout.push_back(new BreakoutLevel(getResourcePath("level_config") + "lvl0" + std::to_string(currLevel) + ".cfg"));
                break;
            case 2:
                edt_levels_platformer.push_back(new Level(getResourcePath("level_config") + "lvl0" + std::to_string(currLevel) + ".cfg"));
                break;
            case 3:
                edt_levels_galaga.push_back(new GalagaLevel(getResourcePath("level_config") + "lvl0" + std::to_string(currLevel) + ".cfg"));
                break;
        }

        currLevel++;
    }
}

