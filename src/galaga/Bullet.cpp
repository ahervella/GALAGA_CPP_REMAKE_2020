#include "Bullet.hpp"
#include "Constants.hpp"

Bullet::Bullet() : GameObject() {}

Bullet::Bullet(Vector3D pos, Vector3D dim, int ownerTag, std::string spritesheetFile, SDL_Renderer* gRenderer) :
		GameObject(pos, dim, ownerTag, spritesheetFile, gRenderer)
{
	//Shooting down from enemy
	if(ownerTag == Constants::Galaga::Game::Tag::BADY_BULLET_TAG) {
		speed = 5;
	}
	//Shooting up from player
	if(ownerTag == Constants::Galaga::Game::Tag::PLAYER_BULLET_TAG) {
		speed = -5;
	}
}

void Bullet::update() {
	pos.y += speed;
}

void Bullet::render(SDL_Renderer* gRenderer) {
	int w;
	int h;
	SDL_QueryTexture(*spritesheet, NULL, NULL, &w, &h);

	Src.x = 0;
	Src.y = 0;

	Src.w = w;
	Src.h = h;

	Dest.x = pos.x;
	Dest.y = pos.y;
	Dest.w = dim.x;
	Dest.h = dim.y;

	SDL_RenderCopy(gRenderer, *spritesheet, &Src, &Dest);


}


