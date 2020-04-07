/*
 * SDLGP_Galaga.hpp
 *
 *  Created on: Apr 6, 2020
 *      Author: Karina
 */

#ifndef SRC_GALAGA_SDLGP_GALAGA_HPP_
#define SRC_GALAGA_SDLGP_GALAGA_HPP_

#include "SDLGraphicsProgram.hpp"

class SDLGP_Galaga : SDLGraphicsProgram
{
public:

	SDLGP_Galaga();
	~SDLGP_Galaga();

	void update();
	void render();
	void loop();

	void gameOver();
	void loseLife();

	void initLevel();

	void resetToLevel();

    void makePlayerShip();

    void changeLanguage(int langIndex);

    void resetShipPosition();

private:
	std::vector<GalagaLevel> GalagaLevels;

};



#endif /* SRC_GALAGA_SDLGP_GALAGA_HPP_ */
