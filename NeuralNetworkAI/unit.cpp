#include "unit.h"
/*
Unit::Unit() {

}

Unit::Unit(sf::Vector2f positionIn, sf::Vector2f halfExtentsIn, int healthIn, float speedIn, float attackSpeedIn, int attackDamageIn,float attackRangeIn, team teamIn, objectType::objectType typeIn):GameObject(positionIn, halfExtentsIn, teamIn, typeIn) {
	health = healthIn;
	maxHealth = healthIn;
	speed = speedIn;
	attackSpeed = attackSpeedIn;
	attackDamage = attackDamageIn;
	attackRange = attackRangeIn;
}

Unit::~Unit() {

}


void Unit::update(float dt) {

	if (orders.size() == 0) {
		orders.push_back(order::none);
	}

	if (orders.size() > 0) {
		if (orders.front() == order::move) {
			//update units poition
			updatePosition(dt);

			//check if unit has arrived
			int dX, dY;
			dX = abs(GameObject::position.x - targetLocations.front().x);
			dY = abs(GameObject::position.x - targetLocations.front().x);

			float tempDistance = sqrt(pow(dX, 2) + pow(dY, 2));

			//if it has arrived, clear present move order
			if (tempDistance < 1) {
				orders.erase(orders.begin());
				targetLocations.erase(targetLocations.begin());
			}
		}
	}
	if (orders.size() > 0) {
		if (orders.front() == order::none) {
			//idle
		}
	}

	//check if unit can attack
	if (attackReloadCounter >= attackSpeed) {
		canAttack = true;
	} else {
		canAttack = false;
		attackReloadCounter = attackReloadCounter + dt;
	}
}

void Unit::updatePosition(float dt) {

	float angleInRads = GameObject::getAngleToTarget(targetLocations.front(), GameObject::position) * 3.14159 / 180;

	float dX = (cos(angleInRads) * speed) * dt;
	float dY = (sin(angleInRads) * speed) * dt;

	GameObject::position.x = GameObject::position.x + dX;
	GameObject::position.y = GameObject::position.y + dY;

}

void Unit::commandMove(sf::Vector2f targetLocation) {

	orders.clear();
	targetLocations.clear();

	orders.push_back(order::move);
	targetLocations.push_back(targetLocation);
}

void Unit::commandChainMove(sf::Vector2f targetLocation) {

	orders.push_back(order::move);
	targetLocations.push_back(targetLocation);
}

void Unit::stop() {
	if (orders.size() > 0) {
		orders.clear();
	}
	if (targetLocations.size() > 0) {
		targetLocations.clear();
	}
}
*/