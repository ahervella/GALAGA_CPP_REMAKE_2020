#include "Goal.hpp"

Goal::Goal() : GameObject()
{

}

Goal::Goal(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer) :
		GameObject(pos, dim, Constants::Platformer::Game::Tag::GOAL_TAG, spritesheetFile, gRenderer)
{

}

Goal::~Goal()
{

}



