//
// Created by Alejandro Hervella on 2/6/20.
//

#include "SDLGraphicsProgram.hpp"

#include <algorithm>
//#include "TinyMath.hpp"
#include <memory>

//for getting files from directory
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>


#include "Textbox.hpp"

#include "GameObject.hpp"

#include"TinyMath.hpp"

//#define SCREEN_WIDTH 500
//#define SCREEN_HEIGHT 500
#define SCREEN_TICKS_PER_FRAME 1000/60

//#define BRICK_ROWS 3
//#define BRICK_COLUMNS 8
//#define BRICK_WIDTHS 50
//#define BRICK_HEIGHTS 10
//#define BRICK_GROUP_DIST_FROM_CEILING 40


//#define BALL_SPEED 5
//#define BALL_RADIUS 5

//#define PADDLE_WIDTH 75
//#define PADDLE_HEIGHT 10
//#define PADDLE_DIST_FROM_CEILING 400
//#define PADDLE_SPEED 6
//#define DEFAULT_LIVES 3

//#define GAME_OVER_TEXT "Game Over! ~(O ^ o)~"
#define GAME_WON_TEXT "You won! Space for next Level!"
#define NA_TEXT "N/A"

class Level {


public:
    //2 for columns and rows
    std::vector<int> contents;
    int BRICK_COLUMNS;
    int BRICK_ROWS;

    Level(std::string configFilePath) {
        this -> configFilePath = configFilePath;
        //std::cout<<configFilePath<<std::endl;
        this -> configFileStream = ResourceManager::getInstance()->getConfigFileResource(configFilePath);
        parseStreamToString();

        BRICK_COLUMNS = contents[0];
        BRICK_ROWS = contents[1];
        std::cout<<BRICK_ROWS<<std::endl;

    }

    ~Level(){
        this -> configFileStream.reset();
        ResourceManager::getInstance()->deleteConfigFileResource(this-> configFilePath);
    }

private:
    std::string configFilePath;
    std::shared_ptr<std::ifstream*> configFileStream;


    void parseStreamToString(){

        int curr_line;

        while((*configFileStream)->good()){

            char c = (*configFileStream)->get();

            if (c == '\n'){
                contents.push_back(curr_line);
                c = (*configFileStream)->get();
                contents.push_back(c - '0');
                break;
            }
            curr_line = c - '0';
        }

    }


};

//const int TOTAL_BRICKS = BRICK_ROWS * BRICK_COLUMNS;
int BRICK_GROUP_X_POS = -1;

int BRICK_ROWS = -1;
int BRICK_COLUMNS  = -1;
//const SDL_Color BALL_COLOR = {250, 250, 250, 250};
const SDL_Color PADDLE_COLOR = {0, 250, 0, 250};
//const SDL_Color BRICK_COLOR = {50, 50, 50, 0};


std::vector<Level> levels;


Textbox centerText = Textbox(NA_TEXT, 20, SCREEN_WIDTH / 2 - 175, SCREEN_HEIGHT / 2 - 100);
CounterTextbox livesText = CounterTextbox(NA_TEXT, DEFAULT_LIVES, 15, 15, 3);
CounterTextbox scoreText = CounterTextbox(NA_TEXT, 0, 15, SCREEN_WIDTH - 75, 3);


std::map<std::string, std::string> gameTexts{
        {"LANG_MENU", NA_TEXT},
        {"DONE", NA_TEXT},
        {"START", NA_TEXT},
        {"LOSE", NA_TEXT},
        {"WIN", NA_TEXT},
        {"LIVES", NA_TEXT},
        {"SCORE", NA_TEXT}
};


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

        //prevent from getting other hidden non text file paths
        if (stat( filePath.c_str(), &filestat )) {continue;}
        if (S_ISDIR( filestat.st_mode ))  {continue;}

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

    //(*currLanguageStream)->open(languageFiles[langIndex].c_str(), std::fstream::out );
    //TODO: Fails on the first call to changeLanguage for some reason.
    /*
    blah->open(languageFiles[langIndex].c_str(), std::fstream::out );
    if(blah->fail()) {
    	std::cout << "Error: " << strerror(errno) << std::endl;
    }
    */
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
        /*
        ss >> key >> value; // set the variables
        if (gameTexts.count(key) > 0){
            gameTexts[key] = value;
            std::cout<<key + gameTexts[key]<<std::endl<<std::endl;
        }
        else{
            std::cout<<"Warning--Could not parse key: " + key + ", with value: " + value<<std::endl<<std::endl;
        }
        */
    }




/*
        while ( *(*currLanguageStream) >> key >> value ) {
            if (gameTexts.count(key) > 0){
                gameTexts[key] = value;
                std::cout<<key + gameTexts[key]<<std::endl<<std::endl;
            }
            else{
                std::cout<<"Warning--Could not parse key: " + key + ", with value: " + value<<std::endl<<std::endl;
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


void SDLGraphicsProgram::initLevelLoading(){


    getLanguages();
    for(auto i = languageFiles.begin(); i != languageFiles.end(); i++) {
    	std::cout << *i << std::endl;
    }
    //Default to English
    changeLanguage(0);

    std::ifstream fin;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
    std::string resourceConfigsPath = getResourcePath("level_config");



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

        //prevent from getting other hidden non text file paths
        if (stat( filePath.c_str(), &filestat )) {continue;}
        if (S_ISDIR( filestat.st_mode ))  {continue;}

        levels.push_back(Level(filePath));

    }



}



std::vector <Brick> bricks;
Ball ball;
Paddle paddle;
int prevCollisionID = -1;
int brickCollisionIndex = -1;

int lifeCount = DEFAULT_LIVES;
int score = 0;

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

void gameOver() {
    GameObject::firstGameOver = true;
    GameObject::gameOver = true;

    if (lifeCount < 0) {
        centerText.text = centerText.text = gameTexts["LOSE"];
        return;
    }

    centerText.text = gameTexts["WIN"];
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


void resetToLevel(Level lvl){
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

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h, std::string backgroundMusicFile) :
        screenWidth(w),
        screenHeight(h),
        gWindow(nullptr),
        gRenderer(nullptr) {
    // Initialize random number generation.

    initLevelLoading();
    resetToLevel(levels[GameObject::levelCount]);






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
    //Ball init with sound files
    ball.init(getResourcePath() + "459145__mattix__retro-pew-shot-01.wav", getResourcePath() + "219619__ani-music__pew-pew-sound-effect-peww1.wav");
    //Load background music
    this->backgroundMusicFile = getResourcePath() + backgroundMusicFile;
    backgroundMusic = resourceManager->getMusicResource(this->backgroundMusicFile);
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


// Update OpenGL
void SDLGraphicsProgram::update() {
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


// Render
// The render function gets called once per loop
//after update() function in the main loop (background
//and positions already changed by now)
void SDLGraphicsProgram::render() {

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


//Loops forever!
void SDLGraphicsProgram::loop() {


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

                            if ((size_t)GameObject::levelCount < levels.size() && GameObject::gameOver){
                                if(GameObject::levelCount != 0){
                                    resetToLevel(levels[GameObject::levelCount]);
                                }


                                GameObject::levelCount++;

                                if((size_t)GameObject::levelCount == levels.size()) {
                                    centerText.text = gameTexts["DONE"];
                                }

                                GameObject::gameOver = false;
                                centerText.text = " ";


                            }

                            //resetToLevel(levels[1]);
                        //}

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



