#ifndef PLAYER_SHIP_H
#define PLAYER_SHIP_H

#include "GameObject.hpp"

class PlayerShip : public GameObject
{
public:
	//The amount of lives the player ship has
	static int lifeCount;

	PlayerShip();

	PlayerShip(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer);

	//Used for testing
	PlayerShip(Vector3D pos, Vector3D dim);

	~PlayerShip();

	/**
	 * Shoots a bullet that can damage the enemy
	 */
	void shoot();

	/**
	 * Moves the player horizontally
	 */
	void move();

	//Update the PlayerShip
	void update() override;

	//Render the PlayerShip
	void render(SDL_Renderer* gRenderer) override;

	/**
	 * Switches the direction that the player will move when calling move(). 0 for neutral, 1 for right, and -1 for left
	 */
	void changeDirection(int direction);

	/**
	 * Returns the player's score
	 */
	int getScore();

	/**
 	 * Uses a direction from which the player collides with an object to make a state update on the player
     *
     * @param collisionDirection: The direction from which a collision on the player is detected
     * @param otherTag: the tag of the other object
     */
	bool collisionUpdate(GameObject::SIDE collisionDirection, int otherTag);



private:
	//Player's score
	int score = 0;
	//Speed of the player
	int speed = 3;
	//The direction the player will move when calling move (0 = neutral, 1 = right, -1 = left)
	int direction;
	//Shooting sound effect and filename for resource management
	std::shared_ptr<Mix_Chunk*> shootSFX;
	std::string shootSFXFileName;

	//Losing sound effect and filename for resource management
	std::shared_ptr<Mix_Chunk*> loseSFX;
	std::string loseSFXFileName;

	//A cooldown for shooting (set in frames)
	int cooldown = 120;
	//Keeping track of the number of frames since the last shot
	int sinceLastShot = 120;

	SDL_Rect Src;
	SDL_Rect Dest;

};

#endif
