#ifndef BASE_H
#define BASE_H

#include <SFML/Graphics.hpp>
#include "gameObject.h"
#include <iostream>
const int baseHealth = 500;
class Base : public GameObject
{
private:
	float m_dt;
public:
	Base(sf::Vector2f positionIn, team::team teamIn);
	~Base();
	void update(float dt) override;
	void message(const std::string msg) override;
	virtual std::string getMessage() { return " "; }
};

#endif