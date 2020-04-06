#ifndef BADY_HPP
#define BADY_HPP

#include "GameObject.hpp"
#include "Constants.hpp"

/**
 * A bady enemy object in the galaga game
 */
class Bady : public GameObject {

public:
	Bady();

	Bady(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer);

	//Used for testing
	Bady(Vector3D pos, Vector3D dim);

	~Bady();
};



#endif //BADY_HPP
