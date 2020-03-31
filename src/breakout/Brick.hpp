//
// Created by Alejandro Hervella on 3/30/20.
//

#ifndef PROJ3_FINAL_MIKEY_ALEJANDRO_KARI_FINAL_BRICK_HPP
#define PROJ3_FINAL_MIKEY_ALEJANDRO_KARI_FINAL_BRICK_HPP

#include "GameObject.hpp"


#define BRICK_WIDTHS 50
#define BRICK_HEIGHTS 10
#define BRICK_GROUP_DIST_FROM_CEILING 40



/**
 * A brick class that represents a block on the screen
 */
class Brick : public GameObject {

public:
    float h;
    float w;
    float x;
    float y;

    //empty constructor so easier to make array list

    /**
     * Default constructor. Initializes a brick with the color black.
     */
    Brick();

    /**
     * Init function that initializes a brick with the given parameters
     *
     * @param w: The width of the brick
     * @param h: The height of the brick
     * @param x: The x position of the brick
     * @param y: The y position of the brick
     * @param clr: The color of the brick
     */
    void init(float w, float h, float x, float y, SDL_Color clr);

    /**
     * Renders the brick on the screen
     *
     * @param gRenderer: The renderer used to render the brick
     */
    void render(SDL_Renderer *gRenderer);

private:
    SDL_Color clr = {0, 0, 255, 250};

};

#endif //PROJ3_FINAL_MIKEY_ALEJANDRO_KARI_FINAL_BRICK_HPP
