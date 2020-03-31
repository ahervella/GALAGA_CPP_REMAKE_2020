#ifndef GOAL_HPP
#define GOAL_HPP

#include "GameObject.hpp"
#include "Constants.hpp"

//A collectible object
class Goal : public GameObject {

public:
	Goal();

	Goal(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer);

	//Used for testing
	Goal(Vector3D pos, Vector3D dim);

	~Goal();
};



#endif //GOAL_HPP
