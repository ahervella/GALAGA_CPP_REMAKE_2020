#include "Bady.hpp"

Bady::Bady() : GameObject() {}

Bady::Bady(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer) :
		GameObject(pos, dim, Constants::Galaga::Game::Tag::BADY_TAG, spritesheetFile, gRenderer)
{
}

Bady::Bady(Vector3D pos, Vector3D dim) :
		GameObject(pos, dim, Constants::Galaga::Game::Tag::BADY_TAG)
{
}

Bady::~Bady()
{
}

void Bady::update() {
	//TODO: Make enemy more interesting
	if(!bulletFired) {

		//sinceLastShot += 1;

		if(sinceLastShot >= cooldown) {
			sinceLastShot = 0;
			bulletFired = true;
			return;
		}
	}
	else {
		bulletFired = false;
	}
}

bool Bady::hasFired() {
	return bulletFired;
}

bool Bady::collisionUpdate(GameObject::SIDE collision, int otherTag) {
	switch(otherTag) {
			case Constants::Galaga::Game::Tag::PLAYER_BULLET_TAG:
				if(collision != GameObject::SIDE::NONE) {
					//TODO: Eliminate enemy
					return true;
				}
	}
	return false;
}
