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

	this->shootSFXFileName = Constants::Galaga::SFXPath::SHOOT;
	this->shootSFX = ResourceManager::getInstance()->getSFXResource(shootSFXFileName);

	this->playShootSFXFile();
}

Bullet::~Bullet()
{
	if(shootSFX) {
		shootSFX.reset();
        ResourceManager::getInstance()->deleteSFXResource(shootSFXFileName);
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


	Dest.w = dim.x / 0.2;
	Dest.h = dim.y / 0.2;
    Dest.x = pos.x - (Dest.w - dim.x)/2;
    Dest.y = pos.y - (Dest.h - dim.y)/2;

	SDL_RenderCopy(gRenderer, *spritesheet, &Src, &Dest);


}

bool Bullet::collisionUpdate(GameObject::SIDE collision, int otherTag) {


    if(collision != GameObject::SIDE::NONE) {

        switch(otherTag){
            case Constants::Galaga::Game::Tag::BADY_TAG:
                if (this->tag == Constants::Galaga::Game::Tag::PLAYER_BULLET_TAG){
                    return true;
                }
                break;

            case Constants::Galaga::Game::Tag::PLAYER_TAG:
                if (this->tag == Constants::Galaga::Game::Tag::BADY_BULLET_TAG){
                    return true;
                }

        }

    }

    return false;
}

void Bullet::playShootSFXFile() {
	Mix_PlayChannel(-1, *shootSFX, 0);
}


