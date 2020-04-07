/*
 * SDLGP_Breakout.cpp
 *
 *  Created on: Apr 6, 2020
 *      Author: Karina
 */

#include "SDLGP_Platformer.hpp"

SDLGP_Platformer::SDLGP_Platformer()
{
	backgroundMusicFile = Constants::Platformer::SFXPath::MUSIC;
	backgroundImageFile = Constants::Platformer::TexturePath::BACKGROUND;
	loadBackgroundMusicAndImage();
	initLevel();
}

SDLGP_Platformer::~SDLGP_Platformer()
{

}

// Update OpenGL
void SDLGP_Platformer::update() {
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

void SDLGP_Platformer::render() {

	SDL_RenderCopy(getSDLRenderer(), *backgroundImage, NULL, NULL);

	PlatformerLevels[currLevelIndex].render(getSDLRenderer());

    SDL_RenderPresent(getSDLRenderer());

}

//Loops forever!
void SDLGP_Platformer::loop() {
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

    //Quick fix for being frozen at start of game
    //Will need to remove later to print instructions to screen before starting
    GameObject::gameOver = false;

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

                	case SDLK_n:
                		if ((size_t)currLevelIndex < PlatformerLevels.size() && GameObject::gameOver && Player::lifeCount >= 0){



                		    if((size_t)currLevelIndex + 1 == PlatformerLevels.size()) {
                		        centerText.text = gameTexts["DONE"];
                		        return;
                		    }

                		    currLevelIndex++;

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

                    case SDLK_q:
                        quit = true;
                        break;
                    case SDLK_r:
                    	if(Player::lifeCount < 0) {
                    		Player::lifeCount = 0;
                    	}
                    	PlatformerLevels[currLevelIndex].constructLevel(getSDLRenderer());
                        GameObject::gameOver = false;
                        centerText.text = " ";
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

void SDLGP_Platformer::gameOver() {
    GameObject::gameOver = true;

    std::cout << "Game over set" << std::endl;
    int lives = 0;
    int level_num = 0;
    int level_max = 0;

    //Player::lifeCount holds the lives for Platformer

   	lives = Player::lifeCount;
   	level_num = currLevelIndex;
   	level_max = PlatformerLevels.size();

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

void SDLGP_Platformer::loseLife() {

	Player::lifeCount -= 1;

	if(Player::lifeCount < 0) {
		gameOver();
		return;
	}
}

void SDLGP_Platformer::initLevel()
{
    std::ifstream fin;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
    std::string resourceConfigsPath = "";

    resourceConfigsPath = getResourcePath("platformer/level_config");

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

        PlatformerLevels.push_back(Level(filePath));
    }
}
