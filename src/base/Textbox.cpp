#include "Textbox.hpp"



Textbox::Textbox() {
}

Textbox::Textbox(std::string text, int textSize, int x, int y) {
	this->text = text;
	this->textSize = textSize;
	this->x = x;
	this->y = y;
}

CounterTextbox::CounterTextbox(std::string textTemplate, int counter, int textSize, int x, int y) {
    this -> textTemplate = textTemplate;
    setText(counter);
    this->textSize = textSize;
    this->x = x;
    this->y = y;
}

void CounterTextbox::setText(int counter) {
    this->currCounter = counter;
    this->text = this->textTemplate + std::to_string(counter);
}
