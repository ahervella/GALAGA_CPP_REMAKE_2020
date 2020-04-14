#include "Bady.hpp"
#include <math.h>
#include <ctime>

Bady::Bady() : GameObject() {

    randomizeShootStartTime();

}

Bady::Bady(Vector3D pos, Vector3D dim, bool m, std::string spritesheetFile, SDL_Renderer* gRenderer) :
		GameObject(pos, dim, Constants::Galaga::Game::Tag::BADY_TAG, spritesheetFile, gRenderer)
{
	mobile = m;
	populateLocations();
	if (m)
	{
		mForward = true;
		locationIndex = 0;
		mStep = 0;

		//set the first m0, and mSteps with the current locationIndex (0)
        refreshMoveDestination();
	}

    randomizeShootStartTime();

    hitSFXFileName = Constants::Galaga::SFXPath::ENEMYHIT;
    hitSFX = ResourceManager::getInstance()->getSFXResource(hitSFXFileName);

}

Bady::Bady(Vector3D pos, Vector3D dim) :
		GameObject(pos, dim, Constants::Galaga::Game::Tag::BADY_TAG)
{
	mobile = false;
    randomizeShootStartTime();
    hitSFXFileName = Constants::Galaga::SFXPath::ENEMYHIT;
    hitSFX = ResourceManager::getInstance()->getSFXResource(hitSFXFileName);
}

Bady::~Bady()
{
	if(hitSFX) {
		hitSFX.reset();
		ResourceManager::getInstance()->deleteSFXResource(hitSFXFileName);
	}
}

void Bady::playHitSFX() {
	Mix_PlayChannel(-1, *hitSFX, 0);
}

void Bady::randomizeShootStartTime(){

    sinceLastShot = (int) std::rand() % (int)cooldown;
    //std::cout<<"sinceLastShottttttttttttt: "<<sinceLastShot<<std::endl;
}

void Bady::update() {

    //if moving
	if (mobile)
	{
	    //update position
		pos.x = (pos.x + m0x * (mForward? 1:-1));
		pos.y = (pos.y + m0y * (mForward? 1:-1));

		//increase step
		mStep++;

		//check for when ship has finished this destination
		if (mStep >= mSteps){
            incrementMoveDestination();
		}


	}


	//TODO: Make enemy more interesting
	if(!bulletFired) {

		sinceLastShot += 1;

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

void Bady::populateLocations()
{
	auto loc = std::make_pair<int, int>(100, 100);
	auto loc2 = std::make_pair<int, int>(300, 100);
	auto loc3 = std::make_pair<int, int>(0, 0);

	ds.push_back(loc);
	ds.push_back(loc2);
	ds.push_back(loc3);
}



void Bady::incrementMoveDestination(){
    mStep = 0;
    locationIndex++;

    refreshMoveDestination();


    //check to see if all destinations have been done and should loop again
    if (locationIndex + 1>= ds.size())
    {
        locationIndex = 0;
    }
}



void Bady::refreshMoveDestination(){
    m0x = ds[locationIndex].first - pos.x;
    m0y = ds[locationIndex].second - pos.y;

    float len = sqrtf(m0x * m0x + m0y * m0y);
    m0x = m0x / len * speed;
    m0y = m0y / len * speed;

    mSteps = len / speed;
}
