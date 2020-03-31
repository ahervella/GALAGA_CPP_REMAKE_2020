//
// Created by Alejandro Hervella on 2/27/20.
//

#include "Player.hpp"

int Player::lifeCount = Constants::Platformer::Game::LIVES;

Player::Player() : GameObject() {}

Player::Player(Vector3D pos, Vector3D dim, std::string spritesheetFileName, SDL_Renderer* gRenderer) :
		GameObject(pos, dim, Constants::Platformer::Game::Tag::PLAYER_TAG, spritesheetFileName, gRenderer)
{
	this->jumpSFXFileName = Constants::Platformer::SFXPath::JUMP;
	this->jumpSFX = ResourceManager::getInstance()->getSFXResource(jumpSFXFileName);

	this->collectSFXFileName = Constants::Platformer::SFXPath::COLLECT;
	this->collectSFX = ResourceManager::getInstance()->getSFXResource(collectSFXFileName);

	this->goalSFXFileName = Constants::Platformer::SFXPath::GOAL;
	this->goalSFX = ResourceManager::getInstance()->getSFXResource(goalSFXFileName);

	this->loseSFXFileName = Constants::Platformer::SFXPath::LOSE;
	this->loseSFX = ResourceManager::getInstance()->getSFXResource(loseSFXFileName);
}

Player::Player(Vector3D pos, Vector3D dim) :
		GameObject(pos, dim, Constants::Platformer::Game::Tag::PLAYER_TAG)
{
	this->jumpSFXFileName = Constants::Platformer::SFXPath::JUMP;
	this->jumpSFX = ResourceManager::getInstance()->getSFXResource(jumpSFXFileName);

	this->collectSFXFileName = Constants::Platformer::SFXPath::COLLECT;
	this->collectSFX = ResourceManager::getInstance()->getSFXResource(collectSFXFileName);

	this->goalSFXFileName = Constants::Platformer::SFXPath::GOAL;
	this->goalSFX = ResourceManager::getInstance()->getSFXResource(goalSFXFileName);

	this->loseSFXFileName = Constants::Platformer::SFXPath::LOSE;
	this->loseSFX = ResourceManager::getInstance()->getSFXResource(loseSFXFileName);
}

Player::~Player() {
    if (jumpSFX) {
        jumpSFX.reset();
        ResourceManager::getInstance()->deleteSFXResource(jumpSFXFileName);
    }
    if(collectSFX) {
    	collectSFX.reset();
    	ResourceManager::getInstance()->deleteSFXResource(collectSFXFileName);
    }
    if (goalSFX) {
        goalSFX.reset();
        ResourceManager::getInstance()->deleteSFXResource(goalSFXFileName);
    }
    if(loseSFX) {
        loseSFX.reset();
        ResourceManager::getInstance()->deleteSFXResource(loseSFXFileName);
    }
    if (spritesheet) {
    	spritesheet.reset();
    	ResourceManager::getInstance()->deleteTextureResource(spritesheetFileName);
    }
}

void Player::render(SDL_Renderer *gRenderer) {
	//TODO: Figure out which sprite to show

	int w;
	int h;
	SDL_QueryTexture(*spritesheet, NULL, NULL, &w, &h);
	//Always displaying frames 0-2 (so start from dim.x * frameNum)
	Src.x = (w / lastFrame) * currRunningFrame;

	//If jumping, jump sprite
	if(jumping) {
		//Jumping sprites are on the third row of 7, so starting two heights from the top
		//TODO: Define macros or something that put away these magic numbers
		Src.y = 2 * (h / 9);
	}
	//If not jumping, stand sprite
	else {
		//Walk.run sprites are on the second row of 7, so starting one height from the top
		//TODO: Define macros or something that put away these magic numbers
		Src.y = (h / 9);
	}

	Src.w = w / lastFrame;
	Src.h = h / 9;

	Dest.x = pos.x;
	Dest.y = pos.y;
	Dest.w = dim.x;
	Dest.h = dim.y;

	//Facing right --> default direction of sprite
	if(!facingLeft) {
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

void Player::move(){

    //std::cout<<"move " + std::to_string(vel.y)<<std::endl;

	//An extra safety measure
	if(gameOver) { return; }
    pos += vel;
}

void Player::update(){
    if (gameOver) { return; }

    std::cout << "Velocity: " << vel.x << std::endl;
    //Increment frame. If after last available frame for animation, loop back to beginning
    currRunningFrame++;
    if(currRunningFrame >= lastFrame) {
    	currRunningFrame = 0;
    }

    //If we're moving to the right, we're no longer facing left.
    if(vel.x > 0 && facingLeft) {
    	facingLeft = false;
    	moving = true;
    	currRunningFrame = 0;
    }

    //If we are moving to the left, we best be facing left (otherwise we'd be running backwards, which is highly inefficient)
    if(vel.x < 0 && !facingLeft) {
    	std::cout << "Facing left!" << std::endl;
    	facingLeft = true;
    	moving = true;
    	currRunningFrame = 0;
    }

    //Switch to idle animation
    if(vel.x == 0 && moving) {
    	moving = false;
    	currRunningFrame = 0;
    }

    //Switch to walk/run animation
    if(vel.x != 0 && !moving) {
    	moving = true;
    	currRunningFrame = 0;
    }

    //Anything that relies on collision, such as jumping/landing and running into walls, is handled by SDLGraphicsProgram::update(),
    //which calls Player::collisionUpdate() for each collision

}

void Player::playJumpSFXFile() {
	Mix_PlayChannel(-1, *jumpSFX, 0);
}

void Player::playCollectSFXFile() {
	Mix_PlayChannel(-1, *collectSFX, 0);
}

void Player::playGoalSFXFile() {
	Mix_PlayChannel(-1, *goalSFX, 0);
}

void Player::playLoseSFXFile() {
	Mix_PlayChannel(-1, *loseSFX, 0);
}

bool Player::collisionUpdate(GameObject::SIDE collisionDirection, int otherTag) {
	switch(otherTag) {
		case Constants::Platformer::Game::Tag::BLOCK_TAG:
			switch (collisionDirection)
				{

				    case NONE:
				        break;
					case TOP:
					        std::cout<<"TOP"<<std::endl;
			            prevCollision = GameObject::SIDE::TOP;
						if(jumping){
						vel.y = 0;
						}
						break;
					case BOTTOM:
					        std::cout<<"BOTTOM"<<std::endl;
			            prevCollision = GameObject::SIDE::BOTTOM;

						//If we were previously in a jump and have landed, we are no longer in a jump. Reset the currRunningFrame count
						//so we can reset the animation to the appropriate one when on the ground.
						if(!justJumped) {
							jumping = false;
							currRunningFrame = 0;
						}

			      if (!justJumped && !jumping){
			          vel.y = 0;
			      }
						break;
					case LEFT:
			            prevCollision = GameObject::SIDE::LEFT;
			            std::cout<<"LEFT"<<std::endl;
						//TODO: Stop velocity when going left
						//I think this should work...
						if(vel.x < 0) {
							vel.x = 0;
						}
						break;
					case RIGHT:
			            prevCollision = GameObject::SIDE::RIGHT;
			            std::cout<<"RIGHT"<<std::endl;
						//TODO: Stop velocity when going right
						//I think this should work...
						if(vel.x > 0) {
							vel.x = 0;
						}
				}
			break;
		case Constants::Platformer::Game::Tag::GOAL_TAG:
			if(collisionDirection != SIDE::NONE) {
				//TODO: Play sound (check!), end game, print "You win! Next level!"
				this->playGoalSFXFile();
				return true;
			}
			break;
		case Constants::Platformer::Game::Tag::COLLECTIBLE_TAG:
			if(collisionDirection != SIDE::NONE) {
				//TODO: Play sound (check!), delete collectible, add 1 to score (check!)
				this->playCollectSFXFile();
				score++;
				std::cout << "COLLECTIBLE WITH PLAYER" << std::endl;
				return true;
			}
			break;
		case Constants::Platformer::Game::Tag::ENEMY_TAG:
			if(collisionDirection != SIDE::NONE) {
				//TODO: Play sound (check!), stop game and prints "You lose!"
				this->playLoseSFXFile();
				return true;
			}
			break;
	}
	return false;

}

bool Player::isJumping() {
	return jumping;
}

bool Player::isFacingLeft() {
	return facingLeft;
}

bool Player::isMoving() {
	return moving;
}

void Player::jump() {
	jumping = true;
	this->playJumpSFXFile();
}

int Player::getScore() {
	return score;
}
