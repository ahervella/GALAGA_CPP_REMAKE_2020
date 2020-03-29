//
// Created by Alejandro Hervella on 2/6/20.
//

#ifndef PROJ1_BREAKOUT_MIKEY_BREAKOUT_UTIL_HPP
#define PROJ1_BREAKOUT_MIKEY_BREAKOUT_UTIL_HPP



#include <SDL.h>

//for pc
//#include "SDL2/SDL_ttf.h"

//for mac
//#include "SDL2_ttf/SDL_ttf.h"
#include <SDL_ttf.h>

#include <iostream>
#include <string>


/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg);

/*
 * Get the resource path for resources located in res/subDir
 */
std::string getResourcePath(const std::string &subDir = "");

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
                        SDL_Color color, int fontSize, SDL_Renderer *renderer);

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

/*
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
//same as before
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);



#endif //PROJ1_BREAKOUT_MIKEY_BREAKOUT_UTIL_HPP
