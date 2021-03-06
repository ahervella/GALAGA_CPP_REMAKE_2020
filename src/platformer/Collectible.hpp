#ifndef COLLECTIBLE_HPP
#define COLLECTIBLE_HPP

#include "GameObject.hpp"
#include "../base/Constants.hpp"

/**
 * A collectible object in the Platformer game
 */
class Collectible : public GameObject {

public:
	Collectible();

	Collectible(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer);

	//Used for testing
	Collectible(Vector3D pos, Vector3D dim);

	~Collectible();
};



#endif //COLLECTIBLE_HPP
