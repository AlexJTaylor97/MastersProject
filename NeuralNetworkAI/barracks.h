#ifndef BARRACKS_H
#define BARRACKS_H

#include <SFML/Graphics.hpp>
#include "gameObject.h"
#include <iostream>
//barracks constant values

/*
const float infantryBuildTime = 5.0f;
const float speederBuildTime = 8.0f;
const float tankBuildTime = 15.0f;
*/

const int barracksHealth = 100;


class Barracks : public GameObject
{
private:
	float m_dt;
public:
	Barracks(sf::Vector2f positionIn, team::team teamIn);
	~Barracks();
	void update(float dt) override;
	void message(const std::string msg) override;
	virtual std::string getMessage() { return " "; }
};

#endif