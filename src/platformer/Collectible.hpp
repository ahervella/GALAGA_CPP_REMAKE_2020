#ifndef COLLECTIBLE_HPP
#define COLLECTIBLE_HPP

#include "GameObject.hpp"
#include "Constants.hpp"

//A collectible object
class Collectible : public GameObject {

public:
	Collectible();

	Collectible(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer);

	//Used for testing
	Collectible(Vector3D pos, Vector3D dim);

	~Collectible();
};



#endif //COLLECTIBLE_HPP
