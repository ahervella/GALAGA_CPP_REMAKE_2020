#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "TinyMath.hpp"
#include "ResourceManager.hpp"

#include <stdlib.h>
#include <string>
#include <memory>
#include <SDL.h>
#include <SDL_mixer.h>

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500


#define BRICK_WIDTHS 50
#define BRICK_HEIGHTS 10

//#define DEFAULT_LIVES 3


#define GRAV 0.1

#define MAX_SPEED_X 3
#define MAX_SPEED_Y 6

/**
 * A class that maintains the game state and is a superclass for game objects
 *
 * We chose the "Object-Centric Architecture" as described in 16.2.1 of Game Engine Architecture because it made sense for all
 * objects in the game (player, blocks, coins, enemies) to inherit from a class that includes position, dimensions, and velocity information
 * as well as the declaration of key functionality (rendering and updating). This inheritance simplifies our code; for each game object, we
 * simply render() and update() per-frame when necessary.
 *
 */
class GameObject {

public:
    //TODO: Relocate game state variables
    static bool gameOver;
    //static int levelCount;
    //static bool gameStarted;
    int gameObjectID = rand();

    enum SIDE {TOP, LEFT, RIGHT, BOTTOM, NONE};

    SDL_Color fillClr;
    SDL_Color lineClr;

    int tag;

    Vector3D pos;

    Vector3D dim;

    Vector3D vel = Vector3D(0, 0);

    SIDE prevCollision = SIDE::NONE;

    GameObject();

    GameObject(Vector3D pos, Vector3D dim, int tag, std::string spritesheetFileName, SDL_Renderer* gRenderer);

    //Used for testing
    GameObject(Vector3D pos, Vector3D dim, int tag);

    GameObject(Vector3D pos, Vector3D dim, int tag, SDL_Color fillClr, SDL_Color lineClr, std::string spritesheetFileName, SDL_Renderer* gRenderer);

    virtual ~GameObject();


    /**
     * Used to check whether this game object is colliding with another.
     *
     * @param obj the object to check if colliding with
     * @return returns the side that is colliding with respect to THIS game object, or null if not colliding
     */
    GameObject::SIDE isColliding(GameObject obj);

    virtual void render(SDL_Renderer *gRenderer);

    virtual void update();

    void setSpritesheet(std::string spritesheetFileName, SDL_Renderer* gRenderer){
        this->spritesheet = ResourceManager::getInstance()->getTextureResource(gRenderer, spritesheetFileName);
        this->spritesheetFileName = spritesheetFileName;
    }

protected:
    std::shared_ptr<SDL_Texture*> spritesheet;
    std::string spritesheetFileName;

};


#endif
