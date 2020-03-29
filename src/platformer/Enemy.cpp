#include "Enemy.hpp"

Enemy::Enemy() : GameObject() {}

Enemy::Enemy(Vector3D pos, Vector3D dim, std::string spritesheetFileName, SDL_Renderer* gRenderer) :
		GameObject(pos, dim, Constants::Game::Tag::ENEMY_TAG, spritesheetFileName, gRenderer)
{

}

Enemy::~Enemy()
{

}

void Enemy::update() {
	/*
	std::cout << "vel.x: " << vel.x << std::endl;
	if(vel.x == 0 && !firstFrame) {
		std::cout << "Collision detected!" << std::endl;
		mForward = !mForward;
	}

	if(mForward) {
		vel.x = speed;
		std::cout << "vel.x after going forward: " << vel.x << std::endl;
	}

	else {
		vel.x = -1 * speed;
		std::cout << "vel.x after going backwards: " << vel.x << std::endl;
	}

	currRunningFrame++;
	if(currRunningFrame >= lastFrame) {
	    currRunningFrame = 0;
	}

	firstFrame = false;
	*/
	if(currStep > steps) {
		mForward = !mForward;
		currStep = 0;
	}

	if(mForward) {
		vel.x = speed;
	}
	else {
		vel.x = -1 * speed;
	}

	currStep++;
}

void Enemy::move() {
	pos += vel;
}

void Enemy::render(SDL_Renderer *gRenderer) {
	int w;
	int h;
	SDL_QueryTexture(*spritesheet, NULL, NULL, &w, &h);
	//Always displaying frames 0-2 (so start from dim.x * frameNum)
	Src.x = (w / lastFrame) * currRunningFrame;
	Src.y = h / 9;

	Src.w = w / lastFrame;
	Src.h = h / 9;

	Dest.x = pos.x;
	Dest.y = pos.y;
	Dest.w = dim.x;
	Dest.h = dim.y;

	//Facing right --> default direction of sprite
	if(mForward) {
		SDL_RenderCopy(gRenderer, *spritesheet, &Src, &Dest);
	}
	//Facing left --> flipped sprite direction (flip along y-axis)
	//0.0 is the angle we want to rotate the sprite (we don't rotate)
	//NULL is center of rotation (NULL sets it to default -- center of image), but we don't rotate anyways
	//SDL_FLIP_HORIZONTAL: flip horizontally
	else {
		SDL_RenderCopyEx(gRenderer, *spritesheet, &Src, &Dest, 0.0, NULL, SDL_FLIP_HORIZONTAL);
	}
}

void Enemy::collisionUpdate(GameObject::SIDE collisionDirection, int otherTag) {
	switch(otherTag) {
		case Constants::Game::Tag::BLOCK_TAG:
			switch (collisionDirection)
				{
					case NONE:
					    break;
					case TOP:
						break;
					case BOTTOM:
						std::cout << "ENEMY BOTTOM" << std::endl;
						vel.y = 0;
						break;
					case LEFT:
				        std::cout<<"ENEMY LEFT"<<std::endl;
				        //TODO: Stop velocity when going left
						//I think this should work...
						if(vel.x < 0) {
							vel.x = 0;
						}
						break;
					case RIGHT:
				        std::cout<<"ENEMY RIGHT"<<std::endl;
				        //TODO: Stop velocity when going right
						//I think this should work...
						if(vel.x > 0) {
							vel.x = 0;
						}
						break;
				}
				break;
	}
}
