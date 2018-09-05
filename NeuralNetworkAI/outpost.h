#ifndef OUTPOST_H
#define OUTPOST_H

#include <SFML/Graphics.hpp>

#include "gameObject.h"
#include <iostream>

const int outpostHealth = 150;

class Outpost : public GameObject
{
private:
	float m_dt;
public:
	Outpost(sf::Vector2f positionIn, team::team teamIn);
	~Outpost();
	void update(float dt) override;
	void message(const std::string msg) override;
	virtual std::string getMessage() { return " "; }
};


#endif