#ifndef COMPUTER_H
#define COMPUTER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Utilities.h"
#include "spending.h"
#include "gameObject.h"
#include "barracks.h"
#include "tower.h"
#include "refinery.h"
#include "outpost.h"
#include "resource.h"
#include "tank.h"

#include "fuzzySystem.h"
enum constructionStates { investingInEconomy, investingInDefence, investingInMilitary };				//states that the building state machine can be in
enum actionStates { idle, attacking, defending };														//states that the action state machine can be in

enum resourceTasks{buildRefinery, buildOutpost, buildTower, buildBarracks, buildUnit, none};		//tasks that the buildingState machine can do
enum attackStratergy {destroy, distrupt, raid};														//tasks that the action machine can do
enum defenceStratergy {baseRace, defend, retreat};													//

const sf::Vector2f playerBaseLocation(gameOriginX + 100, gameHeight - 100);							//set positions for the base and rally point
const sf::Vector2f rallyPoint(gameWidth - 200, gameOriginY + 200);

const float timeToForgetAttack = 20.0f;																//constants for time related variables
const float timeToWaitBetweenOrderingBuildings = 10.0f;												//
const float timeToWaitBeforeIdleMove = 10.0f;														//
const float timeToWaitBeforeDefenceMove = 3.0f;														//
const float timeToWaitBeforeAttackMove = 3.0f;														//
class Computer
{
public:
	Computer(std::vector<Resource*> resourcesIn) { resourceNodes = resourcesIn; computerResources = 500; }		//constructor takes in positions of resources and starting resources
	Computer() {}
	//Fuzzy input data
	std::vector<Spending> recentSpending;			//stores the recent spending of the player
	float totalRecentSpending;						//sum of total recent spending

	int recentMilSpending;							//recent spending categorised by type
	int recentEcoSpending;							//
	int recentDefSpending;							//

	float recentMilSpendingPercent = 0;				//recent spending percentage categorised by type for use in fuzzy inference system
	float recentEcoSpendingPercent = 0;				//
	float recentDefSpendingPercent = 0;				//

	int amountOfRefinerys = 0;					//integers that store the amount of buildings the computer has by type
	int amountOfOutposts = 0;					//used in override rules 
	int amountOfTowers = 0;						//
	int amountOfBarracks = 0;					//

	int amountOfPlayerRefineries = 0;			//integers that store the amount of buildings the player has by type
	int amountOfPlayerBarracks = 0;				//	
	int amountOfPlayerUnits = 0;				//integer that stores the amount of units the player has 


	std::vector<Resource*> resourceNodes;		//storage for the resource positions passed in from teh constructor
	sf::Vector2f attackLocation;				//position that stores the location of the attack
	sf::Vector2f closestPlayerBarracks;			//position that stores the position of the closest player barracks for use in attacking production
	sf::Vector2f closestPlayerRefinery;			//position that stores the position of the closest player barracks for use in attacking economy 

	float attackedRecentlyTimer = 0.0f;		//floats for storing timers
	float constructionOrderTimer = 0.0f;	//
	float idleMoveTimer = 0.0f;				//
	float defenceMoveTimer = 10.0f;			//
	float attackMoveTimer = 0.0f;			//

	bool attackedRecently = false;			//boolean variables that are used in the state machine
	bool resourceInRange = false;			//
	bool trainingAtCapacity = false;		//
	bool shouldAttack = false;				//

	bool usingFuzzyOutput = false;			//boolean to switch between using the fuzzy output or the standard rules if there is no fuzzy output

	std::vector<GameObject*> computerAssets;		//vector of the computer's buildings
	std::vector<GameObject*> computerSelectedUnits; //vector of the computer's selected units

	int computerResources = 500;			//integer storing the amount of resources the computer has
	
	float aggressionCalculated = 0;			//float storing the output of teh fuzzy inference system	

	void update(float dt);			//function that updates the computer

	void addResources(int resoucesIn) { computerResources = computerResources + resoucesIn; }		//increses the amount of resouces the player has, used by game which manages the resource income timers

	constructionStates constructionState = investingInEconomy;		//enumerator states for state machine states and tasks
	actionStates actionState;										//
	resourceTasks task;												//
	attackStratergy bestAttackStratergy;							//
	defenceStratergy bestDefenceStratergy;							//
private:
	FuzzySystem fuzzySystem;										//the fuzzy inference system 

	void buildBuilding(objectType::objectType typeIn);										//Utility functions, buids a building depending on type this function manages finding a location and placing the building
	void groupMove(std::vector<GameObject*> group, sf::Vector2f position);					//moves the group of selected units
	float getAngleToTarget(sf::Vector2f targetPositionIn, sf::Vector2f positionIn);			//gets angle between two game objects
	sf::Vector2f inBorder(sf::Vector2f targetLocation);										//confines a position to be in the game world border
};


#endif