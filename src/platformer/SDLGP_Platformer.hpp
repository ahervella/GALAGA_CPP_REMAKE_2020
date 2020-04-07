/*
 * SDLGP_Breakout.hpp
 *
 *  Created on: Apr 6, 2020
 *      Author: Karina
 */

#ifndef SRC_PLATFORMER_SDLGP_PLATFORMER_HPP_
#define SRC_PLATFORMER_SDLGP_PLATFORMER_HPP_

#include "SDLGraphicsProgram.hpp"

class SDLGP_Platformer : SDLGraphicsProgram
{
public:

	SDLGP_Platformer();
	~SDLGP_Platformer();

	void update();
	void render();
	void loop();

	void gameOver();
	void loseLife();

	void initLevel();

private:
	std::vector<Level> PlatformerLevels;

};



#endif /* SRC_PLATFORMER_SDLGP_PLATFORMER_HPP_ */
