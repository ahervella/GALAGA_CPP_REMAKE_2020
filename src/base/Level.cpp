//
// Created by Alejandro Hervella on 2/27/20.
//

#include "Level.hpp"
#include "Collectible.hpp"
#include "Goal.hpp"
#include <SDL.h>

Level::Level(std::string configFilePath) {

    this->configFilePath = configFilePath;
    this->configFileStream = ResourceManager::getInstance()->getConfigFileResource(configFilePath);
    parseStreamToString();

}

Level::~Level() {
    this->configFileStream.reset();
    ResourceManager::getInstance()->deleteConfigFileResource(this->configFilePath);
}


void Level::parseStreamToString() {

    contents.clear();

    std::string line;

    //while((*(*configFileStream)) >> a >> b){
    while (getline(*(*configFileStream), line)) {

        contents.push_back(line);
    }

}


Level::ADJ_SIDE Level::adjacentRects(SDL_Rect r1, SDL_Rect r2) {
    bool adjRight = r1.x + r1.w == r2.x && r1.y == r2.y;
    bool adjLeft = r1.x == r2.x + r2.w && r1.y == r2.y;
    bool adjTop = r1.y == r2.y + r2.h && r1.x == r2.x;
    bool adjBottom = r1.y + r1.h == r2.y && r1.x == r2.x;

    if (adjRight) { return Level::ADJ_SIDE::RIGHT; }
    if (adjLeft) { return Level::ADJ_SIDE::LEFT; }
    if (adjTop) { return Level::ADJ_SIDE::TOP; }
    if (adjBottom) { return Level::ADJ_SIDE::BOTTOM; }
    return Level::ADJ_SIDE::NONE;
}

//this method is to calculate the level objects as horizontal and vertical shapes such that
//a level that looks like this:

//......X
//...X..X.......
//XXXXXXXXX....X

//will have the horizontal shapes:
//         X
//   X     X
//   X  &  X  &  X

//and the horizontal shapes:
//
//
//XXXXXXXXX  &  X

//this will avoid bugs with collisions as our character moves a long a line of collisions at any speed
void Level::calcCollisionObjs(SDL_Renderer *gRenderer) {

//TODO: double check this shit still works for clearning properly.
    std::vector <GameObject> thingsToKeep;
    for(GameObject obj : levelObjs) {
        if (obj.tag != Constants::Platformer::Game::Tag::BLOCK_TAG){
                thingsToKeep.push_back(obj);
        }
    }

    levelObjs.clear();

    for (GameObject obj : thingsToKeep){
        levelObjs.push_back(obj);
    }

    std::cout << "Made it past levelObjs" << std::endl;

    //here wil keep track of all the vertical shapes and all the horizontal shapes
    std::vector <SDL_Rect> verRects;
    std::vector <SDL_Rect> horRects;


    for (GameObject obj : blockObjs) {

        if (obj.tag != Constants::Platformer::Game::Tag::BLOCK_TAG){
        return;
        }

        //current obj rect we're checking
        SDL_Rect objR{(int) obj.pos.x, (int) obj.pos.y, (int) obj.dim.x, (int) obj.dim.y};

        //bool to use after the for loop to see if its added as a single block
        //instead of being added to an existing group of blocks (or other single block)
        bool makeObjRLoneVerRect = true;

        //loop through all vertical rects
        //here use delimeter to edit references
        for (SDL_Rect &verR : verRects) {

            //get the adjacent side the the other current verR block is relative to the objR block

            ADJ_SIDE verAdj = adjacentRects(objR, verR);

            //add to shape appropriately if on either top or bottom

            if (verAdj == Level::ADJ_SIDE::TOP) {
                verR.h = verR.h + objR.h;
                makeObjRLoneVerRect = false;
                break;
            }

            if (verAdj == Level::ADJ_SIDE::BOTTOM) {
                verR.y = objR.y;
                verR.h = verR.h + objR.h;
                makeObjRLoneVerRect = false;
                break;
            }
        }

        //if objR was never added to shape, then make it its own shape
        if (makeObjRLoneVerRect) {
            verRects.push_back(objR);
        }



        //do the same hor the horizontal
        bool makeObjRLoneHorRect = true;

        for (SDL_Rect &horR : horRects) {

            ADJ_SIDE horAdj = adjacentRects(objR, horR);

            if (horAdj == RIGHT) {
                horR.w = horR.w + objR.w;
                horR.x = objR.x;
                makeObjRLoneHorRect = false;
                break;
            }

            if (horAdj == Level::ADJ_SIDE::LEFT) {

                horR.w = horR.w + objR.w;
                makeObjRLoneHorRect = false;
                break;
            }
        }

        if (makeObjRLoneHorRect) {
            horRects.push_back(objR);
        }

}

        //now we need to ONLY KEEP the blocks that are shapes with more than one block
        //(aka dimensions that are not equal to the BLOCK_UNIT_DIM const) AND the blocks that are lone shapes
        //in BOTH the vertical and horizontal groups.

        //keep track of the indices from the horizontal and vertical shapes, respectively
        std::vector<int> verRectIndToKeep;
        std::vector<int> horRectIndToKeep;

        //for vertical shapes
        for (size_t i = 0; i < verRects.size(); ++i) {

            //shortcut for the current vertical shape
            SDL_Rect currVerR = verRects[i];

            //check if the shape is a lone shape (in the vertical group)
            if (currVerR.w == Constants::Platformer::Game::BLOCK_UNIT_DIM.x * Constants::Platformer::Game::UNIT
            && currVerR.h == Constants::Platformer::Game::BLOCK_UNIT_DIM.y * Constants::Platformer::Game::UNIT) {

                //It is a lone shape (in the vertical group). Find if its also a lone shape in the horizontal group
                for (size_t j = 0; j < horRects.size(); ++j) {

                    //shortcut for current horizontal shape
                    SDL_Rect currHorR = horRects[j];

                    //check if the horizontal shape is a lone shape (in the horizontal group)
                    if (currHorR.w == Constants::Platformer::Game::BLOCK_UNIT_DIM.x * Constants::Platformer::Game::UNIT
                    && currHorR.h == Constants::Platformer::Game::BLOCK_UNIT_DIM.y * Constants::Platformer::Game::UNIT) {

                        //it is also a lone shape. Now check if both lone shapes (one from the vert group, the other
                        //from the horiz group) are the SAME lone shape by checking their positions
                        if (currVerR.x == currHorR.x && currVerR.y == currHorR.y) {
                            //both lone shapes are the same shape, therefore we want to keep it (this logic is only
                            //for keeping vertical shapes, will need to do the same later for the horizontal group)
                            verRectIndToKeep.push_back(i);

                            //don't need to keep looking in the horizontal group
                            break;
                        }

                    }

                }
            }

                //if its not a lone shape, keep it
            else {
                verRectIndToKeep.push_back(i);
            }
        }


        //now do the same for the horizontal group (see above for details)
        for (size_t i = 0; i < horRects.size(); ++i) {

            SDL_Rect currHorR = horRects[i];

            if (currHorR.w == Constants::Platformer::Game::BLOCK_UNIT_DIM.x * Constants::Platformer::Game::UNIT
            && currHorR.h == Constants::Platformer::Game::BLOCK_UNIT_DIM.y * Constants::Platformer::Game::UNIT) {

                for (size_t j = 0; j < verRects.size(); ++j) {
                    SDL_Rect currVerR = verRects[j];

                    if (currVerR.w == Constants::Platformer::Game::BLOCK_UNIT_DIM.x * Constants::Platformer::Game::UNIT
                    && currVerR.h == Constants::Platformer::Game::BLOCK_UNIT_DIM.y * Constants::Platformer::Game::UNIT) {
                        if (currHorR.x == currVerR.x && currHorR.y == currVerR.y) {
                            horRectIndToKeep.push_back(i);
                            break;
                        }

                    }

                }
            } else {
                horRectIndToKeep.push_back(i);
            }
        }



        //Now we need to add the shapes we want to keep to the levelCollisionObjs! (we stored the indicies of each
        //group, so now we need to access those and add them to levelCollisionObjs)

        for (size_t i = 0; i < verRectIndToKeep.size(); ++i) {

            SDL_Rect rect = verRects[verRectIndToKeep[i]];
            GameObject obj = GameObject(Vector3D(rect.x, rect.y), Vector3D(rect.w, rect.h), Constants::Platformer::Game::Tag::BLOCK_TAG,
            Constants::Platformer::TexturePath::BLOCK, gRenderer);
            levelObjs.push_back(obj);
        }

        for (size_t i = 0; i < horRectIndToKeep.size(); ++i) {
                    SDL_Rect rect = horRects[horRectIndToKeep[i]];
                    GameObject obj = GameObject(Vector3D(rect.x, rect.y), Vector3D(rect.w, rect.h), Constants::Platformer::Game::Tag::BLOCK_TAG,
                    Constants::Platformer::TexturePath::BLOCK, gRenderer);

            levelObjs.push_back(obj);
        }


    //this program method has been brought to you by, WALUIGI!
}



//helper for the method below
void Level::constructLevel(SDL_Renderer *gRenderer) {

    std::cout << "Starting construction of level!" << std::endl;

    levelObjs.clear();
    enemyObjs.clear();

    float heightOffSet = Constants::Platformer::Game::UNIT * Constants::Platformer::Game::SCREEN_UNIT_HEIGHT -
                         contents.size() * Constants::Platformer::Game::BLOCK_UNIT_DIM.y * Constants::Platformer::Game::UNIT;

    for (size_t i = 0; i < contents.size(); i++) {

        for (std::string::size_type j = 0; j < contents[i].size(); ++j) {

            Vector3D elementPos = Vector3D(j * Constants::Platformer::Game::BLOCK_UNIT_DIM.x * Constants::Platformer::Game::UNIT,
                                           i * Constants::Platformer::Game::UNIT * Constants::Platformer::Game::BLOCK_UNIT_DIM.y);

            elementPos.y += heightOffSet;


            Vector3D playerDim = Constants::Platformer::Game::PLAYER_UNIT_DIM * Constants::Platformer::Game::UNIT;
            Vector3D blockDim = Constants::Platformer::Game::BLOCK_UNIT_DIM * Constants::Platformer::Game::UNIT;
            Vector3D goalDim = Constants::Platformer::Game::GOAL_UNIT_DIM * Constants::Platformer::Game::UNIT;
            Vector3D enemyDim = Constants::Platformer::Game::ENEMY_UNIT_DIM * Constants::Platformer::Game::UNIT;
            Vector3D collectibleDim = Constants::Platformer::Game::COLLECTIBLE_UNIT_DIM * Constants::Platformer::Game::UNIT;

            std::string playerTextureFileName = Constants::Platformer::TexturePath::PLAYER;
            std::string blockTextureFileName = Constants::Platformer::TexturePath::BLOCK;
            std::string collectibleTextureFileName = Constants::Platformer::TexturePath::COLLECTIBLE;
            std::string goalTextureFileName = Constants::Platformer::TexturePath::GOAL;
            std::string enemyTextureFileName = Constants::Platformer::TexturePath::ENEMY;

            SDL_Color clearClr = {0, 0, 0, 0};

            GameObject block;
            Collectible collectible;
            Enemy enemy;
            Goal goal;



            switch (contents[i][j]) {
                case '.':
                    break;
                case 'P':
                case 'p':
                    player = Player(elementPos, playerDim, playerTextureFileName, gRenderer);
                    std::cout << "make player" << std::endl;
                    break;
                case 'X':
                case 'x':
                    block = GameObject(elementPos, blockDim, Constants::Platformer::Game::Tag::BLOCK_TAG, clearClr, clearClr, blockTextureFileName,
                                                  gRenderer);
                    blockObjs.push_back(block);
                    std::cout << "Make block" << std::endl;
                    break;
                case 'C':
                case 'c':
                {
                	collectible = Collectible(elementPos, collectibleDim, collectibleTextureFileName, gRenderer);
                	levelObjs.push_back(collectible);
                	std::cout << "Collectible!" << std::endl;
                }
                	break;
                case 'G':
                case 'g':
                {
                	goal = Goal(elementPos, goalDim, goalTextureFileName, gRenderer);
                	levelObjs.push_back(goal);
                	std::cout << "Goal!" << std::endl;
                }
                	break;
                case 'E':
                case 'e':
                {
                	enemy = Enemy(elementPos, enemyDim, enemyTextureFileName, gRenderer);
                	enemyObjs.push_back(enemy);
                	std::cout << "Enemy!" << std::endl;
                }
                	break;
            }
        }
    }

    calcCollisionObjs(gRenderer);


}

void Level::render(SDL_Renderer *gRenderer) {
    for (GameObject obj : levelObjs) {
        obj.render(gRenderer);
    }

    for (GameObject obj : blockObjs) {
        obj.render(gRenderer);
    }

    for (Enemy e : enemyObjs) {
    	e.render(gRenderer);
    }


    player.render(gRenderer);
}
