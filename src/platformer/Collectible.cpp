#include "Collectible.hpp"

Collectible::Collectible() : GameObject() {}

Collectible::Collectible(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer) :
		GameObject(pos, dim, Constants::Platformer::Game::Tag::COLLECTIBLE_TAG, spritesheetFile, gRenderer)
{
}

Collectible::~Collectible()
{
}
