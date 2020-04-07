#ifndef BULLET_HPP
#define BULLET_HPP

#include "GameObject.hpp"

class Bullet : public GameObject {
public:
	Bullet();

	Bullet(Vector3D pos, Vector3D dim, int ownerTag, std::string spritesheetFile, SDL_Renderer* gRenderer);

	void update() override;

	void render(SDL_Renderer* gRenderer) override;

private:
	//Vertical speed
	float speed = 5;

	SDL_Rect Src;
	SDL_Rect Dest;
};

#endif
