#pragma once
#include "Component.h"


const float infantryBuildTime = 5.0f;
const float speederBuildTime = 8.0f;
const float tankBuildTime = 15.0f;

class SpawnerComponent : public Component
{ 
private:
	sf::RectangleShape healthRed;		//speeder health sprites
	sf::RectangleShape healthGreen;		//

	float timer;			//timer to store current units built time remaining

	int infantryBeingTrained = 0;		//integers to store how many of each unit type are being trained
	int speedersBeingTrained = 0;		//
	int tanksBeingTrained = 0;			//

	bool spawningInfantry = false;		//booleans to show if game needs to spawn a specific type of unit
	bool spawningSpeeder = false;		//
	bool spawningTank = false;			//
	
	sf::CircleShape rallyFlag;			//barracks  rally point sprite
	sf::Vector2f rallyPoint;			//position of rally point

	std::vector<objectType::objectType> buildQueue;	//vector of unit types to store its build queue 
public:
	SpawnerComponent() {}
	SpawnerComponent(sf::Vector2f positionIn) {

		rallyPoint.x = positionIn.x + 10;
		rallyPoint.y = positionIn.y + 50;

		rallyFlag.setOrigin(5, 5);
		rallyFlag.setRadius(10);
		rallyFlag.setPosition(rallyPoint);
		rallyFlag.setFillColor(sf::Color(255, 255, 255));

	};

	void update(float dt) override {
		if (infantryBeingTrained > 0 || speedersBeingTrained > 0 || tanksBeingTrained > 0) {
			timer = timer + dt;
		} else {
			timer = 0;
		}

		spawningInfantry = false;
		spawningSpeeder = false;
		spawningTank = false;

		if (buildQueue.size() > 0) {
			if (buildQueue.front() == objectType::infantry && timer >= infantryBuildTime) {
				timer = 0;
				buildQueue.erase(buildQueue.begin());
				infantryBeingTrained--;
				spawningInfantry = true;
			}
		}

		if (buildQueue.size() > 0) {
			if (buildQueue.front() == objectType::speeder && timer >= speederBuildTime) {
				timer = 0;
				buildQueue.erase(buildQueue.begin());
				speedersBeingTrained--;
				spawningSpeeder = true;
			}
		}

		if (buildQueue.size() > 0) {
			if (buildQueue.front() == objectType::tank && timer >= tankBuildTime) {
				timer = 0;
				buildQueue.erase(buildQueue.begin());
				tanksBeingTrained--;
				spawningTank = true;
			}
		}

	
	}
	void message(const std::string m) override {}



	//prompts the barracks to bulild a unit
	void buildUnit(objectType::objectType unitIn) {
		if (unitIn == objectType::infantry) {
			buildQueue.push_back(objectType::infantry);
			infantryBeingTrained++;
		}
		if (unitIn == objectType::speeder) {
			buildQueue.push_back(objectType::speeder);
			speedersBeingTrained++;
		}
		if (unitIn == objectType::tank) {
			buildQueue.push_back(objectType::tank);
			tanksBeingTrained++;
		}
	};			
	void setRallyPoint(sf::Vector2f rallyPointIn) { rallyPoint = rallyPointIn; rallyFlag.setPosition(rallyPoint); };		//sets the rally points position
	sf::Vector2f getRallyPoint() { return rallyPoint; };									//returns the rally points position
	sf::CircleShape* getRallyFlag() { return &rallyFlag; };
	bool spawnInfantry() { return spawningInfantry; };					//checks if the booleans that prompt ga\me to build a unit are true
	bool spawnSpeeder() { return spawningSpeeder; };					//
	bool spawnTank() { return spawningTank; };							//

	int getInfantryBeingTrained() { return infantryBeingTrained; }
	int getSpeedersBeingTrained() { return speedersBeingTrained; }
	int getTanksBeingTrained() { return tanksBeingTrained; }
};