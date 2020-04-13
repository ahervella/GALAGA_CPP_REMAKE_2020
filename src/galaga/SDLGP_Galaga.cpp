/*
 * SDLGP_Galaga.cpp
 *
 *  Created on: Apr 6, 2020
 *      Author: Karina
 */

#include "SDLGP_Galaga.hpp"
#include <algorithm>

PlayerShip playerShip;

SDLGP_Galaga::SDLGP_Galaga()
{
	initLevel();
	this->backgroundImageFile = Constants::Galaga::TexturePath::BACKGROUND;
	loadBackgroundMusicAndImage();
}

SDLGP_Galaga::~SDLGP_Galaga()
{

}



void SDLGP_Galaga::makePlayerShip() {

    std::string playerShipTexturePath = Constants::Galaga::TexturePath::PLAYERSHIP;

    playerShip = PlayerShip(Vector3D(0, 0), Vector3D(0, 0),  playerShipTexturePath, gRenderer);

    resetShipPosition();
}


void SDLGP_Galaga::getLanguages()
{
    std::ifstream fin;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
    std::string resourceConfigsPath = "";

    resourceConfigsPath = getResourcePath("galaga/lang_config");

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

void SDLGP_Galaga::changeLanguage(int langIndex){

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
    if(!GameObject::gameOver) {
        centerText.text = " ";
    }
    else {
        int lives = 0;
        int level_num = 0;
        int level_max = 0;
        //A result of using two different variables each for lives and level number depending on the game.


                lives = PlayerShip::lifeCount;
                level_num = currLevelIndex;
                level_max = GalagaLevels.size();


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



void SDLGP_Galaga::resetShipPosition(){
    int shipPosX = (Constants::Galaga::Game::SCREEN_UNIT_WIDTH / 2 - Constants::Galaga::Game::PLAYER_UNIT_DIM.x)
                   * Constants::Galaga::Game::UNIT;

    int shipPosY = (Constants::Galaga::Game::SCREEN_UNIT_HEIGHT) * Constants::Galaga::Game::UNIT;

    Vector3D shipDim = Constants::Galaga::Game::PLAYER_UNIT_DIM * Constants::Galaga::Game::UNIT;
    playerShip.resetPosition(Vector3D(shipPosX, shipPosY), shipDim);
}


void SDLGP_Galaga::resetToLevel(){


    resetShipPosition();

    centerText.text = " ";

    //Reset score to pre-level score
    score -= currLevelScore;
    currLevelScore = 0;

    scoreText.setText(score);
}

void SDLGP_Galaga::update()
{

    if(GameObject::gameOver) {
        return;
    }
    //render background
    SDL_SetRenderDrawColor(gRenderer, 0x22, 0x22, 0x22, 0xFF);
    SDL_RenderClear(gRenderer);

    //Check for collision between player and enemies/bullets

    std::vector<Bady> objs = GalagaLevels[currLevelIndex].levelObjs;//levels[currLevelIndex].levelObjs;


    std::vector<Bullet> bullets = GalagaLevels[currLevelIndex].bullets;
    //If we have a collision, let the player figure out what to do based on where the collision is from
    //TODO: For future, figure out

    scoreText.setText(playerShip.getScore());


    for(size_t i = 0; i < GalagaLevels[currLevelIndex].levelObjs.size(); ++i) {
    	if(GalagaLevels[currLevelIndex].levelObjs[i].hasFired()) {
    		Vector3D pos = GalagaLevels[currLevelIndex].levelObjs[i].pos;
    		Vector3D dim = GalagaLevels[currLevelIndex].levelObjs[i].dim;
    		GalagaLevels[currLevelIndex].bullets.push_back
				(Bullet(pos, dim, Constants::Galaga::Game::Tag::BADY_BULLET_TAG, Constants::Galaga::TexturePath::BULLET, gRenderer));
    	}
    }
    if(playerShip.hasFired()) {
    	Vector3D pos = playerShip.pos;
    	Vector3D dim = playerShip.dim;
    	GalagaLevels[currLevelIndex].bullets.push_back
    			(Bullet(pos, dim, Constants::Galaga::Game::Tag::PLAYER_BULLET_TAG, Constants::Galaga::TexturePath::BULLET, gRenderer));
    }

    //TODO: Could use this later on for grabbing powerups
    /*
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
     */


    std::vector<int> bulletsToRemove;
    std::vector<int>badysToRemove;
    //std::vector<Bullet> bulletsToRemove;
    //std::vector<Bady> badysToRemove;
    for(size_t i = 0; i < bullets.size(); ++i) {
    	if(bullets[i].pos.y < 0 || bullets[i].pos.y > (Constants::Galaga::Game::UNIT * Constants::Galaga::Game::SCREEN_UNIT_HEIGHT)) {
    		bulletsToRemove.push_back(i);
    		continue;
    	}
    	//Could be a player bullet or enemy bullet
    	int tag = bullets[i].tag;
    	bool hit = false;
    	for(size_t k = 0; k < objs.size(); ++k) {
    		if(objs[k].collisionUpdate(objs[k].isColliding(bullets[i]), tag)) {
    			hit = true;
    			badysToRemove.push_back(k);
        		playerShip.setScore(playerShip.getScore() + 1);
    			break;
    		}
    	}
    	if(!hit && playerShip.collisionUpdate(playerShip.isColliding(bullets[i]), tag)) {
    		hit = true;
    		loseLife();
    		if(GameObject::gameOver) {
    		    //Stop the game if we hit game over after losing a life
    		    return;
    		}
    	}
    	if(hit) {
    		bulletsToRemove.push_back(i);
    	}
    }

    std::vector<Bullet> bulletsToKeep;
    std::vector<Bady> badysToKeep;
    for(size_t i = 0; i < bullets.size(); ++i) {
    	auto it = std::find(bulletsToRemove.begin(), bulletsToRemove.end(), i);
    	if(it == bulletsToRemove.end()) {
    		bulletsToKeep.push_back(bullets[i]);
    	}
    }

    for(size_t k = 0; k < bullets.size(); ++k) {
        auto it = std::find(badysToRemove.begin(), badysToRemove.end(), k);
        if(it == badysToRemove.end()) {
        	badysToKeep.push_back(objs[k]);
        }
    }
    //GalagaLevels[currLevelIndex].bullets = bulletsToKeep;
    //GalagaLevels[currLevelIndex].levelObjs = badysToKeep;

    /*
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
    */


    //all velocities should be final at this point in frame for player, apply to position
    //std::cout<<"size: " + std::to_string(objs.size())<<std::endl;

    //p->move();
    //p->update();

    for(size_t i = 0; i < GalagaLevels[currLevelIndex].levelObjs.size(); ++i) {
    	GalagaLevels[currLevelIndex].levelObjs[i].update();
    }
    for(size_t k = 0; k < GalagaLevels[currLevelIndex].bullets.size(); ++k) {
    	GalagaLevels[currLevelIndex].bullets[k].update();
    }


    playerShip.move();
    playerShip.update();
    /*
    for(size_t k = 0; k < PlatformerLevels[currLevelIndex].enemyObjs.size(); ++k) {
    	PlatformerLevels[currLevelIndex].enemyObjs[k].move();
    	PlatformerLevels[currLevelIndex].enemyObjs[k].update();
    }
    */
}

void SDLGP_Galaga::render()
{
    SDL_RenderCopy(getSDLRenderer(), *backgroundImage, NULL, NULL);

    GalagaLevels[currLevelIndex].render(getSDLRenderer());
    playerShip.render(getSDLRenderer());

    renderTexts();

    SDL_RenderPresent(getSDLRenderer());

}

void SDLGP_Galaga::loop()
{

    //set alpha channel on
    SDL_SetRenderDrawBlendMode(getSDLRenderer(), SDL_BLENDMODE_BLEND);

    makePlayerShip();

    GalagaLevels[currLevelIndex].constructLevel(getSDLRenderer());


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
                        else {
                            playerShip.shoot();
                        }
                        break;

                    case SDLK_n:

                        if (started && (size_t) currLevelIndex < GalagaLevels.size() && GameObject::gameOver && lifeCount >= 0){


                            //Advancing to next level --> we keep currLevelScore and don't subtract it in resetToLevel()
                            //This will prevent that
                            currLevelScore = 0;

                            if((size_t)currLevelIndex + 1 == GalagaLevels.size()) {
                                //This is set elsewhere when calculating the game over conditions (win/lose/end of game)
                                //So probably not needed
                                centerText.text = gameTexts["DONE"];
                                //TODO: More graceful exit? Quitting/going to the next level at this point causes a whacky exit code
                                return;
                            }

                            currLevelIndex++;

                            if(currLevelIndex != 0){
                                resetToLevel();
                            }




                            GameObject::gameOver = false;
                            centerText.text = " ";
                        }

                        break;

                    case SDLK_RIGHT:
                        if (!rightJustPressed){
                            playerShip.changeDirection(1);
                            rightJustPressed = true;
                        }
                        break;
                    case SDLK_LEFT:
                        if (!leftJustPressed){
                            playerShip.changeDirection(-1);
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
                            if(PlayerShip::lifeCount < 0) {
                                lifeCount = DEFAULT_LIVES;
                                //Reset score, set score and life texts, reset center text
                                centerText.text = " ";
                                livesText.setText(PlayerShip::lifeCount);
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
                playerShip.changeDirection(-1);
            }

            if (!leftJustPressed && rightJustPressed){
                playerShip.changeDirection(1);
            }

            if (!leftJustPressed && !rightJustPressed){
                playerShip.changeDirection(0);
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

void SDLGP_Galaga::gameOver()
{
	GameObject::gameOver = true;

	std::cout << "Game over set" << std::endl;

	int lives = PlayerShip::lifeCount;
	int level_num = currLevelIndex;
	int level_max = GalagaLevels.size();

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

void SDLGP_Galaga::loseLife()
{
	PlayerShip::lifeCount -= 1;

	if(PlayerShip::lifeCount < 0) {
		gameOver();
		return;
	}
}

void SDLGP_Galaga::initLevel()
{
	getLanguages();
	//Default to English
	changeLanguage(0);

	std::ifstream fin;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
    std::string resourceConfigsPath = "";

    resourceConfigsPath = getResourcePath("galaga/level_config");

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

        GalagaLevels.push_back(GalagaLevel(filePath));
    }
}

