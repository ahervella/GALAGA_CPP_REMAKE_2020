/*
 * GalagaLevel.hpp
 *
 *  Created on: Mar 30, 2020
 *      Author: Karina
 */

#ifndef SRC_GALAGA_GALAGALEVEL_HPP_
#define SRC_GALAGA_GALAGALEVEL_HPP_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Bady.hpp"
#include "PlayerShip.hpp"
#include "Bullet.hpp"

class GalagaLevel {

public:
    //2 for columns and rows

    std::vector<std::string> contents;

    std::vector<Bady> levelObjs;



    std::vector<Bullet> bullets;

    GalagaLevel(std::string configFilePath) {


        init(configFilePath);

    }

    ~GalagaLevel(){
        destroy();
    }



    void init(std::string configFilePath){
        this -> configFilePath = configFilePath;
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

        bullets.clear();
    }


    void constructLevel(SDL_Renderer *gRenderer){

        levelObjs.clear();

        for (size_t i = 0; i < contents.size(); i++) {

            for (std::string::size_type j = 0; j < contents[i].size(); ++j) {

                Vector3D elementPos = Vector3D(j * Constants::Galaga::Game::BADY_UNIT_DIM.x * Constants::Galaga::Game::UNIT,
                                               i * Constants::Galaga::Game::UNIT * Constants::Galaga::Game::BADY_UNIT_DIM.y);


                Vector3D badyDim = Constants::Galaga::Game::BADY_UNIT_DIM * Constants::Galaga::Game::UNIT;

                std::string badyTextureFileName = Constants::Galaga::TexturePath::BADY1;

                Bady bady;

                switch (contents[i][j]) {
                    case '.':
                        continue;
                    case '1':
                        badyTextureFileName = Constants::Galaga::TexturePath::BADY1;
                        bady = Bady(elementPos, badyDim, false, badyTextureFileName, gRenderer);
                        break;
                    case '2':
                        badyTextureFileName = Constants::Galaga::TexturePath::BADY2;
                        bady = Bady(elementPos, badyDim, false, badyTextureFileName, gRenderer);
                        break;
                    case '3':
                        badyTextureFileName = Constants::Galaga::TexturePath::BADY3;
                        bady = Bady(elementPos, badyDim, false, badyTextureFileName, gRenderer);
                        break;
                    case'4':
                        badyTextureFileName = Constants::Galaga::TexturePath::BADY4;
                        bady = Bady(elementPos, badyDim, false, badyTextureFileName, gRenderer);
                        break;
                    case '5':
                    	badyTextureFileName = Constants::Galaga::TexturePath::BADY1;
                    	bady = Bady(elementPos, badyDim, true, badyTextureFileName, gRenderer);
                    	break;
                }

                std::cout<<"check xxxxx2"<<std::endl;

                levelObjs.push_back(bady);
            }
        }
    }



    /**
     * Renders the level
     *
     * @param gRenderer: the renderer
     */
    void render(SDL_Renderer* gRenderer){
        for (Bady obj : levelObjs) {

            int dimX = (int) (obj.dim.x) * 0.7;
            int dimY = (int) (obj.dim.y) * 0.7;
            int posX = (int) obj.pos.x + ((int) (obj.dim.x) - dimX)/2;
            int posY = (int) obj.pos.y + ((int) (obj.dim.y) - dimY)/2;

            obj.renderCustomFillRect(gRenderer, {posX, posY, dimX, dimY});
        }
        for(Bullet bullet: bullets) {
        	bullet.render(gRenderer);
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

        while (getline(*(*configFileStream), line)) {

            contents.push_back(line);
        }
    }
};


#endif /* SRC_GALAGA_GALAGALEVEL_HPP_ */
