#include <cxxtest/TestSuite.h>
#include "GameObject.hpp"
#include "Collectible.hpp"
#include "Enemy.hpp"
#include "Goal.hpp"
#include "Player.hpp"
#include "util.hpp"
#include "TinyMath.hpp"
#include "ResourceManager.hpp"
#include "Constants.hpp"

class PlatformerGameObjectTestSuite: public CxxTest::TestSuite
{
public:
	Player p;
	Collectible c1;
	Collectible c2;
	Enemy e;
	Goal g;
	Vector3D playerStart = Vector3D(100, 100);
	Vector3D c1Start = Vector3D(140, 100);
	Vector3D c2Start = Vector3D(60, 100);
	Vector3D enemyStart = Vector3D(200, 100);
	Vector3D goalStart = Vector3D(150, 100);
	Vector3D playerDim = Constants::Platformer::Game::PLAYER_UNIT_DIM * Constants::Platformer::Game::UNIT;
	Vector3D blockDim = Constants::Platformer::Game::BLOCK_UNIT_DIM * Constants::Platformer::Game::UNIT;
	Vector3D goalDim = Constants::Platformer::Game::GOAL_UNIT_DIM * Constants::Platformer::Game::UNIT;
	Vector3D enemyDim = Constants::Platformer::Game::ENEMY_UNIT_DIM * Constants::Platformer::Game::UNIT;
	Vector3D collectibleDim = Constants::Platformer::Game::COLLECTIBLE_UNIT_DIM * Constants::Platformer::Game::UNIT;


	PlatformerGameObjectTestSuite() {
		p = Player(playerStart, playerDim);
		c1 = Collectible(c1Start, collectibleDim);
		c2 = Collectible(c2Start, collectibleDim);
		e = Enemy(enemyStart, enemyDim);
		g = Goal(goalStart, goalDim);
	}

	void reset() {
		GameObject::gameOver = true;
		p = Player(playerStart, playerDim);
		c1 = Collectible(c1Start, collectibleDim);
		c2 = Collectible(c2Start, collectibleDim);
		e = Enemy(enemyStart, enemyDim);
		g = Goal(goalStart, goalDim);
	}

	//Tests player and some collisions with other GameObjects
	void testPlayer() {
		reset();

		//Set velocity
		p.vel.x = 2;

		//gameOver is true, so nothing happens here
		p.update();
		TS_ASSERT(!p.isMoving());
		p.move();
		TS_ASSERT_EQUALS(p.pos.x, playerStart.x);

		//Now the game is active, things will move, etc.
		GameObject::gameOver = false;

		//Will update our moving boolean
		p.update();
		TS_ASSERT(p.isMoving());
		TS_ASSERT(!p.isFacingLeft());

		//Actually move the player
		p.move();
		TS_ASSERT_EQUALS(p.pos.x, 102);
		TS_ASSERT_EQUALS(p.pos.y, 100);

		//Now jump!
		p.vel.y = 2;

		//I would check if jump is true after update, but we manually set that when we first press the up key like so
		p.jump();
		p.justJumped = true;

		p.update();
		TS_ASSERT(p.isJumping());

		p.move();
		TS_ASSERT_EQUALS(p.pos.x, 104);
		TS_ASSERT_EQUALS(p.pos.y, 102);

		//SDLGraphicsProgram handles this simulation of "just jumping", so I'm taking its place here
		p.justJumped = false;
		//Send a collision as if the player hit the bottom of a block (or the player's top hit a block)
		p.collisionUpdate(GameObject::SIDE::TOP, Constants::Platformer::Game::Tag::BLOCK_TAG);
		TS_ASSERT_EQUALS(p.vel.y, 0);

		//Used to show that next test will zero out y velocity
		p.vel.y = 999;

		//Simulate landing on the ground (bottom of player hits a block). We are no longer jumping.
		p.collisionUpdate(GameObject::SIDE::BOTTOM, Constants::Platformer::Game::Tag::BLOCK_TAG);
		TS_ASSERT(!p.isJumping());
		TS_ASSERT_EQUALS(p.vel.y, 0);

		//Now hit the wall on the player's right side. Vel.x should be zero
		p.collisionUpdate(GameObject::SIDE::RIGHT, Constants::Platformer::Game::Tag::BLOCK_TAG);
		TS_ASSERT_EQUALS(p.vel.x, 0);
		p.update();
		TS_ASSERT(!p.isMoving());

		//Now the other way!
		p.vel.x = -2;
		p.update();
		TS_ASSERT(p.isFacingLeft());
		TS_ASSERT(p.isMoving());
		p.move();
		TS_ASSERT_EQUALS(p.pos.x, 102);
		TS_ASSERT_EQUALS(p.pos.y, 102);

		//Hit wall on player's left side
		p.collisionUpdate(GameObject::SIDE::LEFT, Constants::Platformer::Game::Tag::BLOCK_TAG);
		TS_ASSERT_EQUALS(p.vel.x, 0);
		p.update();
		TS_ASSERT(!p.isMoving());
		//Still facing left since that was our last facing direction
		TS_ASSERT(p.isFacingLeft());

		//Collide with collectible and make sure score is updated properly
		TS_ASSERT_EQUALS(p.getScore(), 0);
		p.collisionUpdate(GameObject::SIDE::LEFT, Constants::Platformer::Game::Tag::COLLECTIBLE_TAG);
		TS_ASSERT_EQUALS(p.getScore(), 1);
		p.collisionUpdate(GameObject::SIDE::RIGHT, Constants::Platformer::Game::Tag::COLLECTIBLE_TAG);
		TS_ASSERT_EQUALS(p.getScore(), 2);
		p.collisionUpdate(GameObject::SIDE::TOP, Constants::Platformer::Game::Tag::COLLECTIBLE_TAG);
		TS_ASSERT_EQUALS(p.getScore(), 3);
		p.collisionUpdate(GameObject::SIDE::BOTTOM, Constants::Platformer::Game::Tag::COLLECTIBLE_TAG);
		TS_ASSERT_EQUALS(p.getScore(), 4);
		//This will not increase the score
		p.collisionUpdate(GameObject::SIDE::NONE, Constants::Platformer::Game::Tag::COLLECTIBLE_TAG);
		TS_ASSERT_EQUALS(p.getScore(), 4);

		//Now actually make player check valid collision with a collectible
		p.pos = c2.pos;
		//Player needs a velocity to count a collision
		p.vel.x = 1;
		p.vel.y = 1;
		GameObject::SIDE s = p.isColliding(c2);
		std::cout << "Side: " << s << std::endl;
		TS_ASSERT_DIFFERS(s, GameObject::SIDE::NONE);
		p.collisionUpdate(s, Constants::Platformer::Game::Tag::COLLECTIBLE_TAG);
		TS_ASSERT_EQUALS(p.getScore(), 5);
	}


};
