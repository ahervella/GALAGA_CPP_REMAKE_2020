#include "Bady.hpp"
#include <math.h>

Bady::Bady() : GameObject() {}

Bady::Bady(Vector3D pos, Vector3D dim, bool m, std::string spritesheetFile, SDL_Renderer* gRenderer) :
		GameObject(pos, dim, Constants::Galaga::Game::Tag::BADY_TAG, spritesheetFile, gRenderer)
{
	mobile = m;
	populateLocations();
	if (m)
	{
		mForward = true;
		locationIndex = 1;
		mStep = 0;

		m0x = ds[0].first - pos.x;
		m0y = ds[0].second - pos.y;

		float len = sqrtf(m0x + m0x + m0y + m0y);
		m0x = m0x / len * speed;
		m0y = m0y / len * speed;

		mSteps = len / speed;
	}

}

Bady::Bady(Vector3D pos, Vector3D dim) :
		GameObject(pos, dim, Constants::Galaga::Game::Tag::BADY_TAG)
{
	mobile = false;
}

Bady::~Bady()
{
}

void Bady::update() {

	if (mobile)
	{
		pos.x = (pos.x + m0x * (mForward? 1:-1));
		pos.y = (pos.y + m0y * (mForward? 1:-1));

		mStep++;
		if (mStep >= mSteps && locationIndex < ds.size())
		{
			m0x = ds[locationIndex - 1].first - pos.x;
			m0y = ds[locationIndex - 1].second - pos.y;

			float len = sqrtf(m0x + m0x + m0y + m0y);
			m0x = m0x / len * speed;
			m0y = m0y / len * speed;

			mSteps = len / speed;

			mStep = 0;
			locationIndex++;

			std::cout << "Loop 1" << std::endl;
		}

		else if (mStep >= mSteps && locationIndex >= ds.size())
		{
			m0x = ds[0].first - pos.x;
			m0y = ds[0].second - pos.y;

			float len = sqrtf(m0x + m0x + m0y + m0y);
			m0x = m0x / len * speed;
			m0y = m0y / len * speed;

			mSteps = len / speed;

			mStep = 0;
			locationIndex = 1;

			std::cout << "Loop 2" << std::endl;
		}
	}


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

void Bady::populateLocations()
{
	auto loc = std::make_pair<int, int>(10, 10);
	auto loc2 = std::make_pair<int, int>(20, 15);
	auto loc3 = std::make_pair<int, int>(0, 0);

	ds.push_back(loc);
	ds.push_back(loc2);
	ds.push_back(loc3);
}
