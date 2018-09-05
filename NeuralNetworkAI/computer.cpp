#include "computer.h"
void Computer::update(float dt) {
	////////////check changes in AI boolean data/////////////////
	//check if ai is attacked recently (most of this is handled in game's update area)
	/*
	if (attackedRecently) {
		attackedRecentlyTimer = attackedRecentlyTimer + dt;
		if (attackedRecentlyTimer > timeToForgetAttack) {
			attackedRecently = false;
			attackedRecentlyTimer = 0.0f;
		}
	}
	
	//check if resource is in range
	resourceInRange = false;
	for (int i = 0; i < resourceNodes.size(); i++) {
		for (int j = 0; j < computerBuildings.size(); j++) {
			int tempDx, tempDy;
			//check if there is a base nearby
			if (computerBuildings.at(j)->getType() == objectType::base) {
				tempDx = abs(computerBuildings.at(j)->getPosition().x - resourceNodes.at(i)->getPosition().x);
				tempDy = abs(computerBuildings.at(j)->getPosition().y - resourceNodes.at(i)->getPosition().y);
				float buildRangeDistance = sqrt(pow(tempDx, 2) + pow(tempDy, 2));
				//if it is nearby check if a refinery is already built there
				if (buildRangeDistance < baseBuildRadius) {
					float closestRefinery = 99999;
					
					for (int k = 0; k < computerBuildings.size(); k++) {
						if (computerBuildings.at(k)->getType() == objectType::refinery) {
							float dX, dY;
							dX = resourceNodes.at(i)->getPosition().x - computerBuildings.at(k)->getPosition().x;
							dY = resourceNodes.at(i)->getPosition().y - computerBuildings.at(k)->getPosition().y;

							float distanceFromRefinery = sqrt(pow(dX, 2) + pow(dY, 2));
							if(distanceFromRefinery < closestRefinery){
								closestRefinery = distanceFromRefinery;
							}
						}
					}
					//if its withing range of a base and the nearest refinery to it is not on it then there is a resource in range
					if (closestRefinery > 20) {
						resourceInRange = true;
					}
				}
			}
			//repeat the previous checks only using the outposts build radius not the bases
			if (computerBuildings.at(j)->getType() == objectType::outpost) {
				tempDx = abs(computerBuildings.at(j)->getPosition().x - resourceNodes.at(i)->getPosition().x);
				tempDy = abs(computerBuildings.at(j)->getPosition().y - resourceNodes.at(i)->getPosition().y);
				float buildRangeDistance = sqrt(pow(tempDx, 2) + pow(tempDy, 2));
				if (buildRangeDistance < outpostBuildRadius) {
					float closestRefinery = 99999;

					for (int k = 0; k < computerBuildings.size(); k++) {
						if (computerBuildings.at(k)->getType() == objectType::refinery) {
							float dX, dY;
							dX = resourceNodes.at(i)->getPosition().x - computerBuildings.at(k)->getPosition().x;
							dY = resourceNodes.at(i)->getPosition().y - computerBuildings.at(k)->getPosition().y;

							float distanceFromRefinery = sqrt(pow(dX, 2) + pow(dY, 2));
							if (distanceFromRefinery < closestRefinery) {
								closestRefinery = distanceFromRefinery;
							}
						}
					}
					if (closestRefinery > 20) {
						resourceInRange = true;
					}
				}
			}
		}
	}

	//check if training is at capcaity
	trainingAtCapacity = true;
	for (int i = 0; i < computerBuildings.size(); i++) {
		if (computerBuildings.at(i)->getType() == objectType::barracks) {
			if (computerBuildings.at(i)->getInfantryBeingTrained() + computerBuildings.at(i)->getSpeedersBeingTrained() + computerBuildings.at(i)->getTanksBeingTrained() < 5) {
				trainingAtCapacity = false;
			}
		}
	}

	//check if AI should attack
	shouldAttack = false;
	if (!attackedRecently && computerUnits.size() > 20) {
		shouldAttack = true;
	}

	///////////////////Construction state machine//////////////////
	switch (constructionState) {
	case(investingInEconomy):////////////////////////////////////////////////////////
	{
		//check if AI is already building refinery 
		bool buildingRefinery = false;
		if (task == buildRefinery) {
			buildingRefinery = true;
		}
		//if there is a resource node in range plan to build on it if not plan to build at outpost
		if (resourceInRange) {
			task = buildRefinery;
		}
		else {
			task = buildOutpost;
		}
		break;
	}
		
	case(investingInDefence)://///////////////////////////////////////////////////
	{
		//only task that can be done is to build a tower this logic makes sre it is always doing that if the state is to invest in defence
		bool buildingTower = false;
		if (task == buildTower) {
			buildingTower = true;
		}
		if (!buildingTower) {
			task = buildTower;
		}
		break;
	}

	case(investingInMilitary)://///////////////////////////////////////////////////
	{
		//if all the barracks queues are past a threshold build a new barraks if there is room in the queue build units instead
		if (trainingAtCapacity) {
			task = buildBarracks;
		}
		else {
			task = buildUnit;
		}
		break;
	}
	}

	//check for change in state
	//general rules only apply if the fuzzy inference system doesnt have enough data to run
	if (usingFuzzyOutput) {
		if (aggressionCalculated > 80) {
			constructionState = investingInDefence;
		} else if (aggressionCalculated > 60) {
			constructionState = investingInMilitary;
		}	else {
			constructionState = investingInEconomy;
		}
	} else {
		//standard general rules to be used if fuzzy system isnt valid at this time
		//if attacked recently build defence, if resources are too low and you dont already have too many refineries invest in ecomony, else invest in military 
		if (attackedRecently) {
			constructionState = investingInDefence;
		}
		else if (computerResources < 500 && amountOfRefinerys < 12) {
			constructionState = investingInEconomy;
		}
		else {
			constructionState = investingInMilitary;
		}
	}

	
	// resets the amount of buildings the computers have
	amountOfRefinerys = 0;
	amountOfOutposts = 0;
	amountOfTowers = 0;
	amountOfBarracks = 0;

	//loops through the buildings to re populate teh counters we just reset
	for (int i = 0; i < computerBuildings.size(); i++) {
		if (computerBuildings.at(i)->getType() == objectType::refinery ) {
			amountOfRefinerys++;
		}
		if (computerBuildings.at(i)->getType() == objectType::outpost) {
			amountOfOutposts++;
		}
		if (computerBuildings.at(i)->getType() == objectType::tower) {
			amountOfTowers++;
		}
		if (computerBuildings.at(i)->getType() == objectType::barracks) {
			amountOfBarracks++;
		}
	}

	//special rules that overwrite the general trend defeined in the rules abov
	//if there are way more buildings than units force the ai to build units
	if (amountOfRefinerys + amountOfOutposts > computerUnits.size()) {
		constructionState = investingInMilitary;
	}
	//if the rules above set it to defence but there are already far too many towers force teh ai to buidl units 
	if (constructionState == investingInDefence && amountOfTowers > amountOfBarracks + amountOfOutposts + amountOfRefinerys) {
		constructionState = investingInMilitary;
	}
	//if there are too many economy buildings with not enough towers force the ai to build towers
	if ((amountOfRefinerys + amountOfOutposts) / 2 > amountOfTowers) {
		constructionState = investingInDefence;
	}

	
	
	///////////////////Action state machine//////////////////
	switch (actionState) {
	case(idle)://////////////////////////////////////////////////////////
		//if state is idle move units back to rally point every few seconds
		idleMoveTimer = idleMoveTimer + dt;
		if (idleMoveTimer > timeToWaitBeforeIdleMove) {
			groupMove(computerUnits, rallyPoint);
			idleMoveTimer = 0;
		}
		break;
	case(attacking)://////////////////////////////////////////////////////////
		//if task is attack order a new movement every few seconds
		attackMoveTimer = attackMoveTimer + dt;
		if (attackMoveTimer > timeToWaitBeforeAttackMove) {
			//determine which attack stratergy to use for this movement
			if (amountOfPlayerBarracks > amountOfBarracks) {
				bestAttackStratergy = distrupt;
			} else if (amountOfPlayerRefineries > amountOfRefinerys) {
				bestAttackStratergy = raid;
			} else {
				bestAttackStratergy = destroy;
			}
			//apply attack stratergy
			switch (bestAttackStratergy) {
			case(raid):
				groupMove(computerUnits, closestPlayerRefinery);
				break;
			case(distrupt):
				groupMove(computerUnits, closestPlayerBarracks);
				break;
			case(destroy):
				groupMove(computerUnits, playerBaseLocation);
				break;
			}
		}		
		break;
	case(defending):///////////////////////////////////////////////////////////
		//if defending order a defencive move every few seconds
		defenceMoveTimer = defenceMoveTimer + dt;
		if (defenceMoveTimer > timeToWaitBeforeDefenceMove) {
			//determien which defencive move to do
			if (computerUnits.size() / 1.5 > amountOfPlayerUnits) {
				bestDefenceStratergy = baseRace;
			} else if (amountOfPlayerUnits / 2 > computerUnits.size()) {
				bestDefenceStratergy = retreat;
			} else {
				bestDefenceStratergy = defend;
			}

			//apply defencive move
			switch (bestDefenceStratergy) {
			case(baseRace):
				groupMove(computerUnits, playerBaseLocation);
				break;
			case(defend):
				groupMove(computerUnits, attackLocation);
				break;
			case(retreat):
				groupMove(computerUnits, rallyPoint);
				break;
			}
			defenceMoveTimer = 0;
		}
		
		break;
	}

	//general rules for changing action state
	if (usingFuzzyOutput) {
		if (aggressionCalculated < 40 && amountOfPlayerUnits < computerUnits.size()) {
			actionState = attacking;
		}
		else if (attackedRecently) {
			actionState = defending;
		}
		else {
			actionState = idle;
		}
	} else {
		if (shouldAttack) {
			actionState = attacking;
		}
		else if (attackedRecently) {
			actionState = defending;
		}
		else {
			actionState = idle;
		}
	}

	
	////////////////////task handling ///////////////////////
	switch (task) {
		case(buildUnit):
			//for every 100 resources build a unit
			for (int i = 0; i < (computerResources / 100); i++) {
			int unit = rand() % ((3 - 1) + 1) + 1;		//randomly pick what kind of unit
			int queue = 999;							//
			GameObject* lowestQueue = new Tank(sf::Vector2f(500, 500), player);					//needs to be initilised to something, this will be immidiatly overwritten

			//loop through the buildings to find barracks and check which has the lowest queue
			for (int j = 0; j < computerBuildings.size(); j++) {
				if (computerBuildings.at(j)->getType() == objectType::barracks) {
					if (computerBuildings.at(j)->getInfantryBeingTrained() + computerBuildings.at(j)->getSpeedersBeingTrained() + computerBuildings.at(j)->getTanksBeingTrained() < queue) {
						lowestQueue = computerBuildings.at(j);
						queue = computerBuildings.at(j)->getInfantryBeingTrained() + computerBuildings.at(j)->getSpeedersBeingTrained() + computerBuildings.at(j)->getTanksBeingTrained();
					}
				}
			}
			lowestQueue->setRallyPoint(rallyPoint);
			
			//now that we have the barracks with the lowest queue tell it it build a unit
			switch (unit) {
			case(1):
				lowestQueue->buildUnit(units::infantry);
				computerResources = computerResources - infantryCost;
				break;
			case(2):
				lowestQueue->buildUnit(units::speeder);
				computerResources = computerResources - speederCost;
				break;
			case(3):
				lowestQueue->buildUnit(units::tank);
				computerResources = computerResources - tankCost;
				break;
				}
			}
			break;
		case(buildBarracks):
			buildBuilding(objectType::barracks);
			break;
		case(buildOutpost):
			buildBuilding(objectType::outpost);
			break;
		case(buildRefinery):
			buildBuilding(objectType::refinery);
			break;
		case(buildTower):
			buildBuilding(objectType::tower);
			break;
	}

	////////////////////fuzzy system ///////////////////////
	if (totalRecentSpending > 0) {
		aggressionCalculated = fuzzySystem.calculateAggression(recentMilSpendingPercent, recentEcoSpendingPercent, recentDefSpendingPercent);
		usingFuzzyOutput = true;
	} else {
		usingFuzzyOutput = false;
	}
	*/
}

void Computer::buildBuilding(objectType::objectType typeIn) {
/*
	switch (typeIn) {
	
	case(objectType::barracks):
	{
		//if the type is barracks 
		bool builtBarracks = false;
		//check if it can be affoarded
		if (computerResources > barracksCost) {
			//if so find a suitable center building (base or outpost)
			std::vector<GameObject*> centerBuildings;
			for (int i = 0; i < computerBuildings.size(); i++) {
				if (computerBuildings.at(i)->getType() == objectType::base || computerBuildings.at(i)->getType() == objectType::outpost) {
					centerBuildings.push_back(computerBuildings.at(i));
				}
			}
			int randdomlySelectedBuilding = rand() % ((centerBuildings.size() - 0) + 1) + 0;
			//randomly select a center building for use
			if (randdomlySelectedBuilding > centerBuildings.size() - 1) {
				randdomlySelectedBuilding--;
			}
			//calculate a random position to build the barracks within range of a center buiding
			sf::Vector2f buildPosition;
			int angle = rand() % ((180 - 90) + 1) + 90;
			float radian = (angle * 3.141592) / 180;

			float xDiff, yDiff;
			if (centerBuildings.at(randdomlySelectedBuilding)->getType() == objectType::base) {
				xDiff = baseBuildRadius / 2 * cos(radian);
				yDiff = baseBuildRadius / 2 * sin(radian);
			}
			else {
				xDiff = outpostBuildRadius / 2 * cos(radian);
				yDiff = outpostBuildRadius / 2 * sin(radian);
			}
			float xPos = centerBuildings.at(randdomlySelectedBuilding)->getPosition().x + xDiff;
			float yPos = centerBuildings.at(randdomlySelectedBuilding)->getPosition().y + yDiff;
			computerResources = computerResources - barracksCost;
			computerBuildings.push_back(new Barracks(sf::Vector2f(xPos, yPos), computer));
			//build the barracks
			builtBarracks = true;
			
		}
	}
	break;
	case(objectType::outpost):
	{
		bool builtOutpost = false;
		//if the type is outpost check if you can affoard it
		if (computerResources > outpostCost) {
			//Get the clostst unclaimed resource position
			sf::Vector2f closestResourcePosition;
			float closestDistance = 99999;
			//loop through each resource to see if it is in range of an outpost or base
			for (int i = 0; i < resourceNodes.size(); i++) {

				bool inBuildRange = false;
				for (int j = 0; j < computerBuildings.size(); j++) {
					int tempDx, tempDy;
					if (computerBuildings.at(j)->getType() == objectType::base) {
						tempDx = abs(computerBuildings.at(j)->getPosition().x - resourceNodes.at(i)->getPosition().x);
						tempDy = abs(computerBuildings.at(j)->getPosition().y - resourceNodes.at(i)->getPosition().y);
						float buildRangeDistance = sqrt(pow(tempDx, 2) + pow(tempDy, 2));
						if (buildRangeDistance < baseBuildRadius) {
							inBuildRange = true;
						}
					}
					if (computerBuildings.at(j)->getType() == objectType::outpost) {
						tempDx = abs(computerBuildings.at(j)->getPosition().x - resourceNodes.at(i)->getPosition().x);
						tempDy = abs(computerBuildings.at(j)->getPosition().y - resourceNodes.at(i)->getPosition().y);
						float buildRangeDistance = sqrt(pow(tempDx, 2) + pow(tempDy, 2));
						if (buildRangeDistance < outpostBuildRadius) {
							inBuildRange = true;
						}
					}
				}
				if (!inBuildRange) {
					float closestDistanceFromCores = 99999;
					for (int j = 0; j < computerBuildings.size(); j++) {
						int tempDx, tempDy;
						if (computerBuildings.at(j)->getType() == objectType::base) {
							tempDx = abs(computerBuildings.at(j)->getPosition().x - resourceNodes.at(i)->getPosition().x);
							tempDy = abs(computerBuildings.at(j)->getPosition().y - resourceNodes.at(i)->getPosition().y);
							float dist = sqrt(pow(tempDx, 2) + pow(tempDy, 2));
							if (dist < closestDistanceFromCores) {
								closestDistanceFromCores = dist;
							}
						}
						if (computerBuildings.at(j)->getType() == objectType::outpost) {
							tempDx = abs(computerBuildings.at(j)->getPosition().x - resourceNodes.at(i)->getPosition().x);
							tempDy = abs(computerBuildings.at(j)->getPosition().y - resourceNodes.at(i)->getPosition().y);
							float dist = sqrt(pow(tempDx, 2) + pow(tempDy, 2));
							if (dist < closestDistanceFromCores) {
								closestDistanceFromCores = dist;
							}
						}
					}
					if (closestDistanceFromCores < closestDistance) {
						closestDistance = closestDistanceFromCores;
						closestResourcePosition = resourceNodes.at(i)->getPosition();
					}
				}
			}

			//get the closest core building to the unclaimed resource
			sf::Vector2f closestCorebuildingPosition;
			float buildRadius;
			float dist = 9999;
			for (int i = 0; i < computerBuildings.size(); i++) {
				if (computerBuildings.at(i)->getType() == objectType::base) {
					float dX, dY;
					dX = closestResourcePosition.x - computerBuildings.at(i)->getPosition().x;
					dY = closestResourcePosition.y - computerBuildings.at(i)->getPosition().y;

					if (sqrt(pow(dX, 2) + pow(dY, 2)) < dist) {
						dist = sqrt(pow(dX, 2) + pow(dY, 2));
						closestCorebuildingPosition = computerBuildings.at(i)->getPosition();
						buildRadius = baseBuildRadius;
					}
				}

				if (computerBuildings.at(i)->getType() == objectType::outpost) {
					float dX, dY;
					dX = closestResourcePosition.x - computerBuildings.at(i)->getPosition().x;
					dY = closestResourcePosition.y - computerBuildings.at(i)->getPosition().y;

					if (sqrt(pow(dX, 2) + pow(dY, 2)) < dist) {
						dist = sqrt(pow(dX, 2) + pow(dY, 2));
						closestCorebuildingPosition = computerBuildings.at(i)->getPosition();
						buildRadius = outpostBuildRadius;
					}
				}
			}

			//build it 
			sf::Vector2f buildPosition;
			int angle = getAngleToTarget(closestResourcePosition, closestCorebuildingPosition);
			float radian = (angle * 3.141592) / 180;

			float xDiff, yDiff;
			xDiff = buildRadius * cos(radian);
			yDiff = buildRadius * sin(radian);

			float xPos = closestCorebuildingPosition.x + xDiff;
			float yPos = closestCorebuildingPosition.y + yDiff;

			computerBuildings.push_back(new Outpost(sf::Vector2f(xPos, yPos), computer));
			computerResources = computerResources - outpostCost;
			builtOutpost = true;
		}
	}
	break;
	case(objectType::refinery):
	{
		bool builtRefinery = false; //exists to stop it building multiple refinerys due to overlap and to see if the build failed 
		//check if the type is refinery if so check if you can affoard it
		if (computerResources > refineryCost) {
			//check if a refinery is already built on that node
			for (int i = 0; i < resourceNodes.size(); i++) {
				float distanceFromRefinery = 9999;
				for (int j = 0; j < computerBuildings.size(); j++) {
					if (computerBuildings.at(j)->getType() == objectType::refinery) {
						float dX, dY;
						dX = resourceNodes.at(i)->getPosition().x - computerBuildings.at(j)->getPosition().x;
						dY = resourceNodes.at(i)->getPosition().y - computerBuildings.at(j)->getPosition().y;

						if (sqrt(pow(dX, 2) + pow(dY, 2) < distanceFromRefinery)) {
							distanceFromRefinery = sqrt(pow(dX, 2) + pow(dY, 2));
						}
					}
				}
				//if a refinery is not built on that node check it is in range of a base or outpost
				if (distanceFromRefinery > 20) {
					for (int j = 0; j < computerBuildings.size(); j++) {
						int tempDx, tempDy;
						if (computerBuildings.at(j)->getType() == objectType::base && !builtRefinery) {
							tempDx = abs(computerBuildings.at(j)->getPosition().x - resourceNodes.at(i)->getPosition().x);
							tempDy = abs(computerBuildings.at(j)->getPosition().y - resourceNodes.at(i)->getPosition().y);
							float buildRangeDistance = sqrt(pow(tempDx, 2) + pow(tempDy, 2));
							if (buildRangeDistance < baseBuildRadius) {
								computerResources = computerResources - refineryCost;
								computerBuildings.push_back(new Refinery(resourceNodes.at(i)->getPosition(), computer));
								builtRefinery = true;
							}
						}
						if (computerBuildings.at(j)->getType() == objectType::outpost && !builtRefinery) {
							tempDx = abs(computerBuildings.at(j)->getPosition().x - resourceNodes.at(i)->getPosition().x);
							tempDy = abs(computerBuildings.at(j)->getPosition().y - resourceNodes.at(i)->getPosition().y);
							float buildRangeDistance = sqrt(pow(tempDx, 2) + pow(tempDy, 2));
							//if it is in range build the refinery
							if (buildRangeDistance < outpostBuildRadius) {
								computerResources = computerResources - refineryCost;
								computerBuildings.push_back(new Refinery(resourceNodes.at(i)->getPosition(), computer));
								builtRefinery = true;
							}
						}
					}
				}
			}
		}
	}
	break;
	case(objectType::tower):
	{
		bool builtTower = false;
		//if the type is tower check if you can affoard it
		if (computerResources > towerCost) {
			//if so find all center buildings (base and outposts)
			std::vector<GameObject*> centerBuildings;
			for (int i = 0; i < computerBuildings.size(); i++) {
				if (computerBuildings.at(i)->getType() == objectType::base || computerBuildings.at(i)->getType() == objectType::outpost) {
					centerBuildings.push_back(computerBuildings.at(i));
				}
			}
			//randomly select a center building to build the tower around
			int randdomlySelectedBuilding = rand() % ((centerBuildings.size() - 0) + 1) + 0;
			if (randdomlySelectedBuilding > centerBuildings.size() - 1) {
				randdomlySelectedBuilding--;
			}

			//randomply place the tower around the selected center building
			sf::Vector2f buildPosition;
			int angle = rand() % ((180 - 90) + 1) + 90;
			float radian = (angle * 3.141592) / 180;

			float xDiff, yDiff;
			if (centerBuildings.at(randdomlySelectedBuilding)->getType() == objectType::base) {
				xDiff = baseBuildRadius * cos(radian);
				yDiff = baseBuildRadius * sin(radian);
			}
			else {
				xDiff = outpostBuildRadius * cos(radian);
				yDiff = outpostBuildRadius * sin(radian);
			}
			float xPos = centerBuildings.at(randdomlySelectedBuilding)->getPosition().x + xDiff;
			float yPos = centerBuildings.at(randdomlySelectedBuilding)->getPosition().y + yDiff;

			computerResources = computerResources - towerCost;
			computerBuildings.push_back(new Tower(sf::Vector2f(xPos, yPos), computer));
			//build the tower
			builtTower = true;
		}
	break;
	}
	}
	*/
}

void Computer::groupMove(std::vector<GameObject*> group, sf::Vector2f position) {
	/*
	int width = 5;
	std::vector<sf::Vector2f> nonRotatedPositions;

	//build grid of positions around the targetLocation
	int widthCounter = 0;
	int depthCounter = 0;
	for (int i = 0; i < group.size(); i++) {
		if (widthCounter >= width) {
			widthCounter = 0;
			depthCounter++;
		}
		int yOffset = 25 * widthCounter;
		int xOffset = 25 * depthCounter;
		//check if its in the last row and add an extra offset to center it if it is
		if (group.size() - i <= group.size() % width) {
			nonRotatedPositions.push_back(sf::Vector2f(position.x + xOffset, position.y - ((width * 25) / 2) + yOffset + (12.5 * (width - group.size() % width))));
		}
		else {
			nonRotatedPositions.push_back(sf::Vector2f(position.x + xOffset, position.y - ((width * 25) / 2) + yOffset));

		}
		widthCounter++;
	}


	//get average location of the group
	sf::Vector2f averageLocation;
	for (int i = 0; i < group.size(); i++) {
		averageLocation = averageLocation + group.at(i)->getPosition();
	}
	averageLocation.x = averageLocation.x / group.size();
	averageLocation.y = averageLocation.y / group.size();

	//get the angle in rads from the average location (adds 180 so it makes the potentially shorter row on the back) 
	float angleToTarget = (getAngleToTarget(position, averageLocation) + 180) * (3.14159 / 180);


	//rotate the grid of positions by the angleToTarget											!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! fix angle to target
	std::vector<sf::Vector2f> positions;
	for (int i = 0; i < group.size(); i++) {
		sf::Vector2f rotatedPosition;
		rotatedPosition.x = cos(angleToTarget) * (nonRotatedPositions.at(i).x - position.x) - sin(angleToTarget) * (nonRotatedPositions.at(i).y - position.y) + position.x;
		rotatedPosition.y = sin(angleToTarget) * (nonRotatedPositions.at(i).x - position.x) + cos(angleToTarget) * (nonRotatedPositions.at(i).y - position.y) + position.y;
		positions.push_back(rotatedPosition);
	}

	for (int i = 0; i < group.size(); i++) {
		group.at(i)->commandMove(inBorder(positions.at(i)));
	}
	*/
}

float Computer::getAngleToTarget(sf::Vector2f targetPositionIn, sf::Vector2f positionIn) {
	/*
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
	*/
	return 0.0f;
}

sf::Vector2f Computer::inBorder(sf::Vector2f targetLocation) {
	/*
	//forces values to be in the range of the world space
	if (targetLocation.x < gameOriginX) {
		targetLocation.x = gameOriginX;
	}
	if (targetLocation.x > gameWidth) {
		targetLocation.x = gameWidth;
	}
	if (targetLocation.y < gameOriginY) {
		targetLocation.y = gameOriginY;
	}
	if (targetLocation.y > gameHeight) {
		targetLocation.y = gameHeight;
	}
	return targetLocation;
	*/
	return sf::Vector2f(0,0);
}
