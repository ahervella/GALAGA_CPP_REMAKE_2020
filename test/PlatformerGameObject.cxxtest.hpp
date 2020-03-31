#include <cxxtest/TestSuite.h>
#include "GameObject.hpp"
#include "Collectible.hpp"
#include "Enemy.hpp"
#include "Goal.hpp"
#include "Player.hpp"
#include "util.hpp"
#include "ResourceManager.hpp"

class PlatformerGameObjectTestSuite: public CxxTest::TestSuite
{
public:
	Player p;
	Collectible c1;
	Collectible c2;
	Enemy e;
	Goal g;

	PlatformerGameObjectTestSuite() {

	}


};
