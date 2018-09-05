#ifndef TANK_H
#define TANK_H

#include <SFML/Graphics.hpp>
#include "unit.h"
#include <iostream>
//tank statistics
const int tankHealth = 50.0f;
const float tankSpeed = 200.0f;
const int tankDamage = 20.0f;
const float tankAttackSpeed = 4.0f;
const float tankRange = 500.0f;

class Tank : public GameObject
{

private:
	float m_dt;
public:
	Tank(sf::Vector2f positionIn, team::team teamIn);
	~Tank();
	void update(float dt) override;
	void message(const std::string msg) override;
	virtual std::string getMessage() { return " "; }
};

#endif