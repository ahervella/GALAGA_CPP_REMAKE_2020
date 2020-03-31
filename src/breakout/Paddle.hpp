//
// Created by Alejandro Hervella on 3/30/20.
//

#ifndef PROJ3_FINAL_MIKEY_ALEJANDRO_KARI_FINAL_PADDLE_HPP
#define PROJ3_FINAL_MIKEY_ALEJANDRO_KARI_FINAL_PADDLE_HPP


#include "Brick.hpp"


#define PADDLE_WIDTH 75
#define PADDLE_HEIGHT 10
#define PADDLE_DIST_FROM_CEILING 400
#define PADDLE_SPEED 6

/**
 * A subclass of Brick. Inherits the position, size, and color fields as well as rendering function. Differs in its use of an update function
 * to update its position based on user input.
 */
class Paddle : public Brick {
public:
    float speed;

    /**
     * Updates the position of the player paddle based on user input
     */
    void update();
};

#endif //PROJ3_FINAL_MIKEY_ALEJANDRO_KARI_FINAL_PADDLE_HPP
