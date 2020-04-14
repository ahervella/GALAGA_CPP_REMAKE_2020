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

	/**
	 * adds locations for enemies to travel to
	 */
	void populateLocations();

	/**
	 * Moves on to the next locationIndex and loops if all have been done
	 */
	void incrementMoveDestination();

	/**
	 * Refreshes the move destination, m0, and mSteps with the current locationIndex
	 */
	void refreshMoveDestination();

	/**
	 * Randomizes the time at which an enemy begins to shoot
	 */
	void randomizeShootStartTime();

	/**
	 * Play the soudn effect for getting hit by a bullet
	 */
	void playHitSFX();

private:

	//Sound effect for being hit by a bullet
	std::shared_ptr<Mix_Chunk*> hitSFX;
	std::string hitSFXFileName;

	//Frames for cooldown of firing
	int cooldown = 120;
	int sinceLastShot = 0;
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
