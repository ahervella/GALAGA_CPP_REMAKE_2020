#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Constants.hpp"
#include <vector>
#include "Player.hpp"
#include <iostream>
#include <fstream>

#include "Enemy.hpp"

/**
 * A class that represents a level in the game. Contains a collection of all the game objects in a level.
 */
class Level {


public:
    enum ADJ_SIDE {RIGHT, LEFT, TOP, BOTTOM, NONE};
    //2 for columns and rows
    std::vector<std::string> contents;
    std::vector<GameObject> levelObjs;
    std::vector<GameObject> blockObjs;
    std::vector<Enemy> enemyObjs;
    Player player;

    /**
     * Level constructor that takes a file path for the level config
     *
     * @param configFilePath: file path to level config file
     */
    Level(std::string configFilePath);

    /**
     * Destructor
     */
    ~Level();

    /**
     * Constructs a level and copies the level elements to the renderer (i.e. blocks)
     *
     * @param gRenderer: the renderer
     */
    void constructLevel(SDL_Renderer* gRenderer);

    /**
     * Renders the level
     *
     * @param gRenderer: the renderer
     */
    void render(SDL_Renderer* gRenderer);

    void writeToCfgFile(std::string contents, SDL_Renderer* gRenderer){
        std::ofstream lvlCfgFile;
        lvlCfgFile.open(configFilePath);
        lvlCfgFile << contents;
        lvlCfgFile.close();

        this->configFileStream.reset();
        ResourceManager::getInstance()->deleteConfigFileResource(configFilePath);
        configFileStream = ResourceManager::getInstance()->getConfigFileResource(configFilePath);


        parseStreamToString();


        constructLevel(gRenderer);

    };

private:

    std::string configFilePath;
    std::shared_ptr<std::ifstream*> configFileStream;

    void constructLevel();

    ADJ_SIDE adjacentRects(SDL_Rect r1, SDL_Rect r2);

    void calcCollisionObjs(SDL_Renderer* gRenderer);

    void parseStreamToString();


};

#endif //LEVEL_HPP
