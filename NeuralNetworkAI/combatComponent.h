#pragma once
#include "Component.h"


class CombatComponent : public Component
{ 
private:
	sf::RectangleShape healthRed;		//speeder health sprites
	sf::RectangleShape healthGreen;		//

	float attackSpeed;		//float of attack speed
	float attackReloadCounter = 0.0f;	//float to count own between attacks
	bool reloaded;
	float attackDamage;
	float range;
	float health;
	float maxHealth;
	bool attacksEnabled;
public:
	CombatComponent() {}
	CombatComponent(float damageIn, float rangeIn, float healthIn, float attackSpeedIn, sf::Vector2f sizeIn, sf::Vector2f positionIn, bool canAttackIn) { attackDamage = damageIn; range = rangeIn; health = healthIn, attackSpeed = attackSpeedIn, attacksEnabled = canAttackIn;
		
	if (attacksEnabled) {
		attackDamage = 0;
	}

	healthRed.setOrigin(sizeIn.x/2, sizeIn.y/2);
	healthRed.setSize(sf::Vector2f(sizeIn.x, 10));
	healthRed.setPosition(positionIn.x, positionIn.y - 20);
	healthRed.setFillColor(sf::Color(255, 0, 0));

	healthGreen.setOrigin(sizeIn.x / 2, sizeIn.y / 2);
	healthGreen.setSize(sf::Vector2f(sizeIn.x, 10));
	healthGreen.setPosition(positionIn.x, positionIn.y - 20);
	healthGreen.setFillColor(sf::Color(0, 255, 0));
	}

	void update(float dt) override {
		if (attacksEnabled) {
			//check if unit can attack
			if (attackReloadCounter >= attackSpeed) {
				reloaded = true;
			}
			else {
				reloaded = false;
				attackReloadCounter = attackReloadCounter + dt;
			}
		}
	}
	void message(const std::string m) override {}

	sf::RectangleShape* getHealthRed() { return &healthRed; }
	sf::RectangleShape* getHealthGreen() { return &healthGreen; }

	void updatePosition(sf::Vector2f positionIn) {
		healthGreen.setPosition(positionIn.x, positionIn.y - 20);
		healthRed.setPosition(positionIn.x, positionIn.y - 20);
	}

	void damage(float damageIn) { health = health - damageIn; }

	float getDamage() { return attackDamage; }
	float getRange() { return range; }
	float getHealth() { return health; }
	bool canAttack() { return attacksEnabled; }
};