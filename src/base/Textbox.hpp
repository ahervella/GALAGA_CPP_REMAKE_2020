#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP


#include "util.hpp"

//#include "GameObject.hpp"
#include <iostream>
#include <string>


/**
 * A class that represents a textbox to display important text in a game
 */
class Textbox {

public:
	std::string text;

    SDL_Color clr = {255, 255, 255, 255};
    int textSize = 6;
    int x = 5;
    int y = 5;
    std::string textResPath = getResourcePath() + "NotoSerif-Black.ttf";

    /**
     * Default Textbox constructor. Does nothing.
     */
    Textbox();

    /**
     * Textbox constructor that initializes the text, position, and size of text box
     *
     * @param text: the text to be printed
     * @param textSize: the size of the text
     * @param x: x coordinate
     * @param y: y coordinate
     */
    Textbox(std::string text, int textSize, int x, int y);
};

/**
 * A subclass of TextBox that includes a field for a counted variable (lives, score, etc.)
 */
class CounterTextbox : public Textbox {

public:

    std::string textTemplate;
    int currCounter = 0;

    /**
     * Same as Textbox, but with a counter (e.g. score, lives, etc.)
     *
     * @param text: the text to be printed
     * @param counter: variable number to be printed out (e.g. score, lives, etc.)
     * @param textSize: the size of the text
     * @param x: x coordinate
     * @param y: y coordinate
     */
    CounterTextbox(std::string textTemplate, int counter, int textSize, int x, int y);

    /**
     * Sets the counter for the CounterTextbox
     *
     * @param counter: the variable number to be printed out onto the screen
     */
    void setText(int counter);



};

#endif
