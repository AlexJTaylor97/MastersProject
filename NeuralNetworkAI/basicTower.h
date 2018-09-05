#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>

#include "building.h"
#include <iostream>

const int refineryHealth = 100;

class Tower : public Building
{
private:
	sf::RectangleShape sprite;
	sf::CircleShape selectionCircle;

public:
	Tower( sf::Vector2f positionIn, team teamIn);
	~Tower();

	virtual void draw(RenderTarget &target, RenderStates states)const;
	virtual void update(float deltaTime);


};

#endif