#ifndef TOWER_H
#define TOWER_H

#include <SFML/Graphics.hpp>

#include "gameObject.h"
#include <iostream>

//tower values
const int towerHealth = 100.0f;
const int towerDamage = 10.0f;
const float towerAttackSpeed = 1.2f;
const float towerRange = 500.0f;


class Tower : public GameObject
{
private:
	float m_dt;
public:
	Tower(sf::Vector2f positionIn, team::team teamIn);
	~Tower();
	void update(float dt) override;
	void message(const std::string msg) override;
	virtual std::string getMessage() { return " "; }
};

#endif