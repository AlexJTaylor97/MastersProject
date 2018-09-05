#pragma once
#include "Component.h"



class SelectableComponent : public Component
{ 
private:
	bool selected = false;
	sf::CircleShape selectionCircle;
public:

	SelectableComponent() {}
	SelectableComponent(sf::Vector2f sizeIn, sf::Vector2f positionIn) { 
		selectionCircle.setRadius(sizeIn.x/1.8); 
		selectionCircle.setFillColor(sf::Color(255, 255, 255, 0));
		selectionCircle.setOutlineColor(sf::Color(255, 255, 255));
		selectionCircle.setOutlineThickness(3);
		selectionCircle.setOrigin(sizeIn.x/1.8, sizeIn.y/1.8);
		selectionCircle.setPosition(positionIn);
	}

	void update(float dt) override {}
	void message(const std::string m) override {}

	bool isSelected() { return selected; }
	void select() { selected = true; }
	void deSelect() { selected = false; }


	void updatePosition(sf::Vector2f positionIn) {
		selectionCircle.setPosition(positionIn);
	}

	sf::CircleShape* getSelectionCircle() { return &selectionCircle; }

};