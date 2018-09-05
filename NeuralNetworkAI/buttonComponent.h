#pragma once
#include "Component.h"


class ButtonComponent : public Component
{ 
private:
	bool visible;
	bool pressed;
public:
	sf::RectangleShape sprite;
	ButtonComponent() {}
	ButtonComponent(sf::Vector2f sizeIn, sf::Vector3i colourIn) { sprite.setSize(sizeIn), sprite.setFillColor(sf::Color(colourIn.x, colourIn.y, colourIn.z)); }

	void update(float dt) override {}
	void message(const std::string m) override {}

	void setVisible(bool visibleIn) { visible = visibleIn; }
	void press() { pressed = true; }
	bool isPressed() { 
		if (pressed) {
			pressed = false;
			return true;
		} else {
			return false;
		}
	 }
};