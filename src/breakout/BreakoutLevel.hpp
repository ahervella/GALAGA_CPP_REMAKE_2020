/*
 * BreakoutLevel.hpp
 *
 *  Created on: Mar 30, 2020
 *      Author: Karina
 */

#ifndef SRC_BREAKOUT_BREAKOUTLEVEL_HPP_
#define SRC_BREAKOUT_BREAKOUTLEVEL_HPP_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Brick.hpp"

class BreakoutLevel {

public:
    //2 for columns and rows

    std::vector<std::string> contents;

    std::vector<Brick> levelObjs;
    //std::vector<int> contents;
    //int BRICK_COLUMNS;
    //int BRICK_ROWS;

    BreakoutLevel(std::string configFilePath) {


        init(configFilePath);

        /*BRICK_COLUMNS = contents[0];
        BRICK_ROWS = contents[1];
        std::cout<<BRICK_ROWS<<std::endl;
        */
    }

    ~BreakoutLevel(){
        destroy();
    }



    void init(std::string configFilePath){
        this -> configFilePath = configFilePath;
        //std::cout<<configFilePath<<std::endl;
        this -> configFileStream = ResourceManager::getInstance()->getConfigFileResource(configFilePath);
        parseStreamToString();

    }

    void destroy(){
        this -> configFileStream.reset();
        ResourceManager::getInstance()->deleteConfigFileResource(this-> configFilePath);
    }

    void resetLevel(){
        destroy();
        init(this -> configFilePath);
    }


    void constructLevel(SDL_Renderer *gRenderer){

        levelObjs.clear();

        for (size_t i = 0; i < contents.size(); i++) {

            for (std::string::size_type j = 0; j < contents[i].size(); ++j) {

                Vector3D elementPos = Vector3D(j * Constants::Breakout::Game::BLOCK_UNIT_DIM.x * Constants::Breakout::Game::UNIT,
                                               i * Constants::Breakout::Game::UNIT * Constants::Breakout::Game::BLOCK_UNIT_DIM.y);



                Vector3D blockDim = Constants::Breakout::Game::BLOCK_UNIT_DIM * Constants::Breakout::Game::UNIT;

                std::string blockTextureFileName = Constants::Breakout::TexturePath::BLOCK;

                Brick brick;



                switch (contents[i][j]) {
                    case '.':
                        break;
                    case 'X':
                    case 'x':
                        //brick = Brick();//GameObject(elementPos, blockDim, Constants::Platformer::Game::Tag::BLOCK_TAG, clearClr, clearClr, blockTextureFileName,
                        brick.init(blockDim.x, blockDim.y, elementPos.x, elementPos.y, {0, 0, 255, 255});         //         gRenderer);
                        levelObjs.push_back(brick);
                        break;
                }
            }
        }
    }



    /**
     * Renders the level
     *
     * @param gRenderer: the renderer
     */
    void render(SDL_Renderer* gRenderer){
        for (Brick obj : levelObjs) {
            obj.render(gRenderer);
        }
    }


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

    void parseStreamToString(){

        contents.clear();

        std::string line;

        //while((*(*configFileStream)) >> a >> b){
        while (getline(*(*configFileStream), line)) {

            contents.push_back(line);
        }
    }
};


#endif /* SRC_BREAKOUT_BREAKOUTLEVEL_HPP_ */
