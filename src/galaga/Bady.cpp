#include "Bady.hpp"

Bady::Bady() : GameObject() {}

Bady::Bady(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer) :
		GameObject(pos, dim, Constants::Galaga::Game::Tag::BADY_TAG, spritesheetFile, gRenderer)
{
}

Bady::Bady(Vector3D pos, Vector3D dim) :
		GameObject(pos, dim, Constants::Platformer::Game::Tag::COLLECTIBLE_TAG)
{
}

Bady::~Bady()
{
}
