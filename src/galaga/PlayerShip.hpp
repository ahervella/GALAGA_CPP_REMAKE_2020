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

private:
	int scroe = 0;
	//Shooting sound effect and filename for resource management
	std::shared_ptr<Mix_Chunk*> shootSFX;
	std::string shootSFXFileName;

	//Losing sound effect and filename for resource management
	std::shared_ptr<Mix_Chunk*> loseSFX;
	std::string loseSFXFileName;

	//A cooldown for shooting
	float cooldown;
	//Keeping track of the time since the last shot
	float sinceLastShot;


};
