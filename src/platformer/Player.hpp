#ifndef PLAYER_HPP
#define PLAYER_HPP



#include "GameObject.hpp"
#include "../base/Constants.hpp"

/**
 * Class that represents the player of the Platformer game
 */
class Player : public GameObject{

public:
    static int lifeCount;

    Player();

    Player(Vector3D pos, Vector3D dim, std::string spritesheetFile, SDL_Renderer* gRenderer);

    //Used for testing
    Player(Vector3D pos, Vector3D dim);

    ~Player();

    /**
     * Plays the SFX file attached to the Player for jumping
     */
    void playJumpSFXFile();

    /**
     * Play the SFX file attached to the Player for collecting
     */
    void playCollectSFXFile();

    /**
     * Play the SFX file attached to the Player for hitting the goal
     */
    void playGoalSFXFile();

    /**
     * Play the SFX file attached to the Player for losing the game (i.e. hitting an enemy)
     */
    void playLoseSFXFile();

    /**
     * Renders the Player with the given renderer, using the states of the player to decide which sprites/animations to play
     *
     * @param gRenderer: the renderer
     */
    void render(SDL_Renderer *gRenderer) override;

    void move();

    /**
     * Updates the states of the Player that do not require collision detection
     */
    void update() override;

    /**
     * Uses a direction from which the player collides with an object to make a state update on the player
     *
     * @param collisionDirection: The direction from which a collision on the player is detected
     */
    bool collisionUpdate(GameObject::SIDE collisionDirection, int otherTag);

    /**
     * Returns a bool that represents whether the Player is in the process of a jump.
     */
    bool isJumping();

    /**
     * Returns a bool that represents whether the Player is facing left
     */
    bool isFacingLeft();

    /**
     * Returns a bool that represents whether a player is moving
     */
    bool isMoving();

    /**
     * Sets jumping to true
     */
    void jump();

    /**
     * Returns the player's score
     */
    int getScore();

    bool justJumped = false;


private:
    std::shared_ptr<Mix_Chunk*> jumpSFX;
    std::string jumpSFXFileName;

    std::shared_ptr<Mix_Chunk*> collectSFX;
    std::string collectSFXFileName;

    std::shared_ptr<Mix_Chunk*> goalSFX;
    std::string goalSFXFileName;

    std::shared_ptr<Mix_Chunk*> loseSFX;
    std::string loseSFXFileName;

    int score = 0;

    SDL_Rect Src;
    SDL_Rect Dest;
    //Which frame we are in a running cycle
    int currRunningFrame{0};
    //How many frames are in the running animation
    int lastFrame{3};
    //Are we facing left? Helps maintain direction when stopped (don't automatically look right when stopped)
    //Start looking towards the right (where the goal will be) as default
    bool facingLeft = false;
    //Are we in the process of a jump? Not when we start, of course!
    bool jumping = false;
    //Are we moving? Not when we start, of course!
    bool moving = false;
};



#endif //PLAYER_HPP
