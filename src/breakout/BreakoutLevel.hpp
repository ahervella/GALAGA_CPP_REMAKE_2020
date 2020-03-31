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

class BreakoutLevel {

public:
    //2 for columns and rows
    std::vector<int> contents;
    int BRICK_COLUMNS;
    int BRICK_ROWS;

    BreakoutLevel(std::string configFilePath) {
        this -> configFilePath = configFilePath;
        //std::cout<<configFilePath<<std::endl;
        this -> configFileStream = ResourceManager::getInstance()->getConfigFileResource(configFilePath);
        parseStreamToString();

        BRICK_COLUMNS = contents[0];
        BRICK_ROWS = contents[1];
        std::cout<<BRICK_ROWS<<std::endl;
    }

    ~BreakoutLevel(){
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


#endif /* SRC_BREAKOUT_BREAKOUTLEVEL_HPP_ */
