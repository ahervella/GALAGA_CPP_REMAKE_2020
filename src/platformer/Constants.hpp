//
// Created by Alejandro Hervella on 2/27/20.
//

#ifndef PROJ2_PLATFORMER_WALUIGI_SEND_TWEET_CONSTANTS_HPP
#define PROJ2_PLATFORMER_WALUIGI_SEND_TWEET_CONSTANTS_HPP

#include "TinyMath.hpp"
#include <string>
#include "util.hpp"

namespace Constants {
    namespace Game {

        const float UNIT = 40;
        const Vector3D BLOCK_UNIT_DIM = Vector3D(1, 1);

        //TODO: Subject to change
        const Vector3D COLLECTIBLE_UNIT_DIM = Vector3D(1, 1);
        const Vector3D GOAL_UNIT_DIM = Vector3D(1, 1);
        const Vector3D ENEMY_UNIT_DIM = Vector3D(1, 1);

        const float SCREEN_UNIT_WIDTH = 20;
        const float SCREEN_UNIT_HEIGHT = 11;
        const Vector3D PLAYER_UNIT_DIM = Vector3D(1, 1);//1.4, 2);
        const int LIVES = 0;

        namespace Tag {
        	const int BLOCK_TAG = 1;
        	const int PLAYER_TAG = 2;
        	const int GOAL_TAG = 3;
        	const int COLLECTIBLE_TAG = 4;
        	const int ENEMY_TAG = 5;
        }
    }

    namespace TexturePath {

        const std::string PLAYER = getResourcePath("sprites") + "droid_from_android.png";
        const std::string BLOCK = getResourcePath("sprites") + "block_tiles_red.png";
        const std::string BACKGROUND = getResourcePath("sprites") + "7536921_orig.png";
        const std::string COLLECTIBLE = getResourcePath("sprites") + "coin.png";
        //TODO: GET GOAL SPRITE
        const std::string GOAL = getResourcePath("sprites") + "174-1743878_open-flag-sprite-clipart.png";
        //TODO: GET ENEMY SPRITE
        const std::string CLEAR = getResourcePath("sprites") + "clear.png";
        const std::string ENEMY = getResourcePath("sprites") + "droid_from_android_evil.png";


    }

    namespace SFXPath {
        const std::string JUMP = getResourcePath("SFX") + "403369__corpocracy__2dcharacter-jump01.wav";
        //TODO: GET COLLECT SFX
        const std::string COLLECT = getResourcePath("SFX") + "341695__projectsu012__coins-1.wav";
        //TODO: GET GOAL SFX
        const std::string GOAL = getResourcePath("SFX") + "249524__pearcewilsonking__badass-victory.wav";
        //TODO: GET LOSE SFX
        const std::string LOSE = getResourcePath("SFX") + "350984__cabled-mess__lose-c-03.wav";
        const std::string MUSIC = getResourcePath("Music") + "476547__mrthenoronha__platform-game-theme-loop-3.wav";


    }
}

#endif //PROJ2_PLATFORMER_WALUIGI_SEND_TWEET_CONSTANTS_HPP