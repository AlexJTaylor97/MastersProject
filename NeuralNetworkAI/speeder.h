#ifndef SPEEDER_H
#define SPEEDER_H

#include <SFML/Graphics.hpp>
#include "unit.h"
#include <iostream>

//speeder statistics
const int speederHealth = 25.0f;
const int speederSpeed = 400.0f;
const int speederDamage = 10.0f;
const float speederAttackSpeed = 2.0f;
const float speederRange = 300.0f;


class Speeder : public GameObject
{
private:
	float m_dt;
public:
	Speeder(sf::Vector2f positionIn, team::team teamIn);
	~Speeder();
	void update(float dt) override;
	void message(const std::string msg) override;
	virtual std::string getMessage() { return " "; }
};

#endif