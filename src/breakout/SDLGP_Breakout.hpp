/*
 * SDLGP_Breakout.hpp
 *
 *  Created on: Apr 6, 2020
 *      Author: Karina
 */

#ifndef SRC_BREAKOUT_SDLGP_BREAKOUT_HPP_
#define SRC_BREAKOUT_SDLGP_BREAKOUT_HPP_

#include "SDLGraphicsProgram.hpp"

class SDLGP_Breakout : SDLGraphicsProgram
{
public:

	SDLGP_Breakout();
	~SDLGP_Breakout();

	void update();
	void render();
	void loop();

	void resetToLevel();
	void checkBrickCollision(float &newAngle, int &collisionID, int &brickCollisionIndex);
	void removeBrick(int rmIndex);
	void updateBallCollisions();

	void gameOver();
	void loseLife();

	void getLanguages();
	void changeLanguage(int langIndex);
	void initLevel();

private:
	std::vector<BreakoutLevel> BreakoutLevels;

};



#endif /* SRC_BREAKOUT_SDLGP_BREAKOUT_HPP_ */
