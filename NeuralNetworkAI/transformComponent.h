#pragma once
#include "Component.h"


class TransformComponent : public Component
{ 
private:
	
	sf::Vector2f position;

public:
	sf::RectangleShape sprite;
	TransformComponent() {}
	TransformComponent(sf::Vector2f positionIn) { position = positionIn; }

	void update(float dt)override {};
	void message(const std::string m)override {};
	void draw(sf::RenderTarget &target, sf::RenderStates states)const {};
	void setPosition(sf::Vector2f positionIn) {
		position = positionIn;
	}
	sf::Vector2f getPosition() {
		return position;
	}
};