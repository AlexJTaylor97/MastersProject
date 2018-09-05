#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Utilities.h"

#include "infantry.h"
#include "speeder.h"
#include "tank.h"
#include "resource.h"
#include "base.h"
#include "refinery.h"
#include "barracks.h"
#include "outpost.h"
#include "button.h"
#include "computer.h"
#include "cursor.h"
#include "tower.h"
#include "spending.h"
#include <iostream>

using namespace sf;

enum gameState{paused,playing,ended};


class Game : public sf::Drawable
{
private:
	Computer ai;						//the ai that manages the computers assets

	bool debug = false;					//boolean flip to see if the game should display debug infromation or not
	bool placingBuilding = false;		//boolean to see if the player is currently trying to place a building
	bool leftMouseDown = false;			//boolean to see if the left moust key is down
	bool buildingSelected = false;		//boolean to see if the player has a building selected
	
	int playerResources;			//integer to store the player resources

	sf::Vector2f leftClickLocation;	//position of left click 

	gameState state;				//state of the game

	sf::Font font;					//stores the font used in debug outputs

	sf::RectangleShape background;				//sfml shape to store the background 
	std::vector<Resource*> resources;			//vector of resources on the game
	std::vector<GameObject*> playerAssets;	    //vector of the players buildings
	std::vector<GameObject*> selectedUnits;		//vector of teh players selected units

	

	GameObject *selectedBuilding;	//the currently selected building

	Cursor cursor;				//stores the infomration abut the cursor
		
	Button *btnRefinery;		//all of the buttons used in the game
	Button *btnBarracks;
	Button *btnOutpost;
	Button *btnTower;
	Button *btnInfantry;
	Button *btnSpeeder;
	Button *btnTank;

public:
	bool computerWon;   //booleans used by main to see if the game has ended
	bool playerWon;		//

	Game(sf::Vector2i screenSize);
	~Game();

	void draw(RenderTarget &target, RenderStates states)const;				//draw functions
	void drawUi(RenderTarget &target, RenderStates states)const;			//
	void drawDebugUi(RenderTarget &target, RenderStates states)const;		//

	void update(float dt,sf::Vector2f mouseWorldPosition);			//update function

	void processKeyPress(Keyboard::Key code);														//input handling
	void processKeyRelease(Keyboard::Key code);														//
	void processMouseClick(sf::Vector2f mouseWorldPosition, sf::Vector2f mouseUiPosition);			//
	void processMouseRelease(sf::Vector2f mouseWorldPosition);										//
	void processMouseRightClick(sf::Vector2f mouseWorldPosition, sf::Vector2f mouseUiPosition);		//
	void processMouseRightRelease(sf::Vector2f mouseWorldPosition);									//

	gameState getState() { return state; }															//returns the state of the game
	void setState(gameState stateIn) { state = stateIn; }											//used by main to set the state of the game
	int getRandomNumBetween(int min, int max) { return rand() % (max - min + 1) + min; }			//utility function to get random number between two values

	sf::Vector2f getNearestBuilding(objectType::objectType typeIn, team::team teamIn, int xPos, int yPos);	//returns nearest building of type and team from a specific position
	sf::Vector2f getNearestResource(int xPos, int yPos);												//returns nearest resource to position

	sf::Vector2f inBorder(sf::Vector2f targetLocation);												//forces the values in it to be withing the bounds of the world space
	bool isInBorder(sf::Vector2f targetLocation);													//checks if values are in world space
	void groupMove(std::vector<GameObject*> group, sf::Vector2f mousePosition);						//moves teh vecot rof units provided together in formation

	float getAngleToTarget(sf::Vector2f targetPositionIn, sf::Vector2f positionIn);				//returns angle between two game objects
	GameObject* getNearestHostileToUnit(GameObject* unitIn);									//gets nearest hosting to unit provided
	float getDistanceBetween(GameObject* unitIn, GameObject* unitIn2);							//returns distance between units provided
};




#endif