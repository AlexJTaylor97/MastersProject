#include "game.h"
Game::Game(sf::Vector2i screenSize)
{
	//load in the font
	if (!font.loadFromFile(".\\assets\\font.ttf")) {
		std::cout << "Error loading Font" << std::endl;
	}

	playerResources = 500;
	//set the background
	background.setSize(sf::Vector2f(gameWidth - 10, gameHeight - 5));
	background.setFillColor(sf::Color(100,55,32,255));
	background.setOutlineColor(sf::Color(155, 160, 160, 255));
	background.setOutlineThickness(5);
	background.setPosition(5, 5);

	//give the player and ai 4 resources near their base each time
	resources.push_back(new Resource(gameOriginX + 250 + getRandomNumBetween(-60, 60), gameHeight - 150 + getRandomNumBetween(-60, 60)));
	resources.push_back(new Resource(gameOriginX + 600 + getRandomNumBetween(-60, 60), gameHeight - 150 + getRandomNumBetween(-60, 60)));
	resources.push_back(new Resource(gameOriginX + 150 + getRandomNumBetween(-60, 60), gameHeight - 400 + getRandomNumBetween(-60, 60)));
	resources.push_back(new Resource(gameOriginX + 500 + getRandomNumBetween(-60, 60), gameHeight - 500 + getRandomNumBetween(-60, 60)));
	resources.push_back(new Resource(gameWidth - 250 + getRandomNumBetween(-60, 60), gameOriginY + 100 + getRandomNumBetween(-60, 60)));
	resources.push_back(new Resource(gameWidth - 600 + getRandomNumBetween(-60, 60), gameOriginY + 100 + getRandomNumBetween(-60, 60)));
	resources.push_back(new Resource(gameWidth - 150 + getRandomNumBetween(-60, 60), gameOriginY + 400 + getRandomNumBetween(-60, 60)));
	resources.push_back(new Resource(gameWidth - 500 + getRandomNumBetween(-60, 60), gameOriginY + 500 + getRandomNumBetween(-60, 60)));

	//populate teh map with random resouces
	for (int i = 0; i < 22; i++) {
		float x = rand() % ((gameWidth - 50) - (gameOriginX + 50) + 1) + (gameOriginX + 50);
		float y = rand() % ((gameHeight - 50) - (gameOriginY + 50) + 1) + (gameOriginY + 50);
		resources.push_back(new Resource(x, y));
	}

	//give the ai the positions of these resouces
	ai = Computer(resources);

	// spawn the bases
	playerAssets.push_back (new Base(sf::Vector2f(gameOriginX + 100, gameHeight - 100), team::player));
	ai.computerAssets.push_back(new Base(sf::Vector2f(gameWidth - 100, gameOriginY + 100), team::computer));

	//create the buttons
	btnRefinery = new Button(sf::Vector2f(((((screenSize.x / 5) / 3) + 5) / 2),		((screenSize.y / 10) + 5) / 2), "Refinery",false, screenSize);
	btnBarracks = new Button(sf::Vector2f((((((screenSize.x / 5) / 3) + 5) / 2) * 3), ((screenSize.y / 10) + 5) / 2), "Barracks",false, screenSize);
	btnOutpost	= new Button(sf::Vector2f((((((screenSize.x / 5) / 3) + 5) / 2) * 5), ((screenSize.y / 10) + 5) / 2), "Outpost", false, screenSize);
	btnTower	= new Button(sf::Vector2f(((((screenSize.x / 5) / 3) + 5) / 2),		(((screenSize.y / 10) + 5) / 2) * 3), "Tower", false, screenSize);
	btnRefinery->updateAmount(refineryCost);
	btnBarracks->updateAmount(barracksCost);
	btnOutpost->updateAmount(outpostCost);
	btnTower->updateAmount(towerCost);


	btnInfantry = new Button(sf::Vector2f(((((screenSize.x / 5) / 3) + 5) / 2),		(((screenSize.y / 10) + 5) / 2) * 5), "Infantry",true, screenSize);
	btnSpeeder	= new Button(sf::Vector2f((((((screenSize.x / 5) / 3) + 5) / 2) * 3), (((screenSize.y / 10) + 5) / 2) * 5), "Speeder",true, screenSize);
	btnTank		= new Button(sf::Vector2f((((((screenSize.x / 5) / 3) + 5) / 2) * 5), (((screenSize.y / 10) + 5) / 2) * 5), "Tank",true, screenSize);
}

Game::~Game()
{

}

//general functions 
void Game::draw(RenderTarget &target, RenderStates states) const
{
	//draw the background first
	target.draw(background);
	target.draw(cursor);

	//draw debug assets if prompted
	/*
	if (debug) {
		for (int i = 0; i < playerBuildings.size(); i++) {
			playerBuildings.at(i)->drawDebug(target, states); 
		}
		for (int i = 0; i < playerUnits.size(); i++) { 
			playerUnits.at(i)->drawDebug(target, states); 
		}
	}
	*/
	//display build radiuses if placing building

	//loop through all assets and draw them
	for (int i = 0; i < resources.size(); i++) {target.draw(*resources.at(i));}
	for (int i = 0; i < playerAssets.size(); i++) {	
		if (playerAssets.at(i)->getComponent<GraphicsComponent>()) {
	
			target.draw(*playerAssets.at(i)->getComponent<GraphicsComponent>()->getSprite());
		}
		if (playerAssets.at(i)->getComponent<SelectableComponent>() && playerAssets.at(i)->getComponent<SelectableComponent>()->isSelected()) {
			target.draw(*playerAssets.at(i)->getComponent<SelectableComponent>()->getSelectionCircle());
			
			if (playerAssets.at(i)->getComponent<CombatComponent>()) {
				target.draw(*playerAssets.at(i)->getComponent<CombatComponent>()->getHealthRed());
				target.draw(*playerAssets.at(i)->getComponent<CombatComponent>()->getHealthGreen());
			}
			if (playerAssets.at(i)->getComponent<SpawnerComponent>()) {
				target.draw(*playerAssets.at(i)->getComponent<SpawnerComponent>()->getRallyFlag());
			}
		}

		if (placingBuilding || debug) {
			if (playerAssets.at(i)->getType() == objectType::base || playerAssets.at(i)->getType() == objectType::outpost) {
				target.draw(*playerAssets.at(i)->getComponent<BuildRadiusComponent>()->getBuildRadiusSprite());
			}
		}

	}


	for (int i = 0; i < ai.computerAssets.size(); i++) { 
		if (ai.computerAssets.at(i)->getComponent<GraphicsComponent>()) {
			target.draw(*ai.computerAssets.at(i)->getComponent<GraphicsComponent>()->getSprite());
		}
	}
}

void Game::drawUi(RenderTarget &target, RenderStates states) const
{
	//draws all of the buttons
	target.draw(*btnRefinery);
	target.draw(*btnBarracks);
	target.draw(*btnOutpost);
	target.draw(*btnTower);
	//if barracks is selected draw the unti build buttons
	if (buildingSelected) {
		if (selectedBuilding->getType() == objectType::barracks) {
			target.draw(*btnInfantry);
			target.draw(*btnSpeeder);
			target.draw(*btnTank);

		}
	}
	//call the draw debug function if debugging 
	if (debug) {
		drawDebugUi(target, states);
	}

	//create and display the players resources
	sf::Text displayText;
	String text;

	displayText.setCharacterSize(38);
	displayText.setFont(font);
	displayText.setFillColor(sf::Color::Blue);

	displayText.setPosition(gameWidth / 2,  50);
	text = std::to_string(playerResources);
	displayText.setString(text);
	target.draw(displayText);
}

void Game::drawDebugUi(RenderTarget &target, RenderStates states)const
{
	//draw recent spending
	sf::Text displayText;
	String text;
	sf::RectangleShape debugBackground;

	debugBackground.setPosition(2400, 0);
	debugBackground.setSize(sf::Vector2f(750,3000));
	debugBackground.setFillColor(sf::Color::Black);
	target.draw(debugBackground);

	displayText.setCharacterSize(38);
	displayText.setFont(font);
	displayText.setFillColor(sf::Color::White);

	//computer information
	displayText.setPosition(gameWidth - 600, 50);
	text = "Computer resources: " + std::to_string(ai.computerResources);
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 600, 100);
	switch (ai.constructionState) {
	case(0):
		text = "Current construction state: Economy";
		break;
	case(1):
		text = "Current construction state: Defence";
		break;
	case(2):
		text = "Current construction state: Military";
		break;
	}
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 550, 140);
			switch (ai.task) {
			case(0):
				text = "Construction task: build Refinery";
				break;
			case(1):
				text = "Construction task: build Outpost";
				break;
			case(2):
				text = "Construction task: Build Tower";
				break;
			case(3):
				text = "Construction task: Build Barracks";
				break;
			case(4):
				text = "Construction task: Build units";
				break;
		}
	displayText.setString(text);
	target.draw(displayText);


	displayText.setPosition(gameWidth - 600, 190);
	switch (ai.actionState) {
	case(0):
		text = "Current action state: Idle";
		break;
	case(1):
		switch (ai.bestAttackStratergy) {
		case(0):
			text = "Current action state: Attacking base";
			break;
		case(1):
			text = "Current action state: Attacking production";
			break;
		case(2):
			text = "Current action state: Attacking economy";
			break;
		}
		break;
	case(2):
		switch (ai.bestDefenceStratergy) {
		case(0):
			text = "Current action state: Counter attacking";
			break;
		case(1):
			text = "Current action state: Defending base";
			break;
		case(2):
			text = "Current action state: Retreating";
			break;
		}
		break;
	}
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 600, 280);
	text = "Computer boolean logic";
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 550, 320);
	text = "Has been attacked recently: " + std::to_string(ai.attackedRecently);
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 550, 360);
	text = "Unused resource in range: " + std::to_string(ai.resourceInRange);
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 550, 400);
	text = "Training at capacity: " + std::to_string(ai.trainingAtCapacity);
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 550, 440);
	text = "Should attack: " + std::to_string(ai.shouldAttack);
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 600, 490);
	text = "Computer statistics ";
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 550, 530);
	text = "Amount of refineries: " + std::to_string(ai.amountOfRefinerys);
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 550, 570);
	text = "Amount of outposts: " + std::to_string(ai.amountOfOutposts);
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 550, 610);
	text = "Amount of towers: " + std::to_string(ai.amountOfTowers);
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 550, 650);
	text = "Amount of barracks: " + std::to_string(ai.amountOfBarracks);
	displayText.setString(text);
	target.draw(displayText);
	//Needs updating!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	/*
	displayText.setPosition(gameWidth - 550, 690);
	text = "Amount of units: " + std::to_string(ai.computerUnits.size());
	displayText.setString(text);
	target.draw(displayText);
	*/
	//Fuzzy logic data
	displayText.setPosition(gameWidth - 600, 800);
	text = "Total recent spending : " + std::to_string(ai.totalRecentSpending);
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 550, 840);
	text = "Military spending : " + std::to_string((ai.recentMilSpending * 100.) / 100.) + "  " + std::to_string((ai.recentMilSpendingPercent* 100.) / 100.) + "%";
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 550, 880);
	text = "Economic spending : " + std::to_string((ai.recentEcoSpending* 100.) / 100.) + "  " + std::to_string((ai.recentEcoSpendingPercent* 100.) / 100.) + "%";
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 550, 920);
	text = "Defence spending : " + std::to_string((ai.recentDefSpending* 100.) / 100.) + "  " + std::to_string((ai.recentDefSpendingPercent* 100.) / 100.) + "%";
	displayText.setString(text);
	target.draw(displayText);

	displayText.setPosition(gameWidth - 600, 970);
	text = "Aggression outputted: " + std::to_string(ai.aggressionCalculated);
	displayText.setString(text);
	target.draw(displayText);
}

void Game::update(float dt, sf::Vector2f mousePosition)
{
	//check if the player or computer base is destroyed
	for (int i = 0; i < playerAssets.size(); i++) {
		if (playerAssets.at(i)->getType() == objectType::base) {
			if (playerAssets.at(i)->getComponent<CombatComponent>()->getHealth() < 0) {
				computerWon = true;
			}
		}
	}

	for (int i = 0; i < ai.computerAssets.size(); i++) {
		if (ai.computerAssets.at(i)->getType() == objectType::base) {
			if (ai.computerAssets.at(i)->getComponent<CombatComponent>()->getHealth() < 0) {
				playerWon = true;
			}
		}
	}

	cursor.update(dt,mousePosition);
	//update loops 
	//updates player spending data
	ai.totalRecentSpending = 0;
	ai.recentMilSpending = 0;
	ai.recentEcoSpending = 0;
	ai.recentDefSpending = 0;
	for (int i = 0; i < ai.recentSpending.size(); i++) {
		ai.recentSpending.at(i).update(dt);
		if (!ai.recentSpending.at(i).isValid()) {
			ai.recentSpending.erase(ai.recentSpending.begin() + i);
		}
	}
	for (int i = 0; i < ai.recentSpending.size(); i++) {
		ai.totalRecentSpending = ai.totalRecentSpending + ai.recentSpending.at(i).getAmount();

		if (ai.recentSpending.at(i).getType() == 'm') {
			ai.recentMilSpending = ai.recentMilSpending + ai.recentSpending.at(i).getAmount();
		}
		if (ai.recentSpending.at(i).getType() == 'e') {
			ai.recentEcoSpending = ai.recentEcoSpending + ai.recentSpending.at(i).getAmount();
		}
		if (ai.recentSpending.at(i).getType() == 'd') {
			ai.recentDefSpending = ai.recentDefSpending + ai.recentSpending.at(i).getAmount();
		}
	}

	ai.recentMilSpendingPercent = (ai.recentMilSpending / ai.totalRecentSpending) * 100;
	ai.recentEcoSpendingPercent = (ai.recentEcoSpending / ai.totalRecentSpending) * 100;
	ai.recentDefSpendingPercent = (ai.recentDefSpending / ai.totalRecentSpending) * 100;



	//Updates player assets
	if (playerAssets.size() > 0) {
		for (int i = 0; i < playerAssets.size(); i++) {
			playerAssets.at(i)->update(dt);
			/////////////////////////////////Units////////////////////////////////////////
			if (playerAssets.at(i)->getComponent<MovementComponent>()) {

				if (ai.computerAssets.size() > 0) {
					//check if units can atack and damage units if they can
					if (playerAssets.at(i)->getComponent<CombatComponent>()->canAttack() && getDistanceBetween(getNearestHostileToUnit(playerAssets.at(i)), playerAssets.at(i)) < playerAssets.at(i)->getComponent<CombatComponent>()->getRange()) {
						getNearestHostileToUnit(playerAssets.at(i))->getComponent<CombatComponent>()->damage(playerAssets.at(i)->getComponent<CombatComponent>()->getDamage());
						if (getNearestHostileToUnit(playerAssets.at(i))->getType() == objectType::barracks || getNearestHostileToUnit(playerAssets.at(i))->getType() == objectType::refinery || getNearestHostileToUnit(playerAssets.at(i))->getType() == objectType::outpost || getNearestHostileToUnit(playerAssets.at(i))->getType() == objectType::tower || getNearestHostileToUnit(playerAssets.at(i))->getType() == objectType::base) {
							ai.attackedRecently = true;
							ai.attackedRecentlyTimer = 0.0f;
							ai.attackLocation = playerAssets.at(i)->getComponent<TransformComponent>()->getPosition();
						}
					}
				}

				//deselects and erases dead units before the game deletes them properly
				if (selectedUnits.size() > 0) {
					for (int i = 0; i < selectedUnits.size(); i++) {
						if (selectedUnits.at(i)->getComponent<CombatComponent>()->getHealth() < 0) {
							selectedUnits.at(i)->getComponent<SelectableComponent>()->deSelect();
							selectedUnits.erase(selectedUnits.begin() + i);
						}
					}
				}
				//destroy dead units
				if (playerAssets.at(i)->getComponent<CombatComponent>()->getHealth() < 0) {
					delete playerAssets.at(i);
					playerAssets.erase(playerAssets.begin() + i);
					i--;
				}
			}
			/////////////////////////////////Buildings////////////////////////////////////////
			if (!playerAssets.at(i)->getComponent<MovementComponent>()) {
				float barracksDistance = 9999, refineryDistance = 9999;
				sf::Vector2f barracksPosition, refineryPosition;
				int amountOfPlayerRefineries = 0;
				int amountOfPlayerBarracks = 0;
				float tempDistance;
				//barracks special behaviour (spawning)
				if (playerAssets.at(i)->getType() == objectType::barracks) {
					amountOfPlayerBarracks++;
					if (playerAssets.at(i)->getComponent<SpawnerComponent>()->spawnInfantry()) {
						playerAssets.push_back(new Infantry(playerAssets.at(i)->getComponent<TransformComponent>()->getPosition(), team::player));
						playerAssets.back()->getComponent<MovementComponent>()->commandMove(inBorder(playerAssets.at(i)->getComponent<SpawnerComponent>()->getRallyPoint()));
					}
					if (playerAssets.at(i)->getComponent<SpawnerComponent>()->spawnSpeeder()) {
						playerAssets.push_back(new Speeder(playerAssets.at(i)->getComponent<TransformComponent>()->getPosition(), team::player));
						playerAssets.back()->getComponent<MovementComponent>()->commandMove(inBorder(playerAssets.at(i)->getComponent<SpawnerComponent>()->getRallyPoint()));
					}
					if (playerAssets.at(i)->getComponent<SpawnerComponent>()->spawnTank()) {
						playerAssets.push_back(new Tank(playerAssets.at(i)->getComponent<TransformComponent>()->getPosition(), team::player));
						playerAssets.back()->getComponent<MovementComponent>()->commandMove(inBorder(playerAssets.at(i)->getComponent<SpawnerComponent>()->getRallyPoint()));
					}
					float dX, dY;
					dX = playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x - gameWidth - 100;
					dY = playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y - gameOriginY + 100;

					//get distence between us and the ideal location
					tempDistance = sqrt(pow(dX, 2) + pow(dY, 2));
					if (tempDistance < barracksDistance) {
						barracksDistance = tempDistance;
						barracksPosition = playerAssets.at(i)->getComponent<TransformComponent>()->getPosition();
					}
				}

				//tower special behaviour (attacking)
				if (playerAssets.at(i)->getType() == objectType::tower) {
					if (playerAssets.at(i)->getComponent<CombatComponent>()->canAttack() && getDistanceBetween(playerAssets.at(i), getNearestHostileToUnit(playerAssets.at(i))) < playerAssets.at(i)->getComponent<CombatComponent>()->getRange()) {
						getNearestHostileToUnit(playerAssets.at(i))->getComponent<CombatComponent>()->damage(playerAssets.at(i)->getComponent<CombatComponent>()->getDamage());
					}
				}

				//refinery special behaviour
				if (playerAssets.at(i)->getType() == objectType::refinery) {
					amountOfPlayerRefineries++;
					if (playerAssets.at(i)->getComponent<ResourceProducerComponent>()->isFinishedMining()) {
						playerAssets.at(i)->getComponent<ResourceProducerComponent>()->completedMining();
						playerResources = playerResources + refineryOutput;
					}

					float dX, dY;
					dX = playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x - gameWidth - 100;
					dY = playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y - gameOriginY + 100;

					//get distence between us and the ideal location
					tempDistance = sqrt(pow(dX, 2) + pow(dY, 2));
					if (tempDistance < refineryDistance) {
						refineryDistance = tempDistance;
						refineryPosition = playerAssets.at(i)->getComponent<TransformComponent>()->getPosition();
					}
				}

				//base special behaviour
				if (playerAssets.at(i)->getType() == objectType::base) {
					if (playerAssets.at(i)->getComponent<ResourceProducerComponent>()->isFinishedMining()) {
						playerAssets.at(i)->getComponent<ResourceProducerComponent>()->completedMining();
						playerResources = playerResources + baseOutput;
					}
				}


				if (buildingSelected && selectedBuilding->getComponent<CombatComponent>()->getHealth() == 0) {

					buildingSelected = false;
					selectedBuilding = nullptr;
				}

				//destory dead buildings
				if (playerAssets.at(i)->getComponent<CombatComponent>()->getHealth() < 0 && playerAssets.at(i)->getType() != objectType::base) {
					playerAssets.erase(playerAssets.begin() + i);
					i--;
				}

				//update ai infomration on player buildings
				if (barracksDistance < 9999) {
					ai.closestPlayerBarracks = barracksPosition;
				}
				else {
					ai.closestPlayerBarracks = playerBaseLocation;
				}
				if (refineryDistance < 9999) {
					ai.closestPlayerRefinery = refineryPosition;
				}
				else {
					ai.closestPlayerRefinery = playerBaseLocation;
				}
				ai.amountOfPlayerBarracks = amountOfPlayerBarracks;
				ai.amountOfPlayerRefineries = amountOfPlayerRefineries;
			}
		}
	}
	//update ai data
	//Needs updating!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//ai.amountOfPlayerUnits = playerUnits.size();
	
	//Updates ai units
	if (ai.computerAssets.size() > 0) {
		for (int i = 0; i < ai.computerAssets.size(); i++) {
			ai.computerAssets.at(i)->update(dt);
			/////////////////////////////////Units////////////////////////////////////////
			if (ai.computerAssets.at(i)->getComponent<MovementComponent>()) {
				//damage hostile units if ai units can attack
				if (playerAssets.size() > 0) {
					if (ai.computerAssets.at(i)->getComponent<CombatComponent>()->canAttack() && getDistanceBetween(ai.computerAssets.at(i), getNearestHostileToUnit(ai.computerAssets.at(i))) < ai.computerAssets.at(i)->getComponent<CombatComponent>()->getRange()) {
						getNearestHostileToUnit(ai.computerAssets.at(i))->getComponent<CombatComponent>()->damage(ai.computerAssets.at(i)->getComponent<CombatComponent>()->getDamage());
					}
				}
				//deselects and erases dead units before the game deletes them properly
				if (ai.computerSelectedUnits.size() > 0) {
					for (int i = 0; i < ai.computerSelectedUnits.size(); i++) {
						if (ai.computerSelectedUnits.at(i)->getComponent<CombatComponent>()->getHealth() < 0) {
							ai.computerSelectedUnits.at(i)->getComponent<SelectableComponent>()->deSelect();
							ai.computerSelectedUnits.erase(ai.computerSelectedUnits.begin() + i);
						}
					}
				}

				//destroy dead units
				if (ai.computerAssets.at(i)->getComponent<CombatComponent>()->getHealth() < 0) {
					delete ai.computerAssets.at(i);
					ai.computerAssets.erase(ai.computerAssets.begin() + i);
					i--;
				}
			}

			/////////////////////////////////Buildings////////////////////////////////////////
			if (!ai.computerAssets.at(i)->getComponent<MovementComponent>()) {
				//barracks special behaviour (spawning)
				if (ai.computerAssets.at(i)->getType() == objectType::barracks) {
					if (ai.computerAssets.at(i)->getComponent<SpawnerComponent>()->spawnInfantry()) {
						ai.computerAssets.push_back(new Infantry(ai.computerAssets.at(i)->getComponent<TransformComponent>()->getPosition(), team::computer));
						ai.computerAssets.back()->getComponent<MovementComponent>()->commandMove(inBorder(ai.computerAssets.at(i)->getComponent<SpawnerComponent>()->getRallyPoint()));

					}
					if (ai.computerAssets.at(i)->getComponent<SpawnerComponent>()->spawnSpeeder()) {
						ai.computerAssets.push_back(new Speeder(ai.computerAssets.at(i)->getComponent<TransformComponent>()->getPosition(), team::computer));
						ai.computerAssets.back()->getComponent<MovementComponent>()->commandMove(inBorder(ai.computerAssets.at(i)->getComponent<SpawnerComponent>()->getRallyPoint()));
					}
					if (ai.computerAssets.at(i)->getComponent<SpawnerComponent>()->spawnTank()) {
						ai.computerAssets.push_back(new Tank(ai.computerAssets.at(i)->getComponent<TransformComponent>()->getPosition(), team::computer));
						ai.computerAssets.back()->getComponent<MovementComponent>()->commandMove(inBorder(ai.computerAssets.at(i)->getComponent<SpawnerComponent>()->getRallyPoint()));
					}
				}


				//tower special behaviour (atttacking)
				if (ai.computerAssets.at(i)->getType() == objectType::tower) {
					if (ai.computerAssets.at(i)->getComponent<CombatComponent>()->canAttack() && getDistanceBetween(ai.computerAssets.at(i), getNearestHostileToUnit(ai.computerAssets.at(i))) < ai.computerAssets.at(i)->getComponent<CombatComponent>()->getRange()) {
						getNearestHostileToUnit(ai.computerAssets.at(i))->getComponent<CombatComponent>()->damage(ai.computerAssets.at(i)->getComponent<CombatComponent>()->getDamage());
					}
				}

				//refinery special behaviour
				if (ai.computerAssets.at(i)->getType() == objectType::refinery) {
					if (ai.computerAssets.at(i)->getComponent<ResourceProducerComponent>()->isFinishedMining()) {
						ai.computerAssets.at(i)->getComponent<ResourceProducerComponent>()->completedMining();
						ai.addResources(refineryOutput);
					}
				}

				//base special behaviour
				if (ai.computerAssets.at(i)->getType() == objectType::base) {
					if (ai.computerAssets.at(i)->getComponent<ResourceProducerComponent>()->isFinishedMining()) {
						ai.computerAssets.at(i)->getComponent<ResourceProducerComponent>()->completedMining();
						ai.addResources(refineryOutput);
					}
				}

				//destroy dead buidlings
				if (ai.computerAssets.at(i)->getComponent<CombatComponent>()->getHealth() < 0 && ai.computerAssets.at(i)->getType() != objectType::base) {
					ai.computerAssets.erase(ai.computerAssets.begin() + i);
					i--;
				}
			}
		}
	}
	

	//updates numbers displayed on buttons if barrack is selected
	if (buildingSelected) {
		if (selectedBuilding->getType() == objectType::barracks) {
			btnInfantry->updateAmount(infantryCost, selectedBuilding->getComponent<SpawnerComponent>()->getInfantryBeingTrained());
			btnSpeeder->updateAmount(speederCost, selectedBuilding->getComponent<SpawnerComponent>()->getSpeedersBeingTrained());
			btnTank->updateAmount(tankCost, selectedBuilding->getComponent<SpawnerComponent>()->getTanksBeingTrained());
		}
	}

	//selects units
	if (cursor.isPlacingBuilding()) {
		placingBuilding = true;
	}
	else {
		placingBuilding = false;
	}

	//alter the size of the selection box
	if (cursor.isSelectingUnits()) {
		sf::Vector2f boxPosition = cursor.getBoxPosition();
		sf::Vector2f boxHalfExtents = cursor.getBoxHalfExtents();
		
		if (selectedUnits.size() > 0) {
			for (int i = 0; i < selectedUnits.size(); i++) {
				selectedUnits.at(i)->getComponent<SelectableComponent>()->deSelect();
				selectedUnits.erase(selectedUnits.begin() + i);
			}
		}

		for (int i = 0; i < playerAssets.size(); i++) {
			if (playerAssets.at(i)->getComponent<MovementComponent>() &&
				boxPosition.x - boxHalfExtents.x < playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x + playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().x &&
				boxPosition.x + boxHalfExtents.x > playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x - playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().x &&
				boxPosition.y - boxHalfExtents.y < playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y + playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().y &&
				boxPosition.y + boxHalfExtents.y > playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y - playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().y &&
				playerAssets.at(i)->getComponent<SelectableComponent>()->isSelected() == false
				) {
				playerAssets.at(i)->getComponent<SelectableComponent>()->select();
				selectedUnits.push_back(playerAssets.at(i));
			}
		}
	}

	//select groups of units
	if (leftMouseDown && (leftClickLocation.x != mousePosition.x || leftClickLocation.y != mousePosition.y)) {
		cursor.groupSelect(inBorder(leftClickLocation));
	}


	//update AI (AI manages which parts update as some of them are quite demanding)
	//ai.update(dt);
	
}

//event handling functions
//key events
void Game::processKeyPress(Keyboard::Key code)
{
	//switch between debug 
	if (code == Keyboard::Key::D) {
		debug = !debug;
	}

	//stops selected units
	if (code == Keyboard::Key::S) {
		if (selectedUnits.size() > 0) {
			for (int i = 0; i < selectedUnits.size(); i++) {
				selectedUnits.at(i)->getComponent<MovementComponent>()->stop();
			}
		}
	}
}

void Game::processKeyRelease(Keyboard::Key code)
{

}

//mouse events
void Game::processMouseClick(sf::Vector2f mouseWorldPosition, sf::Vector2f mouseUiPosition)
{
	leftMouseDown = true;
	leftClickLocation = mouseWorldPosition;

	//if clicked while placing building
	if (cursor.isPlacingBuilding()) {
		//see if it is selecting a builing
		bool inBuildRange = false;
		for (int i = 0; i < playerAssets.size(); i++) {
			if (!playerAssets.at(i)->getComponent<MovementComponent>()) {
				int tempDx, tempDy;
				if (playerAssets.at(i)->getType() == objectType::base) {
					tempDx = abs(playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x - mouseWorldPosition.x);
					tempDy = abs(playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y - mouseWorldPosition.y);
					float buildRangeDistance = sqrt(pow(tempDx, 2) + pow(tempDy, 2));
					if (buildRangeDistance < baseBuildRadius) {
						inBuildRange = true;
					}
				}
				if (playerAssets.at(i)->getType() == objectType::outpost) {
					tempDx = abs(playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x - mouseWorldPosition.x);
					tempDy = abs(playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y - mouseWorldPosition.y);
					float buildRangeDistance = sqrt(pow(tempDx, 2) + pow(tempDy, 2));
					if (buildRangeDistance < outpostBuildRadius) {
						inBuildRange = true;
					}
				}
			}
		}
		bool colliding = false;
		int tempHalfExtentX;
		int tempHalfExtentY;
		if (cursor.getSelectedBuilding() == building::bldRefinery) {
			tempHalfExtentX = 12;
			tempHalfExtentY = 25;
		}
		if (cursor.getSelectedBuilding() == building::bldOutpost) {
			tempHalfExtentX = 15;
			tempHalfExtentY = 15;
		}
		if (cursor.getSelectedBuilding() == building::bldBarracks) {
			tempHalfExtentX = 25;
			tempHalfExtentY = 12;
		}
		if (cursor.getSelectedBuilding() == building::bldTower) {
			tempHalfExtentX = 10;
			tempHalfExtentY = 10;
		}
		//check if it collides with an existing building
		for (int i = 0; i < playerAssets.size(); i++) {
			if (!playerAssets.at(i)->getComponent<MovementComponent>()) {
				if (mouseWorldPosition.x - tempHalfExtentX < playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x + playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().x && mouseWorldPosition.x + tempHalfExtentX > playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x - playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().x && mouseWorldPosition.y - tempHalfExtentY < playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y + playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().y && mouseWorldPosition.y + tempHalfExtentY > playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y - playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().y) {
					colliding = true;
				}
			}
		}
		//build the building if the player can affoard it, if its a refinery also check if it is in range of a resource node
		if (inBuildRange && !colliding && isInBorder(mouseWorldPosition)) {
			if (cursor.getSelectedBuilding() == building::bldRefinery && playerResources > refineryCost) {
				//check if its in building range
				//check which resource its on
				float resourceDistance;
				for (int i = 0; i < resources.size(); i++) {
					int tempDx, tempDy;
					tempDx = abs(resources.at(i)->getPosition().x - mouseWorldPosition.x);
					tempDy = abs(resources.at(i)->getPosition().y - mouseWorldPosition.y);
					resourceDistance = sqrt(pow(tempDx, 2) + pow(tempDy, 2));

					if (resourceDistance < 25 && resources.at(i)->isClaimed() == false && inBuildRange) {

						playerResources = playerResources - refineryCost;
						ai.recentSpending.push_back(Spending('e', refineryCost));

						playerAssets.push_back(new Refinery(resources.at(i)->getPosition(), team::player));
						cursor.stopPlacingBuilding();
						resources.at(i)->claimResource();
					}
				}
			}

			//build barracks
			if (cursor.getSelectedBuilding() == building::bldBarracks && playerResources > barracksCost) {
				playerResources = playerResources - barracksCost;
				ai.recentSpending.push_back(Spending('m', barracksCost));
				playerAssets.push_back(new Barracks(mouseWorldPosition, team::player));
				cursor.stopPlacingBuilding();
			}
			//build outpost
			if (cursor.getSelectedBuilding() == building::bldOutpost && playerResources > outpostCost) {
				playerResources = playerResources - outpostCost;
				ai.recentSpending.push_back(Spending('e', outpostCost / 2));
				ai.recentSpending.push_back(Spending('d', outpostCost / 2));
				playerAssets.push_back(new Outpost(mouseWorldPosition, team::player));
				cursor.stopPlacingBuilding();
			}
			//build tower
			if (cursor.getSelectedBuilding() == building::bldTower && playerResources > towerCost) {
				playerResources = playerResources - towerCost;
				ai.recentSpending.push_back(Spending('d', towerCost));
				playerAssets.push_back(new Tower(mouseWorldPosition, team::player));
				cursor.stopPlacingBuilding();
			}
		}
	}

	//button checks
	if (btnRefinery->isClicked(mouseUiPosition) && playerResources >= refineryCost) {
		sf::RectangleShape temp;
		temp.setOrigin(25, 50);
		temp.setSize(sf::Vector2f(50, 100));
		temp.setFillColor(sf::Color::Blue);
		cursor.setSelectedBuilding(building::bldRefinery);
		cursor.placeBuilding(temp);
	}
	if (btnBarracks->isClicked(mouseUiPosition) && playerResources >= barracksCost) {
		sf::RectangleShape temp;
		temp.setOrigin(50, 25);
		temp.setSize(sf::Vector2f(100, 50));
		temp.setFillColor(sf::Color::Blue);
		cursor.setSelectedBuilding(building::bldBarracks);
		cursor.placeBuilding(temp);
	}
	if (btnOutpost->isClicked(mouseUiPosition) && playerResources >= outpostCost) {
		sf::RectangleShape temp;
		temp.setOrigin(30, 30);
		temp.setSize(sf::Vector2f(60, 60));
		temp.setFillColor(sf::Color::Blue);
		cursor.setSelectedBuilding(building::bldOutpost);
		cursor.placeBuilding(temp);
	}
	if (btnTower->isClicked(mouseUiPosition) && playerResources >= towerCost) {
		sf::RectangleShape temp;
		temp.setOrigin(20, 20);
		temp.setSize(sf::Vector2f(40, 40));
		temp.setFillColor(sf::Color::Blue);
		cursor.setSelectedBuilding(building::bldTower);
		cursor.placeBuilding(temp);
	}

	//building selcected checks
	bool buttonPressed = false;
	//click cases if a building is selected
	if (buildingSelected) {
		if (btnInfantry->isClicked(mouseUiPosition) && selectedBuilding->getType() == objectType::barracks  && playerResources >= infantryCost) {
			buttonPressed = true;
			playerResources = playerResources - infantryCost;
			selectedBuilding->getComponent<SpawnerComponent>()->buildUnit(objectType::infantry);
			ai.recentSpending.push_back(Spending('m', infantryCost));
		}
		if (btnSpeeder->isClicked(mouseUiPosition) && selectedBuilding->getType() == objectType::barracks && playerResources >= speederCost) {
			buttonPressed = true;
			playerResources = playerResources - speederCost;
			selectedBuilding->getComponent<SpawnerComponent>()->buildUnit(objectType::speeder);
			ai.recentSpending.push_back(Spending('m', speederCost));
		}
		if (btnTank->isClicked(mouseUiPosition) && selectedBuilding->getType() == objectType::barracks && playerResources >= tankCost) {
			buttonPressed = true;
			playerResources = playerResources - tankCost;
			selectedBuilding->getComponent<SpawnerComponent>()->buildUnit(objectType::tank);
			ai.recentSpending.push_back(Spending('m', tankCost));
		}
	
	
	}
	//if no button was pressed deSelect the building
	if (buildingSelected && !buttonPressed) {
		selectedBuilding->getComponent<SelectableComponent>()->deSelect();
		buildingSelected = false;
		selectedBuilding = nullptr;
	}	
	
	//if clicked in map with units selected
	if (selectedUnits.size() > 0 && !buttonPressed) {
		for (int i = 0; i < selectedUnits.size(); i++) {
			selectedUnits.at(i)->getComponent<SelectableComponent>()->deSelect();
		}
		selectedUnits.clear();
	}

	//if clicked while not placing building
	if (!cursor.isPlacingBuilding()) {
		//check if it is selecting a buiilding
		for (int i = 0; i < playerAssets.size(); i++) {
			if (!playerAssets.at(i)->getComponent<MovementComponent>()) {
				if (mouseWorldPosition.x < playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x + playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().x && mouseWorldPosition.x > playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x - playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().x &&
					mouseWorldPosition.y < playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y + playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().y && mouseWorldPosition.y > playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y - playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().y
					) {
					selectedBuilding = playerAssets.at(i);
					playerAssets.at(i)->getComponent<SelectableComponent>()->select();
					buildingSelected = true;
				}
			}
		}
		//check if it is selecting a unit
		for (int i = 0; i < playerAssets.size(); i++) {
			if (playerAssets.at(i)->getComponent<MovementComponent>()) {
				if (mouseWorldPosition.x < playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x + playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().x && mouseWorldPosition.x > playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x - playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().x &&
					mouseWorldPosition.y < playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y + playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().y && mouseWorldPosition.y > playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y - playerAssets.at(i)->getComponent<GraphicsComponent>()->getHalfExtents().y
					) {
					if (selectedUnits.size() > 0)
					{
						for (int j = 0; j < selectedUnits.size(); j++) {
							selectedUnits.at(j)->getComponent<SelectableComponent>()->deSelect();
						}
						selectedUnits.clear();
					}
					playerAssets.at(i)->getComponent<SelectableComponent>()->select();
					selectedUnits.push_back(playerAssets.at(i));
				}
			}
		}
	}
}

void Game::processMouseRightClick(sf::Vector2f mouseWorldPosition, sf::Vector2f mouseUiPosition)
{
	//cancels selection 
	cursor.stopPlacingBuilding();
	cursor.stopSelectingUnits();
	if (buildingSelected) {
		if (selectedBuilding->getType() == objectType::barracks) {
			selectedBuilding->getComponent<SpawnerComponent>()->setRallyPoint(inBorder(mouseWorldPosition));
		}
		else {
			selectedBuilding->getComponent<SelectableComponent>()->deSelect();
			buildingSelected = false;
			selectedBuilding = nullptr;
		}
	}
	//moves units (single or grouped)
	if (selectedUnits.size() > 0) {
		if (selectedUnits.size() == 1) {
			selectedUnits.at(0)->getComponent<MovementComponent>()->commandMove(inBorder(mouseWorldPosition));
		}
		else {
			groupMove(selectedUnits, mouseWorldPosition);
		}
	}
}

void Game::processMouseRelease(sf::Vector2f mouseWorldPosition)
{
	cursor.stopSelectingUnits();
	leftMouseDown = false;
}

void Game::processMouseRightRelease(sf::Vector2f mouseWorldPosition)
{
	//if needed
}

//utility functions 
sf::Vector2f Game::getNearestBuilding(objectType::objectType typeIn, team::team teamIn, int xPos, int yPos) {

	if (teamIn == team::player) {
		sf::Vector2f nearestBuilding;
		float distance = 0;
		//calculates the nearest ai building
		for (int i = 0; i < playerAssets.size(); i++) { 
			if (!playerAssets.at(i)->getComponent<MovementComponent>()) {
				if (playerAssets.at(i)->getType() == typeIn) {

					int tempDx, tempDy;
					tempDx = abs(playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x - xPos);
					tempDy = abs(playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y - yPos);

					float tempDistance = sqrt(pow(tempDx, 2) + pow(tempDy, 2));

					if (tempDistance > distance) {
						distance = tempDistance;
						nearestBuilding = playerAssets.at(i)->getComponent<TransformComponent>()->getPosition();
					}
				}
			}
		}
		return nearestBuilding;
	}

	if (teamIn == team::computer) {
		sf::Vector2f nearestBuilding;
		float distance = 0;
		//calculates the nearest player building
		for (int i = 0; i < ai.computerAssets.size(); i++) {
			if (!ai.computerAssets.at(i)->getComponent<MovementComponent>()) {
				if (ai.computerAssets.at(i)->getType() == typeIn) {

					int tempDx, tempDy;
					tempDx = abs(ai.computerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x - xPos);
					tempDy = abs(ai.computerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y - yPos);

					float tempDistance = sqrt(pow(tempDx, 2) + pow(tempDy, 2));

					if (tempDistance > distance) {
						distance = tempDistance;
						nearestBuilding = ai.computerAssets.at(i)->getComponent<TransformComponent>()->getPosition();
					}
				}
			}
		}
		return nearestBuilding;
	}
}

sf::Vector2f Game::getNearestResource(int xPos, int yPos) {
	sf::Vector2f nearestResource;
	float distance = 0;
	//calculates the nearest resource
	for (int i = 0; i < resources.size(); i++) {
		int tempDx, tempDy;
		tempDx = abs(resources.at(i)->getPosition().x - xPos);
		tempDy = abs(resources.at(i)->getPosition().y - yPos);

		float tempDistance = sqrt(pow(tempDx, 2) + pow(tempDy, 2));

		if (tempDistance > distance) {
			distance = tempDistance;
			nearestResource = resources.at(i)->getPosition();
		}
	}
	return nearestResource;
}

sf::Vector2f Game::inBorder(sf::Vector2f targetLocation) {
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
}

bool Game::isInBorder(sf::Vector2f targetLocation) {
	if (targetLocation.x < gameOriginX) {
		return false;
	}
	if (targetLocation.x > gameWidth) {
		return false;
	}
	if (targetLocation.y < gameOriginY) {
		return false;
	}
	if (targetLocation.y > gameHeight) {
		return false;
	}
	return true;
}

float Game::getAngleToTarget(sf::Vector2f targetPositionIn, sf::Vector2f positionIn) {
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

void Game::groupMove(std::vector<GameObject*> group, sf::Vector2f mousePosition) {
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
		if (group.size() - i <= group.size() % width){
			nonRotatedPositions.push_back(sf::Vector2f(mousePosition.x + xOffset, mousePosition.y - ((width * 25) / 2) + yOffset + (12.5 * (width - group.size() % width))));
		}
		else {
			nonRotatedPositions.push_back(sf::Vector2f(mousePosition.x + xOffset, mousePosition.y - ((width * 25) / 2) + yOffset));
		
		}
		widthCounter++;
	}


	//get average location of the group
	sf::Vector2f averageLocation;
	for (int i = 0; i < group.size(); i++) {
		averageLocation = averageLocation + group.at(i)->getComponent<TransformComponent>()->getPosition();
	}
	averageLocation.x = averageLocation.x / group.size();
	averageLocation.y = averageLocation.y / group.size();

	//get the angle in rads from the average location (adds 180 so it makes the potentially shorter row on the back) 
	float angleToTarget = (getAngleToTarget(mousePosition, averageLocation) + 180) * (3.14159 / 180);


	//rotate the grid of positions by the angleToTarget											!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! fix angle to target
	std::vector<sf::Vector2f> positions;
	for (int i = 0; i < group.size(); i++) {
		sf::Vector2f rotatedPosition;
		rotatedPosition.x = cos(angleToTarget) * (nonRotatedPositions.at(i).x - mousePosition.x) - sin(angleToTarget) * (nonRotatedPositions.at(i).y - mousePosition.y) + mousePosition.x;
		rotatedPosition.y = sin(angleToTarget) * (nonRotatedPositions.at(i).x - mousePosition.x) + cos(angleToTarget) * (nonRotatedPositions.at(i).y - mousePosition.y) + mousePosition.y;
		positions.push_back(rotatedPosition);
	}

	for (int i = 0; i < group.size(); i++) {
		group.at(i)->getComponent<MovementComponent>()->commandMove(inBorder(positions.at(i)));
	}
}

GameObject* Game::getNearestHostileToUnit(GameObject* unitIn) {
	float shortestDistance = 99999;
	GameObject* nearestHostile = nullptr;
	if (unitIn->getTeam() == team::player) {
		if (ai.computerAssets.size() > 0) {
			for (int i = 0; i < ai.computerAssets.size(); i++) {
				float dX, dY;
				dX = ai.computerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x - unitIn->getComponent<TransformComponent>()->getPosition().x;
				dY = ai.computerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y - unitIn->getComponent<TransformComponent>()->getPosition().y;

				//get distence between us and the ideal location
				float distance = sqrt(pow(dX, 2) + pow(dY, 2));
				if (distance < shortestDistance) {
					shortestDistance = distance;
					nearestHostile = ai.computerAssets.at(i);
				}
			}
		}
	}
	else {
		if (playerAssets.size() > 0) {
			for (int i = 0; i < playerAssets.size(); i++) {
				float dX, dY;
				dX = playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().x - unitIn->getComponent<TransformComponent>()->getPosition().x;
				dY = playerAssets.at(i)->getComponent<TransformComponent>()->getPosition().y - unitIn->getComponent<TransformComponent>()->getPosition().y;

				//get distence between us and the ideal location
				float distance = sqrt(pow(dX, 2) + pow(dY, 2));
				if (distance < shortestDistance) {
					shortestDistance = distance;
					nearestHostile = playerAssets.at(i);
				}
			}
		}
	}

	return nearestHostile;
}

float Game::getDistanceBetween(GameObject* unitIn, GameObject* unitIn2) {
	float dX, dY;
	dX = unitIn->getComponent<TransformComponent>()->getPosition().x - unitIn2->getComponent<TransformComponent>()->getPosition().x;
	dY = unitIn->getComponent<TransformComponent>()->getPosition().y - unitIn2->getComponent<TransformComponent>()->getPosition().y;

	//get distence between us and the ideal location
	float distance = sqrt(pow(dX, 2) + pow(dY, 2));
	return distance;
}