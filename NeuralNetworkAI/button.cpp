#include "button.h"

Button::Button() {

}

Button::Button(sf::Vector2f positionIn, String textIn, bool showNumberIn, sf::Vector2i screenSize) {
	sf::Vector2f size = sf::Vector2f(((screenSize.x / 5) / 3) - 5, screenSize.y / 10);				//places button relative to screen size
	position = positionIn;
	halfExtents = sf::Vector2f(((((screenSize.x / 5) / 3) - 5) / 2), ((screenSize.y / 10) - 5) / 2);//sizes button relative to screen size
	text = textIn; 

	if (!font.loadFromFile(".\\assets\\font.ttf")) {
		std::cout << "Error loading Font" << std::endl;
	}
	//sets sprites positions
	displayText.setCharacterSize(28);
	displayText.setFont(font);
	displayText.setOrigin(halfExtents.x, halfExtents.y);
	displayText.setPosition(position.x + 3, position.y + 3);
	displayText.setFillColor(sf::Color::Red);
	displayText.setString(text);

	queueSizeText.setCharacterSize(28);
	queueSizeText.setFont(font);
	queueSizeText.setOrigin(halfExtents.x, halfExtents.y);
	queueSizeText.setPosition(position.x + 3, position.y + 25);
	queueSizeText.setFillColor(sf::Color::Red);
	queueSizeText.setString(" ");

	sprite.setFillColor(sf::Color(64, 64, 64, 255));
	sprite.setOutlineColor(sf::Color(192, 192, 192, 255));
	sprite.setOutlineThickness(3);
	sprite.setOrigin(halfExtents.x, halfExtents.y);
	sprite.setSize(size);
	sprite.setPosition(position);
}
Button::~Button() {

}


void Button::draw(RenderTarget &target, RenderStates states)const {
	target.draw(sprite, states);
	target.draw(displayText, states);
	if (showNumber) {
		target.draw(queueSizeText, states);
	}
}

void Button::updateAmount(int amountIn){
	if (amountIn == 0) {
		queueSizeText.setString(" ");
	}
	else {
		queueSizeText.setString(std::to_string(amountIn));
	}
}

void Button::updateAmount(int costIn ,int amountIn) {

	String tempString = std::to_string(costIn) + "          " +  std::to_string(amountIn);
	queueSizeText.setString(tempString);
}

bool Button::isClicked(sf::Vector2f mousePosition) {
	if (mousePosition.x < position.x + halfExtents.x && mousePosition.x > position.x - halfExtents.x && mousePosition.y < position.y + halfExtents.y && mousePosition.y > position.y - halfExtents.y) {
		return true;
	}
	else {
		return false;
	}
}

