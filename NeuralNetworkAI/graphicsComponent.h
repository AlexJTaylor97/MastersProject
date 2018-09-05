#pragma once
#include "Component.h"



class GraphicsComponent : public Component
{ 
private:
	sf::Vector2f size;
	sf::Vector2f halfExtents;
public:
	sf::RectangleShape sprite;
	GraphicsComponent() {}
	GraphicsComponent(sf::Vector2f sizeIn, sf::Vector2f positionIn, sf::Color colourIn) { 
			
		size = sizeIn;
		halfExtents.x = size.x / 2;
		halfExtents.y = size.y / 2;

		sprite.setOrigin(halfExtents);
		sprite.setSize(sizeIn), 
		sprite.setPosition(positionIn), 
		sprite.setFillColor(colourIn);
	}

	void update(float dt) override {}
	void message(const std::string m) override {}

	sf::RectangleShape* getSprite() { return &sprite; }

	void updatePosition(sf::Vector2f positionIn) {
		sprite.setPosition(positionIn.x, positionIn.y);
	}


	sf::Vector2f getSize() { return size; }
	sf::Vector2f getHalfExtents() { return halfExtents; }
};