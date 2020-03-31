/*
 * SDLGraphicsProgram.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: Karina
 */

#include "SDLGraphicsProgram.hpp"

#include <algorithm>
#include <memory>

//for getting files from directory
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "Textbox.hpp"

#include "GameObject.hpp"

#include "TinyMath.hpp"

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Brick.hpp"
#include "Constants.hpp"

#define SCREEN_TICKS_PER_FRAME 1000/60

#define GAME_WON_TEXT "You won! Space for next Level!"
#define NA_TEXT "N/A"
#define DEFAULT_LIVES 3

int BRICK_GROUP_X_POS = -1;

int BRICK_ROWS = -1;
int BRICK_COLUMNS  = -1;

const SDL_Color PADDLE_COLOR = {0, 250, 0, 250};



Textbox centerText = Textbox(NA_TEXT, 20, SCREEN_WIDTH / 2 - 175, SCREEN_HEIGHT / 2 - 100);
CounterTextbox livesText = CounterTextbox(NA_TEXT, DEFAULT_LIVES, 15, 15, 3);
CounterTextbox scoreText = CounterTextbox(NA_TEXT, 0, 15, SCREEN_WIDTH - 75, 3);

std::vector <Brick> bricks;
Ball ball;
Paddle paddle;
int prevCollisionID = -1;
int brickCollisionIndex = -1;

int lifeCount = DEFAULT_LIVES;
int score = 0;
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

// Define functions to help with Breakout

void gameOver() {
    GameObject::gameOver = true;

    std::cout << "Game over set" << std::endl;
    if (Player::lifeCount < 0) {
        centerText.text = centerText.text = gameTexts["LOSE"];
        std::cout << "Game over text set (lose)" << std::endl;
        return;
    }

    centerText.text = gameTexts["WIN"];
    std::cout << "Game over text set (win)" << std::endl;

}


void checkBrickCollision(float &newAngle, int &collisionID, int &brickCollisionIndex) {

    Brick collidingBrick = Brick();
    float closestDistance = -1;

    float ballLeft = ball.x;
    float ballRight = ball.x + ball.r * 2;
    float ballTop = ball.y;
    float ballBottom = ball.y + ball.r * 2;
    float ballCenterX = ball.x + ball.r;
    float ballCenterY = ball.y + ball.r;

    for (size_t i = 0; i < bricks.size(); i++) {
        Brick b = bricks[i];

        float brickLeft = b.x;
        float brickRight = b.x + b.w;
        float brickTop = b.y;
        float brickBottom = b.y + b.h;


        bool xRightBall = ballRight >= brickLeft;
        bool xLeftBall = ballLeft <= brickRight;

        bool yBottomBall = ballBottom > brickTop;
        bool yTopBall = ballTop <= brickBottom;

        if (xLeftBall && xRightBall && yBottomBall && yTopBall) {

            float distanceToCenterOfBrick = sqrt(pow(ballCenterX - (b.x + b.w / 2.0f), 2)
                                                 + pow(ballCenterY - (b.y + b.h / 2.0f), 2));

            if (closestDistance == -1 || distanceToCenterOfBrick < closestDistance) {
                closestDistance = distanceToCenterOfBrick;
                collidingBrick = b;
                brickCollisionIndex = i;
            }
        }
    }


    if (closestDistance != -1) {

        float brickLeft = collidingBrick.x;
        float brickRight = collidingBrick.x + collidingBrick.w;
        float brickTop = collidingBrick.y;
        float brickBottom = collidingBrick.y + collidingBrick.h;

        collisionID = collidingBrick.gameObjectID;

        float brickRightToCenter = brickRight - ballCenterX;
        float brickLeftToCenter = ballCenterX - brickLeft;
        float brickTopToCenter = ballCenterY - brickTop;
        float brickBottomToCenter = brickBottom - ballCenterY;


        //going towards bottom right
        if (cosf(ball.angle) > 0 && sinf(ball.angle) > 0) {
            if (brickLeftToCenter < brickTopToCenter) {
                newAngle = reflectRadAngle(newAngle, false);
                return;
            }
            newAngle = reflectRadAngle(newAngle, true);
            return;
        }

        //going towards bottom left
        if (cosf(ball.angle) <= 0 && sinf(ball.angle) > 0) {
            if (brickRightToCenter < brickTopToCenter) {
                newAngle = reflectRadAngle(newAngle, false);
                return;
            }

            newAngle = reflectRadAngle(newAngle, true);
            return;
        }

        //going towards top right
        if (cosf(ball.angle) > 0 && sinf(ball.angle) <= 0) {
            if (brickLeftToCenter < brickBottomToCenter) {
                newAngle = reflectRadAngle(newAngle, false);
                return;
            }

            newAngle = reflectRadAngle(newAngle, true);
            return;
        }

        //going towards top left
        if (cosf(ball.angle) <= 0 && sinf(ball.angle) <= 0) {
            if (brickRightToCenter < brickBottomToCenter) {
                newAngle = reflectRadAngle(newAngle, false);
                return;
            }

            newAngle = reflectRadAngle(newAngle, true);
            return;
        }

    }
}

void loseLife() {
    lifeCount -= 1;

    if (lifeCount < 0) {
        gameOver();
        return;
    }

    ball.resetBall();
    livesText.setText(lifeCount);
}

void removeBrick(int rmIndex) {


    scoreText.setText(++score);

    bricks.erase(bricks.begin() + rmIndex);

    std::cout<<bricks.size()<<std::endl;
    if (bricks.size() <= 0) {
        gameOver();
        return;
    }
}

void updateBallCollisions() {

    int collisionID = -1;
    float newAngle = ball.angle;

    //For now, this is assuming the ball is drawn in a rectangle that calculates the collisions
    //have to give walls custom collisionIDs (which are negative so that they are not chosen by the
    //randomizer that chooses IDs in the gameObject class)

    //if hit floor, lose a life
    if (ball.y + 2 * ball.r > SCREEN_HEIGHT) {
        loseLife();
        collisionID = -2;
    }

    //if it ceiling
    if (ball.y < 0) {
        newAngle = reflectRadAngle(newAngle, true);
        collisionID = -3;
    }

    //if hit left wall
    if (ball.x < 0) {
        newAngle = reflectRadAngle(newAngle, false);
        collisionID = -4;
    }

    if (ball.x + 2 * ball.r > SCREEN_WIDTH) {
        newAngle = reflectRadAngle(newAngle, false);
        collisionID = -5;
    }

    //if bottom is passing paddle but center is above it, then bounce. If under the paddle completely, no go
    if (ball.y + 2 * ball.r > PADDLE_DIST_FROM_CEILING &&
        ((ball.x + ball.r) > paddle.x && ball.x < (paddle.x + paddle.w)) &&
        ball.y + ball.r < PADDLE_DIST_FROM_CEILING) {
        newAngle = reflectRadAngle(newAngle, true);
        collisionID = -6;
    } else {
        checkBrickCollision(newAngle, collisionID, brickCollisionIndex);
        if (brickCollisionIndex != -1) {
            //remove based off index
            removeBrick(brickCollisionIndex);
            brickCollisionIndex = -1;
        }
    }


    //this logic is so that and ball that is still inside shapes doesn't collide multiple times off of
    //one bounce from the same object
    if (collisionID != prevCollisionID) {
        prevCollisionID = collisionID;
        ball.angle = newAngle;
        ball.playBlockSFXFile();
    }


}


void resetToLevel(BreakoutLevel lvl){
    BRICK_ROWS = lvl.BRICK_ROWS;
    std::cout<<"BRICK_ROWS"<<std::endl;
    std::cout<<BRICK_ROWS<<std::endl;
    BRICK_COLUMNS = lvl.BRICK_COLUMNS;
    ball.resetBall();
    scoreText.setText(0);
    BRICK_GROUP_X_POS = (SCREEN_WIDTH - BRICK_WIDTHS * BRICK_COLUMNS) / 2;


    for(int i = 0; i < BRICK_ROWS * BRICK_COLUMNS; i++){

        bricks.push_back(Brick());

    }

    std::cout<<"sizeXXX"<<std::endl;
    std::cout<<bricks.size()<<std::endl;

    for (int y = 0; y < BRICK_ROWS; ++y) {
        for (int x = 0; x < BRICK_COLUMNS; ++x) {
            int brickX = BRICK_GROUP_X_POS + BRICK_WIDTHS * x;
            int brickY = BRICK_GROUP_DIST_FROM_CEILING + BRICK_HEIGHTS * y;
            bricks[y * BRICK_COLUMNS + x].init(BRICK_WIDTHS, BRICK_HEIGHTS, brickX, brickY, {0, 0, 255, 255});
        }
    }



    float paddleCenter = (SCREEN_WIDTH - PADDLE_WIDTH) / 2.0;
    paddle.init(PADDLE_WIDTH, PADDLE_HEIGHT, paddleCenter, PADDLE_DIST_FROM_CEILING, PADDLE_COLOR);
}

//Define SDLGraphicsProgram functions

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int gameCode) :
        gWindow(nullptr),
        gRenderer(nullptr) {

	gc = gameCode;

    initLevelLoading();
    //resetToLevel(levels[levelCount]);

    // Initialization flag
    bool success = true;
    // String to hold any errors that occur.
    std::stringstream errorStream;

    // The window we'll be rendering to here
    gWindow = nullptr;
    // Render flag

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
        success = false;
    } else {
    	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
    		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
    	    success = false;
    	} else {
        //Initialize TTF
    		if (TTF_Init() != 0) {
            errorStream << "SDL_TTF could not initialize! SDL Error: " << SDL_GetError() << "\n";
            success = false;
    		} else {

    			//Create window
    			gWindow = SDL_CreateWindow("Lab",
                                       	   SDL_WINDOWPOS_UNDEFINED,
										   SDL_WINDOWPOS_UNDEFINED,
										   screenWidth,
										   screenHeight,
										   SDL_WINDOW_SHOWN);

    			// Check if Window did not create.
    			if (gWindow == nullptr) {
    				errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
    				success = false;
    			}

    			//Create a Renderer to draw on
    			//-1 flag places thing to be rendered in next available spot (index)
    			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    			// Check if Renderer did not create.
    			if (gRenderer == nullptr) {
    				errorStream << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
    				success = false;
    			}
        	}
    	}
    }

    // If initialization did not work, then print out a list of errors in the constructor.
    if (!success) {
        errorStream << "Failed to initialize!\n";
        std::string errors = errorStream.str();
        std::cout << errors << "\n";
    } else {
        std::cout << "No SDL, or OpenGL, errors Detected\n\n";
    }

    //Resource Manager initialization
    resourceManager = ResourceManager::getInstance();
    if (gc == 1)
    {
    	//Ball init with sound files
    	ball.init(Constants::Breakout::SFXPath::BALL_BLOCK, Constants::Breakout::SFXPath::BALL_WALL);
    	backgroundMusicFile = Constants::Breakout::SFXPath::MUSIC;
    }

    else if (gc == 2)
    {
    	backgroundMusicFile = Constants::Platformer::SFXPath::MUSIC;
    }

    //Load background music
    this->backgroundMusicFile = getResourcePath() + backgroundMusicFile;
    backgroundMusic = resourceManager->getMusicResource(this->backgroundMusicFile);

    //Load background image
    this->backgroundImageFile = Constants::Platformer::TexturePath::BACKGROUND;
    backgroundImage = resourceManager->getTextureResource(getSDLRenderer(), this->backgroundImageFile);
}


// Proper shutdown and destroy initialized objects
//~ = destructor
SDLGraphicsProgram::~SDLGraphicsProgram() {
	//Stop and destroy music
	Mix_HaltMusic();
	backgroundMusic.reset();
	resourceManager->deleteMusicResource(backgroundMusicFile);
    // Destroy Renderer
    SDL_DestroyRenderer(gRenderer);
    //Destroy window
    SDL_DestroyWindow(gWindow);
    // Point gWindow to NULL to ensure it points to nothing.
    gRenderer = nullptr;
    gWindow = nullptr;
    //Quit SDL subsystems
    //TODO: Check if any other ones need to be quit

    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
    //Destroy resource manager
    resourceManager->shutDown();
}

void SDLGraphicsProgram::update() {

    switch (gc){
    case 1:
        updateBreakout();
    return;
    case 2:
        updatePlatformer();
    return;
    case 3:
        //galaga
        return;
}

}

// Update OpenGL
void SDLGraphicsProgram::updateBreakout() {
    //render background
    SDL_SetRenderDrawColor(gRenderer, 0x22, 0x22, 0x22, 0xFF);
    SDL_RenderClear(gRenderer);

    //update paddle
    paddle.update();

    //update ball position
    ball.update();

    //update collisions (paddle)
    updateBallCollisions();
}

// Update OpenGL
void SDLGraphicsProgram::updatePlatformer() {
	if(GameObject::gameOver) {
		return;
	}
    //render background
    SDL_SetRenderDrawColor(gRenderer, 0x22, 0x22, 0x22, 0xFF);
    SDL_RenderClear(gRenderer);

    //Check for collision between player and blocks.
    //Here we can determine collisions between player and blocks below/to the side of a player
    //And here we can promptly update what needs to get updated based on that

    //TODO: need to also find a way to break boxes and remove them in the individual level objs
    //if we want to break blocks

    std::vector<GameObject> objs = PlatformerLevels[currLevelIndex].levelObjs;//levels[currLevelIndex].levelObjs;
    Player* p = &PlatformerLevels[currLevelIndex].player;
    //If we have a collision, let the player figure out what to do based on where the collision is from
    //TODO: For future, figure out

    //scoreText.setText(p->getScore());

    int indexToRemove = -1;

    for(size_t i = 0; i < objs.size(); ++i) {
        std::cout<<"Index: "<<i<<std::endl;
        int tag = objs[i].tag;
        if (p->collisionUpdate(p->isColliding(objs[i]), tag)){
        	indexToRemove = i;
        	std::cout << "GOT PAST COLLIDING" << std::endl;
        	PlatformerLevels[currLevelIndex].levelObjs.erase(PlatformerLevels[currLevelIndex].levelObjs.begin() + indexToRemove);
        	std::cout << "DELETED" << std::endl;
            if(tag == Constants::Platformer::Game::Tag::GOAL_TAG) {
            	std::cout << "WIN: TOUCHED GOAL" << std::endl;
            	gameOver();
            	return;
            }
        }

    }

    for(size_t j = 0; j < PlatformerLevels[currLevelIndex].enemyObjs.size(); ++j) {
    	if(p->collisionUpdate(p->isColliding(PlatformerLevels[currLevelIndex].enemyObjs[j]), Constants::Platformer::Game::Tag::ENEMY_TAG)) {
    		std::cout << "LOSE: TOUCHED ENEMY" << std::endl;
    		loseLife();
    		if(GameObject::gameOver) {
    			//Stop the game if we hit game over after losing a life
    			return;
    		}
    	}
    	for(size_t s = 0; s < objs.size(); ++s) {
    		PlatformerLevels[currLevelIndex].enemyObjs[j].collisionUpdate
				(PlatformerLevels[currLevelIndex].enemyObjs[j].isColliding(objs[s]), objs[s].tag);
    	}
    }

    //all velocities should be final at this point in frame for player, apply to position
    //std::cout<<"size: " + std::to_string(objs.size())<<std::endl;
    p->move();

    p->update();

    for(size_t k = 0; k < PlatformerLevels[currLevelIndex].enemyObjs.size(); ++k) {
    	PlatformerLevels[currLevelIndex].enemyObjs[k].move();
    	PlatformerLevels[currLevelIndex].enemyObjs[k].update();
    }
}


void SDLGraphicsProgram::render() {

    switch (gc) {
        case 1:
            renderBreakout();
            return;
        case 2:
            renderPlatformer();
            return;
        case 3:
            //galaga
            return;
    }

}

void SDLGraphicsProgram::renderBreakout() {

    for (size_t i = 0; i < bricks.size(); i++) {
        bricks[i].render(getSDLRenderer());
    }

    ball.render(getSDLRenderer());
    paddle.render(getSDLRenderer());


    //render lives left texture
    SDL_Texture *livesTextImage = renderText(livesText.text, livesText.textResPath, livesText.clr, livesText.textSize,
                                             getSDLRenderer());
    renderTexture(livesTextImage, getSDLRenderer(), livesText.x, livesText.y);

    //render score texture
    SDL_Texture *scoreTextImage = renderText(scoreText.text, scoreText.textResPath, scoreText.clr, scoreText.textSize,
                                             getSDLRenderer());
    renderTexture(scoreTextImage, getSDLRenderer(), scoreText.x, scoreText.y);



    //render center texture
    SDL_Texture *centerTextImage = renderText(centerText.text, livesText.textResPath, centerText.clr,
                                              centerText.textSize, getSDLRenderer());
    renderTexture(centerTextImage, getSDLRenderer(), centerText.x, centerText.y);

    SDL_RenderPresent(getSDLRenderer());
    SDL_DestroyTexture(livesTextImage);
    SDL_DestroyTexture(scoreTextImage);
    SDL_DestroyTexture(centerTextImage);

}

void SDLGraphicsProgram::renderPlatformer() {

	SDL_RenderCopy(getSDLRenderer(), *backgroundImage, NULL, NULL);

	PlatformerLevels[currLevelIndex].render(getSDLRenderer());

    SDL_RenderPresent(getSDLRenderer());

}


void SDLGraphicsProgram::loop() {

    switch (gc) {
        case 1:
            loopBreakout();
            return;
        case 2:
            loopPlatformer();
            return;
        case 3:
            //galaga
            return;
    }
}


//Loops forever!
void SDLGraphicsProgram::loopBreakout() {

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

    std::cout<<"working3"<<std::endl;

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

                    case SDLK_SPACE:

                            if ((size_t) levelCount < BreakoutLevels.size() && GameObject::gameOver){
                                if(levelCount != 0){
                                    resetToLevel(BreakoutLevels[levelCount]);
                                }
                                levelCount++;

                                if((size_t)levelCount == BreakoutLevels.size()) {
                                    centerText.text = gameTexts["DONE"];
                                }

                                GameObject::gameOver = false;
                                centerText.text = " ";
                            }

                        break;

                    case SDLK_RIGHT:
                        if (!rightJustPressed){
                            paddle.speed = PADDLE_SPEED;
                            rightJustPressed = true;
                        }
                        break;
                    case SDLK_LEFT:
                        if (!leftJustPressed){
                            paddle.speed = -PADDLE_SPEED;
                            leftJustPressed = true;
                        }
                        break;
                    case SDLK_1:
                        changeLanguage(0);
                        break;
                    case SDLK_2:
                        changeLanguage(1);
                        break;

                    case SDLK_3:
                        changeLanguage(2);
                        break;

                    case SDLK_q:
                        quit = true;
                        break;
                }
            }

            if (e.type == SDL_KEYUP){
                switch( e.key.keysym.sym ){
                    case SDLK_RIGHT:
                        if(rightJustPressed){
                            rightJustPressed = false;
                        }
                        break;
                    case SDLK_LEFT:
                        if(leftJustPressed){
                            leftJustPressed = false;
                        }
                        break;
                }
            }

            //needed for when let go after holding both down.
            if (leftJustPressed && !rightJustPressed){
                paddle.speed = -PADDLE_SPEED;
            }

            if (!leftJustPressed && rightJustPressed){
                paddle.speed = PADDLE_SPEED;
            }

            if (!leftJustPressed && !rightJustPressed){
                paddle.speed = 0;
            }
        }

        // Update our scene
        updateBreakout();
        // Render
        renderBreakout();

        //Cap frame rate
        unsigned int endFrame = SDL_GetTicks();
        unsigned int frameTick = endFrame - startFrame;
        if (frameTick < SCREEN_TICKS_PER_FRAME) {
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTick);
        }
    }

    //Disable text input
    SDL_StopTextInput();
}

//Loops forever!
void SDLGraphicsProgram::loopPlatformer() {
    //start of program

    //set alpha channel on
    SDL_SetRenderDrawBlendMode(getSDLRenderer(), SDL_BLENDMODE_BLEND);

    //doing this here because can't call virtual function in constructor!
    initLevelLoading();

    //construct current level (probably level 1, index 0)
    PlatformerLevels[currLevelIndex].constructLevel(getSDLRenderer());

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

        Player* p = &PlatformerLevels[currLevelIndex].player;

       // p->vel.x = 0;

        if (p->justJumped){
            p->justJumped = false;
        }


        if (p->vel.y < MAX_SPEED_Y){
            p->vel.y += GRAV;
        }

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

                	case SDLK_SPACE:
                		if ((size_t)currLevelIndex < PlatformerLevels.size() && GameObject::gameOver){


                		    currLevelIndex++;

                		    if((size_t)currLevelIndex == PlatformerLevels.size()) {
                		        centerText.text = gameTexts["DONE"];
                		    }

                			if(currLevelIndex != 0){
                				PlatformerLevels[currLevelIndex].constructLevel(getSDLRenderer());
                		    }


                		    GameObject::gameOver = false;
                		    centerText.text = " ";


                		}

                		break;

                    case SDLK_UP:

                        if(p->isJumping()){break;}
                        p->vel.y = -1.0f * MAX_SPEED_Y;
                        if (!p->isJumping()){
                            p->justJumped = true;
                        }

                        p->jump(); //makes jumped true

                        break;

                    case SDLK_RIGHT:
                        if (!rightJustPressed){
                            p->vel.x = MAX_SPEED_X;//paddle.speed = PADDLE_SPEED;
                            rightJustPressed = true;
                        }
                        break;
                    case SDLK_LEFT:
                        if (!leftJustPressed){
                            p->vel.x = (-1.0f * MAX_SPEED_X);
                            leftJustPressed = true;
                        }
                        break;
                    case SDLK_1:
                        changeLanguage(0);
                        break;
                    case SDLK_2:
                        changeLanguage(1);
                        break;

                    case SDLK_3:
                        changeLanguage(2);
                        break;

                    case SDLK_q:
                        quit = true;
                        break;
                    case SDLK_r:
                    	PlatformerLevels[currLevelIndex].constructLevel(getSDLRenderer());
                        GameObject::gameOver = false;
                        break;
                }
            }

            if (e.type == SDL_KEYUP){
                switch( e.key.keysym.sym ){
                    case SDLK_RIGHT:
                        if(rightJustPressed){
                            rightJustPressed = false;
                        }
                        break;
                    case SDLK_LEFT:
                        if(leftJustPressed){
                            leftJustPressed = false;
                        }
                        break;
                }
            }

            //needed for when let go after holding both down.
            if (leftJustPressed && !rightJustPressed){
                p->vel.x = (-1.0f * MAX_SPEED_X);
            }

            if (!leftJustPressed && rightJustPressed){
                p->vel.x = MAX_SPEED_X;
            }

            if (!leftJustPressed && !rightJustPressed){
                p->vel.x = 0;
            }
        }

        // Update our scene
        updatePlatformer();
        // Render
        renderPlatformer();

        //Cap frame rate
        unsigned int endFrame = SDL_GetTicks();
        unsigned int frameTick = endFrame - startFrame;
        if (frameTick < SCREEN_TICKS_PER_FRAME) {
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTick);
        }
    }
    //Disable text input
    SDL_StopTextInput();
}

void SDLGraphicsProgram::initLevelLoading(){

    getLanguages();
    //Default to English
    changeLanguage(0);

    std::ifstream fin;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;


    std::string resourceConfigsPath = getResourcePath("level_config");

    //open directory path
    dp = opendir(resourceConfigsPath.c_str());

    if (dp == NULL)
    {
        std::cout << "Error opening " << resourceConfigsPath << std::endl;
        return;
    }

    //load level configs
    while ((dirp = readdir( dp )))
    {

        std::string contents;
        std::string skipStringMessage = "Skipped: " + (std::string(dirp->d_name));


        std::string filePath = resourceConfigsPath + dirp->d_name;

        if ((std::string(dirp->d_name)).find(".") == 0){
            std::cout<< skipStringMessage <<std::endl<<std::endl;
            continue;}

        //prevent from getting other hidden non text file paths
        if (stat( filePath.c_str(), &filestat )) {
            std::cout<<skipStringMessage<<std::endl<<std::endl;
            continue;}

        if (S_ISDIR( filestat.st_mode ))  {
            std::cout<<skipStringMessage<<std::endl<<std::endl;
            continue;}

        std::cout << "File path: " << filePath << std::endl;

        switch(gc){
            case 1:
                BreakoutLevels.push_back(BreakoutLevel(filePath));
                break;
            case 2:
                PlatformerLevels.push_back(Level(filePath));
                break;
            case 3:
                //galaga levels
                break;

        }


    }
}

void SDLGraphicsProgram::getLanguages(){

    std::ifstream fin;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
    std::string resourceConfigsPath = getResourcePath("lang_config");

    dp = opendir(resourceConfigsPath.c_str());

    if (dp == NULL)
    {
        std::cout << "Error opening " << resourceConfigsPath << std::endl;
        return;
    }

    while ((dirp = readdir( dp )))
    {
        std::string contents;

        std::string filePath = resourceConfigsPath + dirp->d_name;

        std::string skipStringMessage = "Skipped: " + (std::string(dirp->d_name));

        if ((std::string(dirp->d_name)).find(".") == 0){
            std::cout<< skipStringMessage <<std::endl<<std::endl;
            continue;}

        //prevent from getting other hidden non text file paths
        if (stat( filePath.c_str(), &filestat )) {
            std::cout<< skipStringMessage <<std::endl<<std::endl;
            continue;}

        if (S_ISDIR( filestat.st_mode ))  {
            std::cout<< skipStringMessage <<std::endl<<std::endl;
            continue;}

        std::cout<< "Not skipping: " + (std::string(dirp->d_name) )<<std::endl<<std::endl;

        languageFiles.push_back(filePath);
    }
}

void SDLGraphicsProgram::changeLanguage(int langIndex){

	currLanguageStream.reset();
    currLanguageStream = ResourceManager::getInstance()->getConfigFileResource(languageFiles[langIndex]);
        std::string contents;
        std::string line;
        std::string key;
        std::string value;
        std::ifstream* blah = *(currLanguageStream);

        //so that no previous language phrases are still left in cae file is not complete.
    for(auto const& keyy: gameTexts){
            gameTexts[keyy.first] = NA_TEXT;
    }

    (*currLanguageStream)->open(languageFiles[langIndex].c_str(), std::fstream::out );

    while ( (*currLanguageStream)->good() ) {
        std::getline(*blah, line);
        std::istringstream ss(line);
        if (std::getline(ss, key, ' ')) {
            if (gameTexts.count(key) > 0) {
                if (std::getline(ss, value)) {
                    gameTexts[key] = value;
                    std::cout << key + gameTexts[key] << std::endl << std::endl;
                }
            }
        }
    }

    (*currLanguageStream)->close();

    for(auto const& key: gameTexts){
        if (key.second == NA_TEXT){
            std::cout<<"Warning--No match for key: \"" + key.first + "\" was found!"<<std::endl<<std::endl;
        }
    }
}


// Get Pointer to Window
//(private) utility in case we want to get the window
SDL_Window *SDLGraphicsProgram::getSDLWindow() {
    return gWindow;
}

// Get Pointer to Renderer
//(private) utility in case we want to get the window
SDL_Renderer *SDLGraphicsProgram::getSDLRenderer() {
    return gRenderer;
}
