//#include "gameObject.h"
/*
GameObject::GameObject() {

}

GameObject::GameObject(team::team teamIn, objectType::objectType typeIn) {
	team = teamIn;
	type = typeIn;
}
GameObject::~GameObject() {

}

float GameObject::getAngleToTarget(sf::Vector2f targetPositionIn, sf::Vector2f positionIn) {
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
*/