//
// Created by Alejandro Hervella on 2/27/20.
//

#ifndef PROJ2_PLATFORMER_WALUIGI_SEND_TWEET_CONSTANTS_HPP
#define PROJ2_PLATFORMER_WALUIGI_SEND_TWEET_CONSTANTS_HPP

#include "TinyMath.hpp"
#include <string>
#include "util.hpp"

namespace Constants {

    namespace Galaga{
        namespace Game{
            const float UNIT = 40;
            const Vector3D BADY_UNIT_DIM = Vector3D(1, 1);

            const float SCREEN_UNIT_WIDTH = 12;//20
            const float SCREEN_UNIT_HEIGHT = 10;//11
            const Vector3D PLAYER_UNIT_DIM = Vector3D(1, 1);//1.4, 2);
            const int LIVES = 4;

            namespace Tag {
                const int BADY_TAG = 1;
                const int PLAYER_TAG = 2;
                const int PLAYER_BULLET_TAG = 3;
                const int BADY_BULLET_TAG = 4;
            }

        }

        namespace TexturePath{
            const std::string CLEAR = getResourcePath("platformer/sprites") + "clear.png";
            const std::string BULLET = getResourcePath("galaga") + "galaga-png-101-images-in-collection-page-2-galaga-png-1152_1152.png";
            const std::string BADY1 = getResourcePath("galaga") + "Bady1.png";
            const std::string BADY2 = getResourcePath("galaga") + "Bady2.png";
            const std::string BADY3 = getResourcePath("galaga") + "Bady3.png";
            const std::string BADY4 = getResourcePath("galaga") + "Bady4.png";
            const std::string BACKGROUND = getResourcePath("galaga") + "galaga-background.png";
            const std::string PLAYERSHIP = getResourcePath("galaga") + "AlienPlayer.png";


        }

        namespace SFXPath {
            const std::string MUSIC = getResourcePath("galaga") + "Csum_-_01_-_Wild_Colours.wav";
            const std::string SHOOT = getResourcePath("galaga") + "428638__iut-paris8__quillard-charles-2018-laser.wav";
            const std::string LOSE = getResourcePath("galaga") + "350986__cabled-mess__lose-c-01.wav";
            const std::string LOSELIFE = getResourcePath("galaga") + "107788__leviclaassen__hit-001.wav";
            const std::string ENEMYHIT = getResourcePath("galaga") + "362423__alphatrooper18__explosion1.wav";
        }
    }

	namespace Platformer {
		namespace Game {

			const float UNIT = 40;
			const Vector3D BLOCK_UNIT_DIM = Vector3D(1, 1);

			//TODO: Subject to change
			const Vector3D COLLECTIBLE_UNIT_DIM = Vector3D(1, 1);
			const Vector3D GOAL_UNIT_DIM = Vector3D(1, 1);
			const Vector3D ENEMY_UNIT_DIM = Vector3D(1, 1);

			const float SCREEN_UNIT_WIDTH = 12;//20
			const float SCREEN_UNIT_HEIGHT = 14;//11
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

			const std::string PLAYER = getResourcePath("platformer/sprites") + "droid_from_android.png";
			const std::string BLOCK = getResourcePath("platformer/sprites") + "block_tiles_red.png";
			const std::string BACKGROUND = getResourcePath("platformer/sprites") + "7536921_orig.png";
			const std::string COLLECTIBLE = getResourcePath("platformer/sprites") + "coin.png";
			//TODO: GET GOAL SPRITE
			const std::string GOAL = getResourcePath("platformer/sprites") + "174-1743878_open-flag-sprite-clipart.png";
			//TODO: GET ENEMY SPRITE
			const std::string CLEAR = getResourcePath("platformer/sprites") + "clear.png";
			const std::string ENEMY = getResourcePath("platformer/sprites") + "droid_from_android_evil.png";


		}

		namespace SFXPath {
			const std::string JUMP = getResourcePath("platformer/SFX") + "403369__corpocracy__2dcharacter-jump01.wav";
			//TODO: GET COLLECT SFX
			const std::string COLLECT = getResourcePath("platformer/SFX") + "341695__projectsu012__coins-1.wav";
			//TODO: GET GOAL SFX
			const std::string GOAL = getResourcePath("platformer/SFX") + "249524__pearcewilsonking__badass-victory.wav";
			//TODO: GET LOSE SFX
			const std::string LOSE = getResourcePath("platformer/SFX") + "350984__cabled-mess__lose-c-03.wav";
			const std::string MUSIC = getResourcePath("platformer/Music") + "476547__mrthenoronha__platform-game-theme-loop-3.wav";
		}
	}
	namespace Breakout {
	    namespace Game{
            const float UNIT = 40;
            const Vector3D BLOCK_UNIT_DIM = Vector3D(1, 1);

            const float SCREEN_UNIT_WIDTH = 12;//20
            const float SCREEN_UNIT_HEIGHT = 14;//11
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

	    namespace TexturePath{
            const std::string CLEAR = getResourcePath("platformer/sprites") + "clear.png";
            const std::string BLOCK = getResourcePath("platformer/sprites") + "block_tiles_red.png";

        }

		namespace SFXPath {
			const std::string BALL_BLOCK = getResourcePath("breakout") + "459145__mattix__retro-pew-shot-01.wav";
			const std::string BALL_WALL = getResourcePath("breakout") + "219619__ani-music__pew-pew-sound-effect-peww1.wav";
			const std::string MUSIC = getResourcePath("breakout") + "415804__sunsai__mushroom-background-music (1).wav";
		}
	}
}

#endif //PROJ2_PLATFORMER_WALUIGI_SEND_TWEET_CONSTANTS_HPP
