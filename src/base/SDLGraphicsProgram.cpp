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

//editor variables:
#define TEXT_SIZE 20

#define TEXT_X_ANCHOR_BREAKOUT Constants::Breakout::Game::SCREEN_UNIT_WIDTH * Constants::Breakout::Game::UNIT / 2 - 175
#define TEXT_Y_ANCHOR_BREAKOUT Constants::Breakout::Game::SCREEN_UNIT_HEIGHT * Constants::Breakout::Game::UNIT / 2 - 100


#define TEXT_X_ANCHOR_PLATFORMER Constants::Platformer::Game::SCREEN_UNIT_WIDTH * Constants::Platformer::Game::UNIT / 2 - 175
#define TEXT_Y_ANCHOR_PLATFORMER Constants::Platformer::Game::SCREEN_UNIT_HEIGHT * Constants::Platformer::Game::UNIT / 2 - 100

int BRICK_GROUP_X_POS = -1;

int BRICK_ROWS = -1;
int BRICK_COLUMNS  = -1;

const SDL_Color PADDLE_COLOR = {0, 250, 0, 250};

//level editor main menu text

Textbox mainMenuText;

Textbox centerText = Textbox(NA_TEXT, 20, SCREEN_WIDTH / 2 - 175, SCREEN_HEIGHT / 2 - 100);
CounterTextbox livesText = CounterTextbox(NA_TEXT, DEFAULT_LIVES, 15, 15, 3);
CounterTextbox scoreText = CounterTextbox(NA_TEXT, 0, 15, SCREEN_WIDTH - 75, 3);

//std::vector <Brick> bricks;
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


void SDLGraphicsProgram::checkBrickCollision(float &newAngle, int &collisionID, int &brickCollisionIndex) {

    Brick collidingBrick = Brick();
    float closestDistance = -1;

    float ballLeft = ball.x;
    float ballRight = ball.x + ball.r * 2;
    float ballTop = ball.y;
    float ballBottom = ball.y + ball.r * 2;
    float ballCenterX = ball.x + ball.r;
    float ballCenterY = ball.y + ball.r;

    for (size_t i = 0; i < BreakoutLevels[levelCount].levelObjs.size(); i++) {
        Brick b = BreakoutLevels[levelCount].levelObjs[i];

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

void SDLGraphicsProgram::removeBrick(int rmIndex) {


    scoreText.setText(++score);

    BreakoutLevels[levelCount].levelObjs.erase(BreakoutLevels[levelCount].levelObjs.begin() + rmIndex);

    std::cout<<BreakoutLevels[levelCount].levelObjs.size()<<std::endl;
    if (BreakoutLevels[levelCount].levelObjs.size() <= 0) {
        gameOver();
        return;
    }
}

void SDLGraphicsProgram::updateBallCollisions() {

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


void SDLGraphicsProgram::resetToLevel(){

    BreakoutLevels[levelCount].resetLevel();

    BreakoutLevels[levelCount].constructLevel(getSDLRenderer());

    ball.resetBall();
    scoreText.setText(0);
    //BRICK_GROUP_X_POS = (SCREEN_WIDTH - BRICK_WIDTHS * BRICK_COLUMNS) / 2;


    /*
    for(size_t i = 0; i < lvl.levelObjs.size(); i++){

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


*/
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

	//skip if editor here, editor does it in loop
	if (gc > 0){
        initLevelLoading();
	}

    //
    //
    //
    // resetToLevel(levels[levelCount]);

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
        case -3:
        case -2:
        case -1:
            updateEditor();
            return;
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

    //std::cout<<"breakout levels  "<<BreakoutLevels.size()<<"  "<<BreakoutLevels[levelCount].levelObjs.size()<<std::endl;

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

void SDLGraphicsProgram::updateEditor() {
    //render background
    SDL_SetRenderDrawColor(gRenderer, 0x22, 0x22, 0x22, 0xFF);
    SDL_RenderClear(gRenderer);

    switch(gc){
        case -1:
            //break;
            if (!lvlSelectMode) {
                edt_cursor.pos = Vector3D(
                        edt_cursorBlockPos.x * Constants::Breakout::Game::UNIT,
                        edt_cursorBlockPos.y * Constants::Breakout::Game::UNIT);
            }
            break;
        case -2:
            if (!lvlSelectMode) {
                edt_cursor.pos = Vector3D(
                        edt_cursorBlockPos.x * Constants::Platformer::Game::UNIT,
                        edt_cursorBlockPos.y * Constants::Platformer::Game::UNIT);
            }
            break;
        case -3:
            //galaga stuff
            break;
    }


}

void SDLGraphicsProgram::render() {

    switch (gc) {
        case -3:
        case -2:
        case -1:
            renderEditor();
            return;
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

    BreakoutLevels[levelCount].render(getSDLRenderer());
    //std::cout<<"breakout levels  "<<BreakoutLevels.size()<<"  "<<BreakoutLevels[levelCount].levelObjs.size()<<std::endl;

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

void SDLGraphicsProgram::renderEditor() {

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
        switch(gc){
            case -1:
                edt_levels_breakout[edt_currLevelIndex]->render(getSDLRenderer());
                break;
            case -2:
                edt_levels_platformer[edt_currLevelIndex]->render(getSDLRenderer());
                break;
            case -3:
                //galaga stuff
                break;
        }

        std::cout<<"blahahahahhaaha"<<std::endl;
        edt_cursor.render(getSDLRenderer());
    }
//

    SDL_RenderPresent(getSDLRenderer());
    SDL_DestroyTexture(mainMenuTextImg);

    for (SDL_Texture *txtBoxTexture : txtboxs) {
        SDL_DestroyTexture(txtBoxTexture);
    }

    }

void SDLGraphicsProgram::loop() {

    switch (gc) {
        case -3:
        case -2:
        case -1:
            loopEditor();
            return;
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

    //set alpha channel on
    SDL_SetRenderDrawBlendMode(getSDLRenderer(), SDL_BLENDMODE_BLEND);


    BreakoutLevels[levelCount].constructLevel(getSDLRenderer());
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

    resetToLevel();

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

                            	levelCount++;

                            	if((size_t)levelCount == BreakoutLevels.size()) {
                            		centerText.text = gameTexts["DONE"];
                            	}

                            	if(levelCount != 0){
                                    resetToLevel();
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
    //initLevelLoading();

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


void SDLGraphicsProgram::loopEditor() {

    //start of program

    switch(gc){
        case -1:
            edt_cursor = GameObject(Vector3D(0,
                                             (Constants::Breakout::Game::SCREEN_UNIT_HEIGHT) * Constants::Breakout::Game::UNIT, 0),
                                    Vector3D(Constants::Breakout::Game::BLOCK_UNIT_DIM.x * Constants::Breakout::Game::UNIT,
                                             Constants::Breakout::Game::BLOCK_UNIT_DIM.y * Constants::Breakout::Game::UNIT, 0),
                                    -1,
                                    {0, 0, 250, 100},
                                    {255, 0, 0, 255},
                                    Constants::Breakout::TexturePath::CLEAR,
                                    gRenderer);
            break;
        case -2:
            edt_cursor = GameObject(Vector3D(0,
                                             (Constants::Platformer::Game::SCREEN_UNIT_HEIGHT) * Constants::Platformer::Game::UNIT, 0),
                                    Vector3D(Constants::Platformer::Game::BLOCK_UNIT_DIM.x * Constants::Platformer::Game::UNIT,
                                             Constants::Platformer::Game::BLOCK_UNIT_DIM.y * Constants::Platformer::Game::UNIT, 0),
                                    -1,
                                    {0, 0, 250, 100},
                                    {255, 0, 0, 255},
                                    Constants::Platformer::TexturePath::CLEAR,
                                    gRenderer);
            break;
        case -3:
            //galaga stuff
            break;
    }


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

                        switch(gc){
                            case -1:
                                if(edt_cursorBlockPos.y < Constants::Breakout::Game::SCREEN_UNIT_HEIGHT){
                                    edt_cursorBlockPos = Vector3D(edt_cursorBlockPos.x, edt_cursorBlockPos.y + 1);
                                }
                                break;
                            case -2:
                                if(edt_cursorBlockPos.y < Constants::Platformer::Game::SCREEN_UNIT_HEIGHT){
                                    edt_cursorBlockPos = Vector3D(edt_cursorBlockPos.x, edt_cursorBlockPos.y + 1);
                                }
                                break;
                            case -3:
                                //galaga stuff
                                break;
                        }

                        //left
                        break;

                    case SDLK_RIGHT:
                        if(lvlSelectMode){break;}

                        switch(gc){
                            case -1:
                                if(edt_cursorBlockPos.x < Constants::Breakout::Game::SCREEN_UNIT_WIDTH){
                                    edt_cursorBlockPos = Vector3D(edt_cursorBlockPos.x + 1, edt_cursorBlockPos.y);
                                }
                                break;
                            case -2:
                                if(edt_cursorBlockPos.x < Constants::Platformer::Game::SCREEN_UNIT_WIDTH){
                                    edt_cursorBlockPos = Vector3D(edt_cursorBlockPos.x + 1, edt_cursorBlockPos.y);
                                }
                                break;
                            case -3:
                                //galaga stuff
                                break;
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

                        switch(gc){
                            case -1:
                                edt_levels_breakout[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case -2:
                                edt_levels_platformer[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case -3:
                                //galaga stuff
                                break;
                        }


                        lvlSelectMode = false;
                        //1 key
                        break;
                    case SDLK_2:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 1;

                        levelHelper(2);

                        switch(gc){
                            case -1:
                                edt_levels_breakout[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case -2:
                                edt_levels_platformer[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case -3:
                                //galaga stuff
                                break;
                        }


                        lvlSelectMode = false;
                        //2 key
                        break;


                    case SDLK_3:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 2;

                        levelHelper(3);

                        switch(gc){
                            case -1:
                                edt_levels_breakout[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case -2:
                                edt_levels_platformer[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case -3:
                                //galaga stuff
                                break;
                        }

                        lvlSelectMode = false;
                        break;

                    case SDLK_4:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 3;

                        levelHelper(4);

                        switch(gc){
                            case -1:
                                edt_levels_breakout[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case -2:
                                edt_levels_platformer[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case -3:
                                //galaga stuff
                                break;
                        }

                        lvlSelectMode = false;
                        break;

                    case SDLK_5:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 4;

                        levelHelper(5);

                        switch(gc){
                            case -1:
                                edt_levels_breakout[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case -2:
                                edt_levels_platformer[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case -3:
                                //galaga stuff
                                break;
                        }

                        lvlSelectMode = false;
                        break;

                    case SDLK_6:
                        if(!lvlSelectMode){break;}
                        edt_currLevelIndex = 5;

                        levelHelper(6);

                        switch(gc){
                            case -1:
                                edt_levels_breakout[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case -2:
                                edt_levels_platformer[edt_currLevelIndex]->constructLevel(getSDLRenderer());
                                break;
                            case -3:
                                //galaga stuff
                                break;
                        }

                        lvlSelectMode = false;
                        break;

                    case SDLK_c:
                        editTile(edt_cursorBlockPos, "c");
                        //edt_levels_platformer[edt_currLevelIndex].writeToCfgFile("s");
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

    for(Level* lvl : edt_levels_platformer){
        delete lvl;
    }


}



void SDLGraphicsProgram::initLevelLoading(){

    switch(gc){
        case -3:
        case -2:
        case -1:
            initLevelLoadingEditor();
            return;
        case 1:
        case 2:
        case 3:
            initLevelLoadingGames();
            return;
    }

}


void SDLGraphicsProgram::initLevelLoadingEditor() {

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

    std::cout<<"the pipe is leaking"<<std::endl;
    switch(gc){
        case -1:
            mainMenuText = Textbox("MAIN MENU: ", TEXT_SIZE, TEXT_X_ANCHOR_BREAKOUT, TEXT_Y_ANCHOR_BREAKOUT);

            for(BreakoutLevel* lvl : edt_levels_breakout){
                delete lvl;
            }

            edt_levels_breakout.clear();

            break;
        case -2:
            mainMenuText = Textbox("MAIN MENU: ", TEXT_SIZE, TEXT_X_ANCHOR_PLATFORMER, TEXT_Y_ANCHOR_PLATFORMER);

            for(Level* lvl : edt_levels_platformer){
                delete lvl;
            }

            edt_levels_platformer.clear();
            break;
        case -3:
            //galaga stuff
            break;
    }







    //getLanguages();
    //Default to English
    //changeLanguage(0);

    std::ifstream fin;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;

    std::string resourceConfigsPath = "";

    //TODO: don't hard code to only work for editor here
    switch(gc){
        case -1:
            resourceConfigsPath = getResourcePath("breakout/level_config");
            break;
        case -2:
            resourceConfigsPath = getResourcePath("platformer/level_config");
            break;
        case -3:
            //galaga resourceConfigsPath...
            break;
    }

    //open directory path
    dp = opendir(resourceConfigsPath.c_str());

    if (dp == NULL)
    {
        std::cout << "Error opening " << resourceConfigsPath << std::endl;
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

        switch(gc){
            case -1:
                edt_menuTexts.push_back(Textbox(filename, TEXT_SIZE/1.5, TEXT_X_ANCHOR_BREAKOUT, TEXT_Y_ANCHOR_BREAKOUT + textYPosIncrementer));
                edt_levels_breakout.push_back(new BreakoutLevel(filePath));
                break;
            case -2:
                edt_menuTexts.push_back(Textbox(filename, TEXT_SIZE/1.5, TEXT_X_ANCHOR_PLATFORMER, TEXT_Y_ANCHOR_PLATFORMER + textYPosIncrementer));
                edt_levels_platformer.push_back(new Level(filePath));
                break;
            case -3:
                //galaga stuff
                break;
        }


        textYPosIncrementer += 30;

        //std::cout << "File pathhhh: " << filePath << std::endl;




    }

}

void SDLGraphicsProgram::initLevelLoadingGames(){

    getLanguages();
    //Default to English
    changeLanguage(0);

    std::ifstream fin;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;


    std::string resourceConfigsPath = "";
    if(gc == 1) {
    	resourceConfigsPath = getResourcePath("breakout/level_config");

    }
    if(gc == 2) {
        resourceConfigsPath = getResourcePath("platformer/level_config");
    }

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
    std::string resourceConfigsPath = "";
    if(gc == 1) {
    	resourceConfigsPath = getResourcePath("breakout/lang_config");
    }
    if(gc == 2) {
    	resourceConfigsPath = getResourcePath("platformer/lang_config");
    }

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

	if(currLanguageStream) {
		currLanguageStream.reset();
	}
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

    blah->close();
    blah->open(languageFiles[langIndex].c_str());


    while ( /*(*currLanguageStream)*/blah->good() ){
     	std::cout << "blah is good!" << std::endl;
        std::getline(*blah, line);
        std::istringstream ss(line);
        if(std::getline(ss, key, ' ')) {
          	std::cout << "Key:" << key << std::endl;
           	if(gameTexts.count(key) > 0) {
           		if(std::getline(ss, value)) {
           			gameTexts[key] = value;
                    std::cout<<key + gameTexts[key]<<std::endl<<std::endl;
           		}
           	}
        }
    }
/*
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
*/

    (*currLanguageStream)->close();


    for(auto const& key: gameTexts){
        if (key.second == NA_TEXT){
            std::cout<<"Warning--No match for key: \"" + key.first + "\" was found!"<<std::endl<<std::endl;
        }
    }

    livesText.text = livesText.textTemplate = gameTexts["LIVES"];
    scoreText.text = scoreText.textTemplate = gameTexts["SCORE"];

    livesText.setText(livesText.currCounter);
    scoreText.setText(scoreText.currCounter);
}


void SDLGraphicsProgram::editTile(Vector3D blockPos, std::string blockStr) {

    switch(gc){
        case -1:
            editTileBreakout(blockPos, blockStr);
            break;
        case -2:
            editTilePlatformer(blockPos, blockStr);
            break;
        case -3:
            //galaga stuff
            break;
    }

}

void SDLGraphicsProgram::editTileBreakout(Vector3D blockPos, std::string blockStr) {


    BreakoutLevel* lvl = edt_levels_breakout[edt_currLevelIndex];

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

void SDLGraphicsProgram::editTilePlatformer(Vector3D blockPos, std::string blockStr) {

    Level* lvl = edt_levels_platformer[edt_currLevelIndex];

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

void SDLGraphicsProgram::levelHelper(int lvlInt) {

    //std::cout<<edt_levels_platformer.size()<<"  level sizessss"<<std::endl;

    int currLevel = 0;
    int sizeCompare = 0;
    switch(gc){
        case -1:
            sizeCompare = edt_levels_breakout.size();
            currLevel = edt_levels_breakout.size() + 1;
            break;
        case -2:
            sizeCompare = edt_levels_platformer.size();
            currLevel = edt_levels_platformer.size() + 1;
            break;
        case -3:
            //galaga stuff
            break;
    }




    while ((size_t)sizeCompare < (size_t)lvlInt) {

        //make new file
        std::ofstream outfile(getResourcePath("level_config") + "lvl0" + std::to_string(currLevel) + ".cfg");

        std::string conts;

        switch(gc){
            case -1:
                for (int i = 0; i < Constants::Breakout::Game::SCREEN_UNIT_HEIGHT; i++) {

                    for (int k = 0; k < Constants::Breakout::Game::SCREEN_UNIT_WIDTH; k++) {

                        if (i == 0 && k == 0) {
                            conts += "P";
                            continue;
                        }

                        conts += ".";
                    }
                    if (i != Constants::Breakout::Game::SCREEN_UNIT_HEIGHT - 1) {
                        conts += "\n";
                    }
                }
                break;
            case -2:
                for (int i = 0; i < Constants::Platformer::Game::SCREEN_UNIT_HEIGHT; i++) {

                    for (int k = 0; k < Constants::Platformer::Game::SCREEN_UNIT_WIDTH; k++) {

                        if (i == 0 && k == 0) {
                            conts += "P";
                            continue;
                        }

                        conts += ".";
                    }
                    if (i != Constants::Platformer::Game::SCREEN_UNIT_HEIGHT - 1) {
                        conts += "\n";
                    }
                }
                break;
            case -3:
                //galaga stuff
                break;
        }



        outfile << conts << std::endl;

        outfile.close();



        switch(gc){
            case -1:
                edt_levels_breakout.push_back(new BreakoutLevel(getResourcePath("level_config") + "lvl0" + std::to_string(currLevel) + ".cfg"));
                break;
            case -2:
                edt_levels_platformer.push_back(new Level(getResourcePath("level_config") + "lvl0" + std::to_string(currLevel) + ".cfg"));
                break;
            case -3:
                //galaga stuff
                break;
        }




        currLevel++;
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
