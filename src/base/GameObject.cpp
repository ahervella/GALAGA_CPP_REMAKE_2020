//#include"TinyMath.hpp"

#include "GameObject.hpp"
#include"TinyMath.hpp"
#include "ResourceManager.hpp"
#include <iostream>

bool GameObject::gameOver = false;

GameObject::GameObject(){}

GameObject::GameObject(Vector3D pos, Vector3D dim, int tag, std::string spritesheetFileName, SDL_Renderer* gRenderer) {
    this->pos = pos;
    this->dim = dim;
    this->tag = tag;
    this->fillClr = {0, 0, 0, 0};
    this->lineClr = {0, 0, 0, 0};
    //std::cout << "Resource file: " << spritesheetFileName << std::endl;
    this->spritesheet = ResourceManager::getInstance()->getTextureResource(gRenderer, spritesheetFileName);
    this->spritesheetFileName = spritesheetFileName;
}

GameObject::GameObject(Vector3D pos, Vector3D dim, int tag, SDL_Color fillClr, SDL_Color lineClr, std::string spritesheetFileName, SDL_Renderer* gRenderer) {
    this->pos = pos;
    this->dim = dim;
    this->tag = tag;
    this->fillClr = fillClr;
    this->lineClr = lineClr;
    //std::cout << "Resource file: " << spritesheetFileName << std::endl;
    this->spritesheet = ResourceManager::getInstance()->getTextureResource(gRenderer, spritesheetFileName);
    this->spritesheetFileName = spritesheetFileName;
}

GameObject::~GameObject()
{
	if(spritesheet) {
		spritesheet.reset();
		ResourceManager::getInstance()->deleteTextureResource(spritesheetFileName);
	}
}

    GameObject::SIDE GameObject::isColliding(GameObject obj) {
        float left = this->pos.x;
        float right = this->pos.x + this->dim.x;
        float top = this->pos.y;
        float bottom = this->pos.y + this->dim.y;

        float objLeft = obj.pos.x;
        float objRight = obj.pos.x + obj.dim.x;
        float objTop = obj.pos.y;
        float objBottom = obj.pos.y + obj.dim.y;

        bool rightInRange = right >= objLeft;
        bool leftInRange = left <= objRight;
        bool topInRange = top <= objBottom;
        bool bottomInRange = bottom >= objTop;



        //determines if boxes are touching
        if (!rightInRange || !leftInRange || !topInRange || !bottomInRange) {

            return GameObject::SIDE::NONE;
        }

        float rightDiff = abs(right - objLeft);
        float leftDiff = abs(left - objRight);
        float topDiff = abs(top - objBottom);
        float bottomDiff = abs(bottom - objTop);


        //now determine from which side they are colliding based off velocity.
        if (this->vel.x == 0){
            float rightDiff = abs(right - objLeft);
            float leftDiff = abs(left - objRight);

            float xDiff = fmin(rightDiff, leftDiff);
            float yDiff = fmin(topDiff, bottomDiff);


            if (yDiff < xDiff){
                if (vel.y > 0){

                    //bogus collision check
                    if (topDiff < bottomDiff){
                         return GameObject::SIDE::NONE;
                    }

                    return GameObject::SIDE::BOTTOM;
                }


                //bogus collision check
                if (bottomDiff < topDiff){
                     return GameObject::SIDE::NONE;
                }

                return GameObject::SIDE::TOP;
            }

            return GameObject::SIDE::NONE;
        }







        //going towards bottom left
        if (this->vel.x < 0 && this->vel.y > 0) {

              if (leftDiff < bottomDiff){

                    //bogus collision check
                    if (rightDiff < leftDiff){
                         return GameObject::SIDE::NONE;
                    }

                    return GameObject::SIDE::LEFT;
              }

              //bogus collision check
              if (topDiff < bottomDiff){
                   return GameObject::SIDE::NONE;
              }

              return GameObject::SIDE::BOTTOM;
        }



        //going towards bottom right
        if (this->vel.x > 0 && this->vel.y > 0) {

              if (rightDiff < bottomDiff){

                    //bogus collision check
                    if (leftDiff < rightDiff){
                        return GameObject::SIDE::NONE;
                    }

                    return GameObject::SIDE::RIGHT;
              }



              //bogus collision check
              if (topDiff < bottomDiff){
                   return GameObject::SIDE::NONE;
              }

              return GameObject::SIDE::BOTTOM;
        }




        //going towards top right
        if (this->vel.x > 0 && this->vel.y < 0) {


              if (rightDiff < topDiff){

                  //bogus collision check
                   if (leftDiff < rightDiff){
                        return GameObject::SIDE::NONE;
                   }

                   return GameObject::SIDE::RIGHT;
              }



              //bogus collision check
               if (bottomDiff < topDiff){
                    return GameObject::SIDE::NONE;
               }

               return GameObject::SIDE::TOP;
         }



        //going towards top left
        if(this->vel.x < 0 && this->vel.y < 0) {


                      if (leftDiff < topDiff){

                      //bogus collision check
                              if (rightDiff < leftDiff){
                                  return GameObject::SIDE::NONE;
                              }

                          return GameObject::SIDE::LEFT;
                      }


                      //bogus collision check
                      if (bottomDiff < topDiff){
                           return GameObject::SIDE::NONE;
                      }

                      return GameObject::SIDE::TOP;
        }


        //all else fails
        return GameObject::SIDE::NONE;
    }



    void GameObject::render(SDL_Renderer * gRenderer)
    {

        SDL_Rect fillRect = {(int) pos.x, (int) pos.y, (int) dim.x, (int) dim.y};
        //sets render color to fill color
        SDL_SetRenderDrawColor(gRenderer, fillClr.r, fillClr.g, fillClr.b, fillClr.a);
        SDL_RenderFillRect(gRenderer, &fillRect);
        //sets render color to line color
        SDL_SetRenderDrawColor(gRenderer, lineClr.r, lineClr.g, lineClr.b, lineClr.a);
        SDL_RenderDrawRect(gRenderer, &fillRect);

    	SDL_Rect dest = {(int) pos.x, (int)pos.y, (int)dim.x, (int)dim.y};
    	SDL_RenderCopy(gRenderer, *spritesheet, NULL, &dest);
    }


    void GameObject::update() {
        if (gameOver) { return; }
        pos += vel;
    }

