#ifndef INFANTRY_H
#define INFANTRY_H

#include <SFML/Graphics.hpp>

#include <iostream>
//infantry statistics
const int infantryHealth = 20.0f;
const int infantrySpeed = 250.0f;
const int infantryDamage = 3.0f;
const float infantryAttackSpeed = 0.5f;
const float infantryRange = 300.0f;

class Infantry : public GameObject
{
private:
	float m_dt;
public:
	Infantry(sf::Vector2f positionIn, team::team teamIn);
	~Infantry();
	void update(float dt) override;
	void message(const std::string msg) override;
	virtual std::string getMessage() { return " "; }
};
#endif