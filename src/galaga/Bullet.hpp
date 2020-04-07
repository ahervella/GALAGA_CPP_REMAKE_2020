#ifndef BULLET_HPP
#define BULLET_HPP

class Bullet : public GameObject {
	Bullet();

	Bullet(Vector3D pos, Vector3D dim, int ownerTag, std::string spritesheetFile, SDL_Renderer* gRenderer);
};

#endif
