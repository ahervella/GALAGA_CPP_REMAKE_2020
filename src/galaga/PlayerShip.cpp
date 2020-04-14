#include "PlayerShip.hpp"
#include "Constants.hpp"
//#include "ResourceManager.hpp"


int PlayerShip::lifeCount = Constants::Galaga::Game::LIVES;

PlayerShip::PlayerShip() {}

PlayerShip::PlayerShip(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer) :
			GameObject(pos, dim, Constants::Galaga::Game::Tag::PLAYER_TAG, spritesheetFile, gRenderer)
{
	this->hitSFXFileName = Constants::Galaga::SFXPath::LOSELIFE;
	this->hitSFX = ResourceManager::getInstance()->getSFXResource(hitSFXFileName);

	this->loseSFXFileName = Constants::Galaga::SFXPath::LOSE;
	this->loseSFX = ResourceManager::getInstance()->getSFXResource(loseSFXFileName);
}

PlayerShip::PlayerShip(Vector3D pos, Vector3D dim) :
		GameObject(pos, dim, Constants::Galaga::Game::Tag::PLAYER_TAG)
{

	this->loseSFXFileName = Constants::Galaga::SFXPath::LOSE;
	this->loseSFX = ResourceManager::getInstance()->getSFXResource(loseSFXFileName);
}

PlayerShip::~PlayerShip() {
	if(hitSFX) {
		hitSFX.reset();
		ResourceManager::getInstance()->deleteSFXResource(hitSFXFileName);
	}
	if(loseSFX) {
	    loseSFX.reset();
	    ResourceManager::getInstance()->deleteSFXResource(loseSFXFileName);
	}
	//Not necessary, GameObject destructor does this for us
	/*
	if (spritesheet) {
	    spritesheet.reset();
	    ResourceManager::getInstance()->deleteTextureResource(spritesheetFileName);
	}
	*/
}
void PlayerShip::move()
{
	pos.x += speed * direction;
}

void PlayerShip::changeDirection(int dir)
{
	if(dir < 0) {
		direction = -1;
	}
	else if(dir > 0) {
		direction = 1;
	}
	else {
		direction = 0;
	}

}

void PlayerShip::update() {
	if(gameOver) {
		return;
	}
	if(bulletFired) {
		bulletFired = false;
	}
	//Add one to sinceLastShot for each frame we're updating in
	sinceLastShot += 1;
}
/*
void PlayerShip::render(SDL_Renderer* gRenderer) {


	int w;
	int h;
	SDL_QueryTexture(*spritesheet, NULL, NULL, &w, &h);

	Src.w = w;
	Src.h = h;
	Src.x = 0;
	Src.y = 0;

	Dest.x = pos.x;
	Dest.y = pos.y;
	Dest.w = dim.x;
	Dest.h = dim.y;

	SDL_RenderCopy(gRenderer, *spritesheet, &Src, &Dest);

}
*/

void PlayerShip::shoot()
{
	if(sinceLastShot >= cooldown) {
		//Fire bullet, reset sinceLastShot
		sinceLastShot = 0;
		bulletFired = true;
	}
}

void PlayerShip::playLoseSFXFile() {
	Mix_PlayChannel(-1, *loseSFX, 0);
}

void PlayerShip::playHitSFXFile() {
	Mix_PlayChannel(-1, *hitSFX, 0);
}

bool PlayerShip::hasFired() {
	return bulletFired;
}

int PlayerShip::getScore() {
	return score;
}

void PlayerShip::setScore(int s) {
	score = s;
}

bool PlayerShip::collisionUpdate(GameObject::SIDE collisionDirection, int otherTag) {
	switch(otherTag) {
			case Constants::Galaga::Game::Tag::BADY_BULLET_TAG:
				if(collisionDirection != GameObject::SIDE::NONE) {
					//TODO: Lose a life (handled by level)
					playLoseSFXFile();
					return true;
				}
	}
	return false;
}

void PlayerShip::resetPosition(Vector3D pos, Vector3D dim){
    this->pos = pos;
    this->dim = dim;
}
