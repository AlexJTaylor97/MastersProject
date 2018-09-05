#ifndef REFINERY_H
#define REFINERY_H

#include <SFML/Graphics.hpp>

#include "gameObject.h"
#include <iostream>

const int refineryHealth = 100;

class Refinery : public GameObject
{
private:
	float m_dt;
public:
	Refinery(sf::Vector2f positionIn, team::team teamIn);
	~Refinery();
	void update(float dt) override;
	void message(const std::string msg) override;
	virtual std::string getMessage() { return " "; }
};

#endif