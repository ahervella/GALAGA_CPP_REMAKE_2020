#ifndef GOAL_HPP
#define GOAL_HPP

#include "GameObject.hpp"
#include "../base/Constants.hpp"

/**
 * A goal in the platformer game. If the player collects this, they win!
 */
class Goal : public GameObject {

public:
	Goal();

	Goal(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer);

	//Used for testing
	Goal(Vector3D pos, Vector3D dim);

	~Goal();
};



#endif //GOAL_HPP
