#ifndef BULLET_HPP
#define BULLET_HPP

#include "GameObject.hpp"

class Bullet : public GameObject {
public:
	Bullet();

	Bullet(Vector3D pos, Vector3D dim, int ownerTag, std::string spritesheetFile, SDL_Renderer* gRenderer);

	~Bullet();

	/**
	 * Updates the bullet's position each frame
	 */
	void update() override;

	/**
	 * Renders the bullet on screen each frame
	 */
	void render(SDL_Renderer* gRenderer) override;

	/**
	 * Checks if the bullet has collided with another object. Returns true if the level needs to take a certain action
	 *
	 * @return bool that determines if a collision between the bullet and the other object occurs and needs the level to take an action
	 */
    bool collisionUpdate(GameObject::SIDE collision, int otherTag);

    /**
     * Plays the sound effect for shooting
     */
    void playShootSFXFile();

private:
    //Sound for when the bullet fires
    std::shared_ptr<Mix_Chunk*> shootSFX;
    std::string shootSFXFileName;

	//Vertical speed
	float speed = 5;

	SDL_Rect Src;
	SDL_Rect Dest;
};

#endif
