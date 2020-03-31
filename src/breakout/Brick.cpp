//
// Created by Alejandro Hervella on 3/30/20.
//
#include "Brick.hpp"

Brick::Brick() {
    SDL_Color c;
    c.r = 0;
    c.g = 0;
    c.b = 0;
    c.a = 0;
    init(0,0,0,0,c);
}

void Brick::init(float w, float h, float x, float y, SDL_Color clr) {
    this->w = w;
    this->h = h;
    this->x = x;
    this->y = y;
    this->clr = clr;
}

void Brick::render(SDL_Renderer *gRenderer) {
    SDL_Rect fillRect = {(int) x, (int) y, (int) w, (int) h};
    //sets render color to red
    SDL_SetRenderDrawColor(gRenderer, clr.r, clr.g, clr.b, clr.a);
    SDL_RenderFillRect(gRenderer, &fillRect);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0);
    SDL_RenderDrawRect(gRenderer, &fillRect);
}