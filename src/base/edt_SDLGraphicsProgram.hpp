//
// Created by Alejandro Hervella on 3/23/20.
//

#include"SDLGraphicsProgram.hpp"
#include"TinyMath.hpp"

#ifndef PROJ2_PLATFORMER_WALUIGI_SEND_TWEET_EDT_SDLGRAPHICSPROGRAM_HPP
#define PROJ2_PLATFORMER_WALUIGI_SEND_TWEET_EDT_SDLGRAPHICSPROGRAM_HPP




class edt_SDLGraphicsProgram : public SDLGraphicsProgram{
public:

    edt_SDLGraphicsProgram();
    ~edt_SDLGraphicsProgram();

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

    // Get Pointer to Window
    //Blah
    SDL_Window* getSDLWindow();
    // Get Pointer to Renderer
    SDL_Renderer* getSDLRenderer();

    void editTile(Vector3D blockPos, std::string blockStr);


};


#endif //PROJ2_PLATFORMER_WALUIGI_SEND_TWEET_EDT_SDLGRAPHICSPROGRAM_HPP
