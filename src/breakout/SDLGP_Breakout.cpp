/*
 * SDLGP_Breakout.cpp
 *
 *  Created on: Apr 6, 2020
 *      Author: Karina
 */

#include "SDLGP_Breakout.hpp"

#include "Ball.hpp"
#include "Paddle.hpp"
#include "Brick.hpp"
#include "Constants.hpp"

Ball ball;
Paddle paddle;
int prevCollisionID = -1;
int brickCollisionIndex = -1;

//in SDLGraphicsProgram hpp
//int lifeCount = DEFAULT_LIVES;

int BRICK_GROUP_X_POS = -1;
int BRICK_ROWS = -1;
int BRICK_COLUMNS  = -1;

const SDL_Color PADDLE_COLOR = {0, 250, 0, 250};

SDLGP_Breakout::SDLGP_Breakout()
{
	//Ball init with sound files
   	ball.init(Constants::Breakout::SFXPath::BALL_BLOCK, Constants::Breakout::SFXPath::BALL_WALL);
   	backgroundMusicFile = Constants::Breakout::SFXPath::MUSIC;

   	initLevel();
}

SDLGP_Breakout::~SDLGP_Breakout()
{

}

// Update OpenGL
void SDLGP_Breakout::update() {

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

void SDLGP_Breakout::render() {

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
    //Trying to skip new line characters here with strtok. There are better ways, but this will work
    int y = centerText.y;
    //
    bool first = true;
    char * origText = new char[centerText.text.length() + 1];
    strcpy(origText, centerText.text.c_str());
    //Loop for writing new lines on a seaparate line
    char * line = strtok(origText, "\\");
    while (line != NULL) {
    	//A hack for skipping the 'n' in '\n' after strtok skips the '\'.
    	//The first time around, the beginning of the string is the start of the string and not the trailing 'n'
    	if(first) {
    		first = false;
    	}
    	//After the first strtok call, the string will start with 'n'. Skip that.
    	else {
    		//Make sure there is a character afterwards
    		if(*line + 1 != '\0') {
    			line += 1;
    		}
    	}
    	SDL_Texture *centerTextImage = renderText(line, livesText.textResPath, centerText.clr,
    	                                              centerText.textSize, getSDLRenderer());
    	//TODO: Get centered position here. It fits, but it could look better, y'know?
    	renderTexture(centerTextImage, getSDLRenderer(), centerText.x, y);

        SDL_DestroyTexture(centerTextImage);
        line = strtok(NULL, "\\");
        y += centerText.textSize;
    }

    delete [] origText;

    SDL_RenderPresent(getSDLRenderer());
    SDL_DestroyTexture(livesTextImage);
    SDL_DestroyTexture(scoreTextImage);

}

//Loops forever!
void SDLGP_Breakout::loop() {

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
    bool started = false;
    centerText.text = gameTexts["START"];

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

                	//Essentially, only have space do something when we need to start the game
                	case SDLK_SPACE:
                		if(!started) {
                			started = true;
                			GameObject::gameOver = false;
                			centerText.text = " ";
                		}
                		break;

                    case SDLK_n:

                            if (started && (size_t) levelCount < BreakoutLevels.size() && GameObject::gameOver && lifeCount >= 0){


                            	//Advancing to next level --> we keep currLevelScore and don't subtract it in resetToLevel()
                            	//This will prevent that
                            	currLevelScore = 0;

                            	if((size_t)levelCount + 1 == BreakoutLevels.size()) {
                            		//This is set elsewhere when calculating the game over conditions (win/lose/end of game)
                            		//So probably not needed
                            		centerText.text = gameTexts["DONE"];
                            		//TODO: More graceful exit? Quitting/going to the next level at this point causes a whacky exit code
                            		return;
                            	}

                            	levelCount++;

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
                    case SDLK_r:
                    	if(started) {
							if(lifeCount < 0) {
								lifeCount = DEFAULT_LIVES;
								//Reset score, set score and life texts, reset center text
								centerText.text = " ";
								livesText.setText(lifeCount);
								score = 0;
								//If we restart after losing all lives, we don't want to subtract the score earned in the level from 0
								currLevelScore = 0;
								scoreText.setText(score);
							}
							resetToLevel();
							GameObject::gameOver = false;
							break;
                    	}
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

}


void SDLGP_Breakout::resetToLevel(){

    BreakoutLevels[levelCount].resetLevel();

    BreakoutLevels[levelCount].constructLevel(getSDLRenderer());

    //Reset text
    centerText.text = " ";

    //Reset score to pre-level score
    score -= currLevelScore;
    currLevelScore = 0;

    ball.resetBall();
    scoreText.setText(score);

    float paddleCenter = (SCREEN_WIDTH - PADDLE_WIDTH) / 2.0;
    paddle.init(PADDLE_WIDTH, PADDLE_HEIGHT, paddleCenter, PADDLE_DIST_FROM_CEILING, PADDLE_COLOR);

}

void SDLGP_Breakout::checkBrickCollision(float &newAngle, int &collisionID, int &brickCollisionIndex) {

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

void SDLGP_Breakout::removeBrick(int rmIndex) {


    scoreText.setText(++score);
    currLevelScore++;

    BreakoutLevels[levelCount].levelObjs.erase(BreakoutLevels[levelCount].levelObjs.begin() + rmIndex);

    std::cout<<BreakoutLevels[levelCount].levelObjs.size()<<std::endl;
    if (BreakoutLevels[levelCount].levelObjs.size() <= 0) {
        gameOver();
        return;
    }
}

void SDLGP_Breakout::updateBallCollisions() {

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

void SDLGP_Breakout::gameOver() {
    GameObject::gameOver = true;

    std::cout << "Game over set" << std::endl;
    int lives = 0;
    int level_num = 0;
    int level_max = 0;
    //Probably should refactor this, but lifeCount is the global variable used for breakout

   	lives = lifeCount;
   	level_num = levelCount;
   	level_max = BreakoutLevels.size();

	if (lives < 0) {
		centerText.text = gameTexts["LOSE"];
		std::cout << "Game over text set (lose)" << std::endl;
		return;
	}

	if(level_num + 1 == level_max) {
		centerText.text = gameTexts["DONE"];
		std::cout << "Game done text set" << std::endl;
	}
	else {
		centerText.text = gameTexts["WIN"];
		std::cout << "Game over text set (win)" << std::endl;
	}
}

void SDLGP_Breakout::loseLife() {

	lifeCount -= 1;
   	if (lifeCount < 0) {
       	gameOver();
       	return;
   	}

   	ball.resetBall();
   	livesText.setText(lifeCount);
}

void SDLGP_Breakout::getLanguages()
{
    std::ifstream fin;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
    std::string resourceConfigsPath = "";

    resourceConfigsPath = getResourcePath("breakout/lang_config");

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


void SDLGP_Breakout::changeLanguage(int langIndex){

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
    if(!GameObject::gameOver) {
    	centerText.text = " ";
    }
    else {
    	int lives = 0;
    	int level_num = 0;
    	int level_max = 0;
    	//A result of using two different variables each for lives and level number depending on the game.

    	lives = lifeCount;
        level_num = levelCount;
        level_max = BreakoutLevels.size();


    	if(lives < 0) {
    		centerText.text = gameTexts["LOSE"];
    	}
    	else {
    		if(level_num + 1 == level_max) {
    			centerText.text = gameTexts["DONE"];
    			return;
    		}
    		centerText.text = gameTexts["WIN"];
    	}
    }
}

void SDLGP_Breakout::initLevel()
{
    getLanguages();
    //Default to English
    changeLanguage(0);

    std::ifstream fin;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
    std::string resourceConfigsPath = "";

    resourceConfigsPath = getResourcePath("breakout/level_config");

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

        BreakoutLevels.push_back(BreakoutLevel(filePath));
    }
}
