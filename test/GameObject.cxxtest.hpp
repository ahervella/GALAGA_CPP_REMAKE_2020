#include <cxxtest/TestSuite.h>
#include "GameObject.hpp"
#include "util.hpp"
#include "ResourceManager.hpp"

class GameObjectTestSuite: public CxxTest::TestSuite
{
public:
	Ball ball;
    Paddle p;

    GameObjectTestSuite() {
        //ball.init(getResourcePath() + "459145__mattix__retro-pew-shot-01.wav", getResourcePath() + "219619__ani-music__pew-pew-sound-effect-peww1.wav");
        ball.resetBall();
    }

    void resetBallAndPaddle() {
    	GameObject::gameOver = true;
    	ball.resetBall();
    	p.init(100, 100, 100, 100, {0, 0, 0, 0});
    }

    //Play the sounds to make sure the ball was initialized with them correctly
    void testBallInit() {

    	Ball ball2;
    	ball2.init(getResourcePath() + "459145__mattix__retro-pew-shot-01.wav", getResourcePath() + "219619__ani-music__pew-pew-sound-effect-peww1.wav");
    	//Each SFX resource should now have a use count of 2 from the ResourceManager and from ball2
    	//(3 after we get a new shared_ptr to it from the resource manager)
    	std::shared_ptr<Mix_Chunk*> sfx1 = ResourceManager::getInstance()->getSFXResource(getResourcePath() + "459145__mattix__retro-pew-shot-01.wav");
    	TS_ASSERT_EQUALS(sfx1.use_count(), 3);
    	std::shared_ptr<Mix_Chunk*> sfx2 = ResourceManager::getInstance()->getSFXResource(getResourcePath() + "219619__ani-music__pew-pew-sound-effect-peww1.wav");
    	TS_ASSERT_EQUALS(sfx2.use_count(), 3);
    	//Resetting so we don't run into errors with other tests that rely on shared_ptrs from ResourceManager
    	sfx1.reset();
    	sfx2.reset();
    }

    void testBallUpdateAndReset() {
    	resetBallAndPaddle();

    	//Angle starts at 2.0f radians
    	//Speed starts at 5
    	//Start position is (250,250) (SCREEN_WIDTH/2, SCREEN_HEIGHT/2)
    	//gameOver is true initially, so this shouldn't update
    	float startX = ball.x;
    	float startY = ball.y;
    	ball.update();
    	TS_ASSERT_EQUALS(ball.x, startX);
    	TS_ASSERT_EQUALS(ball.y, startY);
    	GameObject::gameOver = false;
    	ball.update();
    	TS_ASSERT_DELTA(ball.x, 247.919, 0.001);
    	TS_ASSERT_DELTA(ball.y, 254.546, 0.001);
    	ball.x = startX;
    	ball.y = startY;
    	ball.angle = -2.0f;
    	ball.update();
    	TS_ASSERT_DELTA(ball.x, 247.919, 0.001);
    	TS_ASSERT_DELTA(ball.y, 245.454, 0.001);
    }

    void testPaddleUpdate() {
    	resetBallAndPaddle();
    	//TODO: Test paddle update
    	//Testing that paddle doesn't move when game is over
    	GameObject::gameOver = true;
    	p.update();
    	TS_ASSERT_EQUALS(p.x, 100);
    	TS_ASSERT_EQUALS(p.y, 100);
    	GameObject::gameOver = false;



    }
};
