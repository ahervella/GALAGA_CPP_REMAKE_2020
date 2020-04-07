#include "PlayerShip.cpp"
#include "Constants.hpp"

PlayerShip::PlayerShip() {}

PlayerShip::PlayerShip(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer) :
			GameObject(pos, dim, Constants::Galaga::Game::Tag::PLAYER_TAG, spritesheetFile, gRenderer)
{
	//this->shootSFXFileName = Constants::Galaga::SFXPath::
	//this->loseSFXFileName = Constants::Galaga::SFXPath::
}

PlayerShip::PlayerShip(Vector3D pos, Vector3D dim) :
		GameObject(pos, dim, Constants::Galaga::Game::Tag::PLAYER_TAG)
{
	//this->shootSFXFileName = Constants::Galaga::SFXPath::

	//this->loseSFXFileName = Constants::Galaga::SFXPath::
}
