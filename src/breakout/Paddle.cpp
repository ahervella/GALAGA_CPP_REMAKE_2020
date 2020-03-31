  
//
// Created by Alejandro Hervella on 3/30/20.
//
#include "Paddle.hpp"



void Paddle::update() {
    if (gameOver) { return; }
    if ((x + w < SCREEN_WIDTH && speed > 0) ||
        (x > 0 && speed < 0)) {
        this->x += this->speed;
    }
}
