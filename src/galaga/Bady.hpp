#ifndef BADY_HPP
#define BADY_HPP

#include "GameObject.hpp"
#include "Constants.hpp"
#include <vector>
#include <utility>

/**
 * A bady enemy object in the galaga game
 */
class Bady : public GameObject {

public:
	Bady();

	Bady(Vector3D pos, Vector3D dim, bool m, std::string spritesheetFile, SDL_Renderer* gRenderer);

	//Used for testing
	Bady(Vector3D pos, Vector3D dim);

	~Bady();

	/**
	 * Updates the enemy/"bady" position
	 */
	void update() override;

	/**
	 * Returns true if a bullet has just been fired
	 *
	 * @return bool has a bullet just been fired?
	 */
	bool hasFired();

	/**
	 * Takes in a collision and tag of the colliding object to fire the desired event within this game object.
	 *
	 * Also returns a boolean value that determines if any additional action needs to be taken in the level
	 *
	 * @return bool determines if any additional action needs to be taken in the level
	 */
	bool collisionUpdate(GameObject::SIDE collisionDirection, int otherTag);

	//adds locatiosn for enemies to travel to
	void populateLocations();

	void incrementMoveDestination();

	void refreshMoveDestination();

private:
	//Frames for cooldown of firing
	float cooldown = 180;
	float sinceLastShot = 0;
	bool bulletFired = false;

	///variables to handle movement
	bool mobile;
	bool mForward;
	int mStep;
	int mSteps;
	unsigned int locationIndex;

	float m0x;
	float m0y;

	std::vector<std::pair<int, int>> ds;
	int speed = 1;
};



#endif //BADY_HPP
