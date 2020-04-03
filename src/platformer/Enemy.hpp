#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "GameObject.hpp"
#include "Constants.hpp"

/**
 * An enemy in the Platformer game
 */
class Enemy : public GameObject {

public:
	Enemy();

	Enemy(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer);

	//Used for testing
	Enemy(Vector3D pos, Vector3D dim);

	~Enemy();

	/**
	 * Updates the enemy position. Will oscillate between two positions in its movement
	 */
	void update() override;

	/**
	 * Moves the enemy
	 */
	void move();

	/**
	 * Renders the enemy
	 */
	void render(SDL_Renderer *gRenderer) override;

	/**
	 * Takes in a collision and tag of the colliding object to fire the desired event
	 */
	void collisionUpdate(GameObject::SIDE collisionDirection, int otherTag);

private:

	SDL_Rect Src;
	SDL_Rect Dest;
	bool mForward {true};
	//bool firstFrame {true};
	int speed {2};
	int currRunningFrame {0};
	int lastFrame{3};
	int steps {30};
	int currStep {0};
};

#endif //ENEMY_HPP
