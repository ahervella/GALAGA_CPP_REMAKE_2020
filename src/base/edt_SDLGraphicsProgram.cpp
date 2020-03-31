//
// Created by Alejandro Hervella on 2/6/20.
//

#include "edt_SDLGraphicsProgram.hpp"

#include <algorithm>
//#include "TinyMath.hpp"
#include <memory>

//for getting files from directory
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "Textbox.hpp"

#include "GameObject.hpp"

#include "TinyMath.hpp"

#include "Level.hpp"

//#define SCREEN_WIDTH 500
//#define SCREEN_HEIGHT 500
#define SCREEN_TICKS_PER_FRAME 1000/60

//#define GAME_OVER_TEXT "Game Over! ~(O ^ o)~"
#define GAME_WON_TEXT "You won! Space for next Level!"
#define NA_TEXT "N/A"
#define TEXT_SIZE 20
#define TEXT_X_ANCHOR Constants::Game::SCREEN_UNIT_WIDTH * Constants::Game::UNIT / 2 - 175
#define TEXT_Y_ANCHOR Constants::Game::SCREEN_UNIT_HEIGHT * Constants::Game::UNIT / 2 - 100

std::vector<Level *> edt_levels;
std::vector<Textbox> edt_menuTexts;

bool lvlSelectMode = true;

int edt_currLevelIndex;

GameObject edt_cursor;
Vector3D edt_cursorBlockPos = Vector3D(0,0);

Textbox mainMenuText = Textbox("MAIN MENU: ", TEXT_SIZE, TEXT_X_ANCHOR, TEXT_Y_ANCHOR);
/*
Textbox centerText = Textbox(NA_TEXT, 20, SCREEN_WIDTH / 2 - 175, SCREEN_HEIGHT / 2 - 100);
CounterTextbox livesText = CounterTextbox(NA_TEXT, DEFAULT_LIVES, 15, 15, 3);
CounterTextbox scoreText = CounterTextbox(NA_TEXT, 0, 15, SCREEN_WIDTH - 75, 3);
int currLevelIndex = 0;


std::map<std::string, std::string> gameTexts{
        {"LANG_MENU", NA_TEXT},
        {"DONE", NA_TEXT},
        {"START", NA_TEXT},
        {"LOSE", NA_TEXT},
        {"WIN", NA_TEXT},
        {"LIVES", NA_TEXT},
        {"SCORE", NA_TEXT}
};
*/


edt_SDLGraphicsProgram::edt_SDLGraphicsProgram(){}

edt_SDLGraphicsProgram::~edt_SDLGraphicsProgram(){}

void edt_SDLGraphicsProgram::initLevelLoading(){

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





    for(Level* lvl : edt_levels){
        delete lvl;
    }

    edt_levels.clear();
    //getLanguages();
    //Default to English
    //changeLanguage(0);

    std::ifstream fin;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;


    std::string resourceConfigsPath = getResourcePath("level_config");

    //open directory path
    dp = opendir(resourceConfigsPath.c_str());

    if (dp == NULL)
    {
        //std::cout << "Error opening " << resourceConfigsPath << std::endl;
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
        edt_menuTexts.push_back(Textbox(filename, TEXT_SIZE/1.5, TEXT_X_ANCHOR, TEXT_Y_ANCHOR + textYPosIncrementer));

        textYPosIncrementer += 30;

        //std::cout << "File pathhhh: " << filePath << std::endl;

        edt_levels.push_back(new Level(filePath));


    }

}

void edt_SDLGraphicsProgram::editTile(Vector3D blockPos, std::string blockStr){
    Level* lvl = edt_levels[edt_currLevelIndex];

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




// Update OpenGL
void edt_SDLGraphicsProgram::update() {
    //render background
    SDL_SetRenderDrawColor(gRenderer, 0x22, 0x22, 0x22, 0xFF);
    SDL_RenderClear(gRenderer);

    if(!lvlSelectMode){
        edt_cursor.pos = Vector3D(
                edt_cursorBlockPos.x * Constants::Game::UNIT,
                edt_cursorBlockPos.y * Constants::Game::UNIT);
    }


    //Check for collision between player and blocks.
    //Here we can determine collisions between player and blocks below/to the side of a player
    //And here we can promptly update what needs to get updated based on that

    //TODO: need to also find a way to break boxes and remove them in the individual level objs
    //if we want to break blocks

    /*

    std::vector<GameObject> objs = levels[currLevelIndex].levelObjs;//levels[currLevelIndex].levelObjs;
    Player* p = &levels[currLevelIndex].player;
    //If we have a collision, let the player figure out what to do based on where the collision is from
    //TODO: For future, figure out

    int indexToRemove = -1;

    for(size_t i = 0; i < objs.size(); ++i) {

        std::cout<<"Index: "<<i<<std::endl;
        if (p->collisionUpdate(p->isColliding(objs[i]), objs[i].tag)){
              indexToRemove = i;
        }

    }

    if (indexToRemove != -1){
          levels[currLevelIndex].levelObjs
          .erase(levels[currLevelIndex].levelObjs.begin() + indexToRemove);
    }

    */

    //all velocities should be final at this point in frame for player, apply to position
    //std::cout<<"size: " + std::to_string(objs.size())<<std::endl;
    //p->move();

    //p->update();



}


// Render
// The render function gets called once per loop
//after update() function in the main loop (background
//and positions already changed by now)
void edt_SDLGraphicsProgram::render() {

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
        edt_levels[edt_currLevelIndex]->render(getSDLRenderer());
        edt_cursor.render(getSDLRenderer());
    }


    SDL_RenderPresent(getSDLRenderer());
    SDL_DestroyTexture(mainMenuTextImg);

    for (SDL_Texture *txtBoxTexture : txtboxs) {
        SDL_DestroyTexture(txtBoxTexture);
    }


}


void levelHelper(int lvlInt){

    //std::cout<<edt_levels.size()<<"  level sizessss"<<std::endl;
    int currLevel = edt_levels.size() + 1;

    while ((size_t)edt_levels.size() < (size_t)lvlInt) {

        //make new file
        std::ofstream outfile(getResourcePath("level_config") + "lvl0" + std::to_string(currLevel) + ".cfg");

        std::string conts;
        for (int i = 0; i < Constants::Game::SCREEN_UNIT_HEIGHT; i++) {

            for (int k = 0; k < Constants::Game::SCREEN_UNIT_WIDTH; k++) {

                if (i == 0 && k == 0) {
                    conts += "P";
                    continue;
                }

                conts += ".";
            }
            if (i != Constants::Game::SCREEN_UNIT_HEIGHT - 1) {
                conts += "\n";
            }
        }

        outfile << conts << std::endl;

        outfile.close();


        edt_levels.push_back(new Level(getResourcePath("level_config") + "lvl0" + std::to_string(currLevel) + ".cfg"));
        currLevel++;
    }

}



//Loops forever!
void edt_SDLGraphicsProgram::loop() {
    //start of program

    //
    edt_cursor = GameObject(Vector3D(0,
               (Constants::Game::SCREEN_UNIT_HEIGHT - Constants::Game::BLOCK_UNIT_DIM.y) * Constants::Game::UNIT, 0),
               Vector3D(Constants::Game::BLOCK_UNIT_DIM.x * Constants::Game::UNIT,
               Constants::Game::BLOCK_UNIT_DIM.y * Constants::Game::UNIT, 0),
               -1,
               {0, 0, 250, 100},
               {255, 0, 0, 255},
               Constants::TexturePath::CLEAR,
               gRenderer);

    //set alpha channel on
    SDL_SetRenderDrawBlendMode(getSDLRenderer(), SDL_BLENDMODE_BLEND);

    //doing this here because can't call virtual function in constructor!
    initLevelLoading();


    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    bool rightJustPressed = false;
    bool leftJustPressed = false;
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

                        if(edt_cursorBlockPos.y < Constants::Game::SCREEN_UNIT_HEIGHT){
                            edt_cursorBlockPos = Vector3D(edt_cursorBlockPos.x, edt_cursorBlockPos.y + 1);
                        }
                        //left
                        break;

                    case SDLK_RIGHT:
                        if(lvlSelectMode){break;}

                        if(edt_cursorBlockPos.x < Constants::Game::SCREEN_UNIT_WIDTH){
                            edt_cursorBlockPos = Vector3D(edt_cursorBlockPos.x + 1, edt_cursorBlockPos.y);
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

                        edt_levels[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                        lvlSelectMode = false;
                        //1 key
                        break;
                    case SDLK_2:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 1;

                        levelHelper(2);


                        edt_levels[edt_currLevelIndex]->constructLevel(getSDLRenderer());

                        lvlSelectMode = false;
                        //2 key
                        break;


                    case SDLK_3:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 2;

                        levelHelper(3);


                        edt_levels[edt_currLevelIndex]->constructLevel(getSDLRenderer());

                        lvlSelectMode = false;
                        break;

                    case SDLK_4:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 3;

                        levelHelper(4);


                        edt_levels[edt_currLevelIndex]->constructLevel(getSDLRenderer());

                        lvlSelectMode = false;
                        break;

                    case SDLK_5:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 4;

                        levelHelper(5);


                        edt_levels[edt_currLevelIndex]->constructLevel(getSDLRenderer());

                        lvlSelectMode = false;
                        break;

                    case SDLK_6:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 5;

                        levelHelper(6);


                        edt_levels[edt_currLevelIndex]->constructLevel(getSDLRenderer());

                        lvlSelectMode = false;
                        break;

                    case SDLK_c:
                        editTile(edt_cursorBlockPos, "c");
                        //edt_levels[edt_currLevelIndex].writeToCfgFile("s");
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
                        initLevelLoading();
                        break;
                }
            }



            if (e.type == SDL_KEYUP){
                switch( e.key.keysym.sym ){
                    case SDLK_RIGHT:
                        //if(rightJustPressed){
                            rightJustPressed = false;
                        //}
                        break;
                    case SDLK_LEFT:
                       // if(leftJustPressed){
                            leftJustPressed = false;
                        //}
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

    for(Level* lvl : edt_levels){
        delete lvl;
    }

}




// Get Pointer to Window
//(private) utility in case we want to get the window
SDL_Window *edt_SDLGraphicsProgram::getSDLWindow() {
    return gWindow;
}

// Get Pointer to Renderer
//(private) utility in case we want to get the window
SDL_Renderer *edt_SDLGraphicsProgram::getSDLRenderer() {
    return gRenderer;
}



