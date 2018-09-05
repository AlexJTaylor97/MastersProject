#pragma once
#include "Component.h"
#include "Utilities.h"


class MovementComponent : public Component
{ 
private:
	float speed;

	std::vector<orderTypes::orderTypes> orders;		//vector of orders 
	std::vector<sf::Vector2f> targetLocations;	//vector of orders locations

	float getAngleToTarget(sf::Vector2f targetPositionIn, sf::Vector2f positionIn) {
		//get difference in positions
		float dX, dY;
		dX = targetPositionIn.x - positionIn.x;
		dY = targetPositionIn.y - positionIn.y;

		//get distence between us and the ideal location
		float distance = sqrt(pow(dX, 2) + pow(dY, 2));

		//get angle to ideal location
		float angleOfIncident;
		angleOfIncident = (acos(dX / distance));
		angleOfIncident = angleOfIncident * (180 / 3.14);

		float angleToLocation;
		if (positionIn.y > targetPositionIn.y) {
			angleToLocation = 360 - angleOfIncident;
		}
		else {
			angleToLocation = angleOfIncident;
		}
		return angleToLocation;
	}
public:
	MovementComponent() {}
	MovementComponent(float speedIn) { speed = speedIn; }

	void update(float dt) override {}

	void message(const std::string m) override {}

	void commandMove(sf::Vector2f targetLocation) {

		orders.clear();
		targetLocations.clear();
	

		orders.push_back(orderTypes::move);
		targetLocations.push_back(targetLocation);
	}

	void commandChainMove(sf::Vector2f targetLocation) {

		orders.push_back(orderTypes::none);
		targetLocations.push_back(targetLocation);
	}


	void stop() {
		if (orders.size() > 0) {
			orders.clear();
		}
		if (targetLocations.size() > 0) {
			targetLocations.clear();
		}
	}

	sf::Vector2f updatePosition(float dt, sf::Vector2f positionIn) {

		float angleInRads = getAngleToTarget(targetLocations.front(), positionIn) * 3.14159 / 180;

		float dX = (cos(angleInRads) * speed) * dt;
		float dY = (sin(angleInRads) * speed) * dt;

		sf::Vector2f newPosition;
		newPosition.x = positionIn.x + dX;
		newPosition.y = positionIn.y + dY;

		return newPosition;
		

	}

	orderTypes::orderTypes getOrder() {
		if (orders.size() > 0) {
			return orders.front();
		}	else {
			return orderTypes::none;
		}
		
	}
	sf::Vector2f getTargetPosition() { return targetLocations.front(); }
};


