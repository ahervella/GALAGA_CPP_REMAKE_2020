//
// Created by Alejandro Hervella on 3/30/20.
//
#include "Ball.hpp"

Ball::Ball() {
}

Ball::~Ball(){
    if(blockSFX) {
        blockSFX.reset();
        ResourceManager::getInstance()->deleteSFXResource(blockSFXFile);
    }
    if(paddleWallSFX) {
        paddleWallSFX.reset();
        ResourceManager::getInstance()->deleteSFXResource(paddleWallSFXFile);
    }
}

void Ball::init(std::string blockSoundFile, std::string paddleWallSoundFile) {
    blockSFXFile = blockSoundFile;
    paddleWallSFXFile = paddleWallSoundFile;
    blockSFX = ResourceManager::getInstance()->getSFXResource(blockSFXFile);
    paddleWallSFX = ResourceManager::getInstance()->getSFXResource(paddleWallSFXFile);
}

void Ball::update() {
    if (gameOver) { return; }
    prevX = x;
    prevY = y;
    x += (speed * cosf(angle));
    y += (speed * sinf(angle));
}

void Ball::render(SDL_Renderer *gRenderer) {
    SDL_Rect fillRect = {(int) x, (int) y, (int) r * 2, (int) r * 2};
    //sets render color to red
    SDL_SetRenderDrawColor(gRenderer, clr.r, clr.g, clr.b, clr.a);
    SDL_RenderDrawRect(gRenderer, &fillRect);
}

void Ball::resetBall() {
    x = SCREEN_WIDTH / 2.0;
    y = SCREEN_HEIGHT / 2.0;
    r = BALL_RADIUS;
    speed = BALL_SPEED;
    angle = 2.0f;
}

void Ball::playBlockSFXFile() {
    Mix_PlayChannel(-1, *(blockSFX), 0);
}

void Ball::playPaddleWallSFXFile() {
    Mix_PlayChannel(-1, *(paddleWallSFX), 0);
}