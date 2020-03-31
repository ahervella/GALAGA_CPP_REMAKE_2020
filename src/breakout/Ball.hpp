  
//
// Created by Alejandro Hervella on 3/30/20.
//

#ifndef PROJ3_FINAL_MIKEY_ALEJANDRO_KARI_FINAL_BALL_HPP
#define PROJ3_FINAL_MIKEY_ALEJANDRO_KARI_FINAL_BALL_HPP

#include "GameObject.hpp"



#define BALL_SPEED 5
#define BALL_RADIUS 5

/**
 * A class for the ball that the player bounces into the blocks with the controllable paddle
 */
class Ball : public GameObject {
public:
    float x = SCREEN_WIDTH / 2.0 + 75;
    float prevX = 0;
    float y = SCREEN_HEIGHT / 2.0;
    float prevY = 0;
    float r = BALL_RADIUS;
    float speed = BALL_SPEED;
    float angle = 2.0f;

    /**
     * Default constructor. No body.
     */
    Ball();

    /**
     * Destructor that will handle getting rid of the ball's resources from the ResourceManager (soundFx)
     */
    ~Ball();

    /**
     * Initializes a ball with two sound files (one for hitting bricks, the other for hitting the paddle or a wall)
     *
     * @param blockSoundFile: Path to sound file that plays when the ball hits a brick
     * @param paddleWallSoundFile: Path to sound file that plays when the ball hits a wall or the paddle
     */
    void init(std::string blockSoundFile, std::string paddleWallSoundFile);

    /**
     * Updates the position of the ball and saves the old position
     */
    void update();


    /**
     * Renders the ball on the screen
     *
     * @param gRenderer: The renderer the ball is rendered by
     */
    void render(SDL_Renderer *gRenderer);

    /**
     * Resets the ball to its starting point
     */
    void resetBall();

    /**
     * Plays the sound effect that plays when a ball hits a block
     */
    void playBlockSFXFile();

    /**
     * Plays the sound effect that plays when a ball hits a wall/paddle
     */
    void playPaddleWallSFXFile();

private:

    SDL_Color clr = {250, 250, 250, 250};
    std::shared_ptr<Mix_Chunk*> blockSFX;
    std::string blockSFXFile;
    std::shared_ptr<Mix_Chunk*> paddleWallSFX;
    std::string paddleWallSFXFile;
};

#endif //PROJ3_FINAL_MIKEY_ALEJANDRO_KARI_FINAL_BALL_HPP
