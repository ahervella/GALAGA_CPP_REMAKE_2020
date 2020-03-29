#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "GameObject.hpp"
#include "Constants.hpp"

//A collectible object
class Enemy : public GameObject {

public:
	Enemy();

	Enemy(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer);

	~Enemy();

	void update() override;

	void move();

	void render(SDL_Renderer *gRenderer) override;

	void collisionUpdate(GameObject::SIDE collisionDirection, int otherTag);

private:

	SDL_Rect Src;
	SDL_Rect Dest;
	bool mForward {true};
	//bool firstFrame {true};
	int speed {2};
	int currRunningFrame {0};
	int lastFrame{3};
	int steps {30};
	int currStep {0};
};

#endif //ENEMY_HPP
